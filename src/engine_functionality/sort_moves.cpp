
#include <algorithm>
#include "chess_engine.hpp"
#include "move.hpp"
#include "move_compare.hpp"
#include "size_defs.hpp"

using namespace std;

void swap(Move moves[218], U8 a, U8 b) {
	Move buffer = moves[a];
	moves[a] = moves[b];
	moves[b] = buffer;
}

// Call std::sort on vector of moves
void ChessEngine::sortMoves(Move moves[218], ChessState* cs, U8* moveCount, U8 depth, const BaseMove* hashMove) {
	// sort(moves, moves+(*moveCount), MoveCompare(this, cs, depth, hashMove));

	U8 left = 0;
	U8 right = (*moveCount)-1;

	// PV Table Matches
	if (depth != 0) {
		for (U8 i=left; i<=right; ++i) {
			if (moves[i] == pvTable[depth-1][1]) {
				swap(moves, left, i);
				++left;
				break;
			}
		}
	}

	// Hash moves from previous calculations
	for (U8 i=left; i<=right; ++i) {
		if (moves[i] == *hashMove) {
			swap(moves, left, i);
			++left;
			break;
		}
	}

	// Seperate all kill moves and order them next
	U8 sortLeft = left;
	U8 sortRight = right;
	while (sortLeft != sortRight) {
		while (moves[sortLeft].isCapture()) {
			if (sortLeft == sortRight) {
				break;
			}
			++sortLeft;
		}
		while (!moves[sortRight].isCapture()) {
			if (sortLeft == sortRight) {
				break;
			}
			--sortRight;
		}
		if (sortLeft != sortRight) {
			swap(moves, sortLeft, sortRight);
		} else {
			break;
		}
	}

	// Sort kill moves
	sort(moves+left, moves+(*moveCount), MoveCompare(this, cs, depth, hashMove));

	left = sortLeft;

	// Killer Heuristic
	for (U8 i=left; i<=right; ++i) {
		if (moves[i] == killerHeuristic[depth][!killerHeuristicNext[depth]]) {
			swap(moves, left, i);
			++left;
			break;
		}
	}

	for (U8 i=left; i<=right; ++i) {
		if (moves[i] == killerHeuristic[depth][killerHeuristicNext[depth]]) {
			swap(moves, left, i);
			++left;
			break;
		}
	}
}
