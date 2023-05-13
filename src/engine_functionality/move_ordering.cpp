
#include "chess_engine.hpp"
#include "move.hpp"
#include "size_defs.hpp"

using namespace std;

// Ranked by manhattan distance from center
const U8 ChessEngine::centerBias[64] {
	60, 50, 45, 35, 35, 45, 50, 60,
	50, 40, 30, 25, 25, 30, 40, 50,
	45, 30, 20, 10, 10, 20, 30, 45,
	35, 25, 10,  0,  0, 10, 25, 35,
	35, 25, 10,  0,  0, 10, 25, 35,
	45, 30, 20, 10, 10, 20, 30, 45,
	50, 40, 30, 25, 25, 30, 40, 50,
	60, 50, 45, 35, 35, 45, 50, 60,
};

// Swaps moves by index
void ChessEngine::swap(Move moves[218], U8 a, U8 b) {
	Move buffer = moves[a];
	moves[a] = moves[b];
	moves[b] = buffer;
}

// Pushes any PV move to the front of the bound.
// Returns whether any move was pushed.
bool ChessEngine::sortPV(Move moves[218], U8 left, U8 right, U8 depth) {
	if (depth != 0) {
		for (U8 i=left; i<=right; ++i) {
			if (moves[i] == pvTable[depth-1][1]) {
				swap(moves, left, i);
				return true;
			}
		}
	}
	return false;
}

// Pushes any hash move to the front of the bound.
// Returns whether any move was pushed.
bool ChessEngine::sortHashMove(Move moves[218], U8 left, U8 right, Move hashMove) {
	for (U8 i=left; i<=right; ++i) {
		if (moves[i] == hashMove) {
			swap(moves, left, i);
			return true;
		}
	}
	return false;
}

// Pushes any killer moves to the front of the bound.
// Returns number of moves pushed.
U8 ChessEngine::sortKillerMoves(Move moves[218], U8 left, U8 right, U8 depth) {
	U8 n(0);
	for (U8 i=left; i<=right; ++i) {
		if (moves[i] == killerHeuristic[depth][!killerHeuristicNext[depth]]) {
			swap(moves, left, i);
			++left;
			++n;
			break;
		}
	}

	for (U8 i=left; i<=right; ++i) {
		if (moves[i] == killerHeuristic[depth][killerHeuristicNext[depth]]) {
			swap(moves, left, i);
			return n+1;
		}
	}

	return n;
}

// Separate captures from non-captures.
// Orders captures first.
// Returns number of capture moves.
U8 ChessEngine::separateCaptures(Move moves[218], U8 left, U8 right) {
	U8 oldLeft = left;
	while (1) {
		while (moves[left].isCapture()) {
			if (left == right) {
				break;
			}
			++left;
		}
		while (!moves[right].isCapture()) {
			if (left == right) {
				break;
			}
			--right;
		}
		if (left != right) {
			swap(moves, left, right);
		} else {
			break;
		}
	}
	return right - oldLeft;
}

// Separate promotions from other moves.
// Orders promotions first.
// Returns number of promotion moves.
U8 ChessEngine::separatePromotions(Move moves[218], U8 left, U8 right) {
	U8 oldLeft = left;
	while (left != right) {
		while (moves[left].isPromotion()) {
			if (left == right) {
				break;
			}
			++left;
		}
		while (!moves[right].isPromotion()) {
			if (left == right) {
				break;
			}
			--right;
		}
		if (left != right) {
			swap(moves, left, right);
		} else {
			break;
		}
	}
	return right - oldLeft;
}

// Separate quiet moves from non-quiet moves.
// Orders non-quiet moves first.
// Returns number of non-quiet moves.
U8 ChessEngine::separateNonQuiet(Move moves[218], U8 left, U8 right) {
	U8 oldLeft = left;
	while (left != right) {
		while (moves[left].getFlags() != Move::QUIET) {
			if (left == right) {
				break;
			}
			++left;
		}
		while (moves[right].getFlags() == Move::QUIET) {
			if (left == right) {
				break;
			}
			--right;
		}
		if (left != right) {
			swap(moves, left, right);
		} else {
			break;
		}
	}
	return right - oldLeft;
}

// Killed ordered by most valuable victim then least valuable attacker.
// (MVV/LVA).
void ChessEngine::sortMVVLVA(Move moves[218], U8 left, U8 right, ChessState* cs) {
	
	if (right - left > 1) {
		for (U8 i=left; i<=right; ++i) {
			cs->updateCapturedPiece(&moves[i]);
		}

		U8 sortLeft = left;

		for (S8 i=QUEEN; i>=PAWN; --i) {

			// Sort MVV
			for (U8 j=sortLeft; j<=right; ++j) {
				if (moves[j].getCapturedPiece() == i) {
					swap(moves, sortLeft, j);
					++sortLeft;
				}
			}

			// Sort LVA
			if (sortLeft - left > 1) {
				for (U8 j(PAWN); j<=QUEEN; ++j) {
					for (U8 k(left); k<sortLeft; ++k) {
						if (moves[k].getMovingPiece() == j) {
							swap(moves, left, k);
							++left;
						}
					}
				}
			}

			left = sortLeft;
		}
	}
}

// Sorts by distance moved towards the center.
// Greater distance put first.
void ChessEngine::sortCenter(Move moves[218], U8 left, U8 right) {
	for (S8 i=6; i>=-6; --i) {
		for (U8 j=left; j<=right; ++j) {
			if (centerBias[moves[j].getEnd()] - centerBias[moves[j].getStart()]) {
				swap(moves, right, j);
				++left;
			}
		}
	}
}

// Sorts quiet moves
void ChessEngine::sortQuiet(Move moves[218], U8 left, U8 right, U8 depth) {
	left += sortKillerMoves(moves, left, right, depth);
	if (right - left > 1) {
		sortCenter(moves, left, right);
	}
}

// Sorts non-quiet moves
void ChessEngine::sortNonQuiet(Move moves[218], U8 left, U8 right, ChessState* cs) {
	// Sort captures
	U8 n = separateCaptures(moves, left, right);
	if (n > 1) {
		sortMVVLVA(moves, left, left+n-1, cs);
	}

	// Sort promotions
	// Put queen promotions then knight promotions
	left += n;
	n = separatePromotions(moves, left, right);
	if (n > 1) {
		for (U8 j=left; j<=left+n-1; ++j) {
			if (moves[j].promotionPiece() == QUEEN) {
				swap(moves, left, j);
				++left;
			}
		}
	}
}

// General move ordering
void ChessEngine::sortMain(Move moves[218], U8 left, U8 right, ChessState* cs, U8 depth, Move hashMove) {
	if (sortPV(moves, left, right, depth)) {
		++left;
	}
	if (sortHashMove(moves, left, right, hashMove)) {
		++left;
	}

	U8 n = separateNonQuiet(moves, left, right);
	if (n > 1) {
		sortNonQuiet(moves, left, left+n-1, cs);
	}

	if (right - left > 1) {
		sortQuiet(moves, left+n, right, depth);
	}
}

// General move ordering without hash moves
void ChessEngine::sortMainNoHash(Move moves[218], U8 left, U8 right, ChessState* cs, U8 depth) {
	if (sortPV(moves, left, right, depth)) {
		++left;
	}

	U8 n = separateNonQuiet(moves, left, right);
	if (n > 1) {
		sortNonQuiet(moves, left, left+n-1, cs);
	}

	if (right - left > 1) {
		sortQuiet(moves, left+n, right, depth);
	}
}
