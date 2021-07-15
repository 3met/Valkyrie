
#include "chess_state.hpp"
#include "move_compare.hpp"

// Shortcut
#define CS ChessState

using namespace std;

// Ranks by distance from center
const U8 MoveCompare::centerBias[64] {
	8, 7, 6, 5, 5, 6, 7, 8,
	7, 5, 4, 3, 3, 4, 5, 7,
	6, 4, 2, 1, 1, 2, 4, 6,
	5, 3, 1, 0, 0, 1, 3, 5,
	5, 3, 1, 0, 0, 1, 3, 5,
	6, 4, 2, 1, 1, 2, 4, 6,
	7, 5, 4, 3, 3, 4, 5, 7,
	8, 7, 6, 5, 5, 6, 7, 8,
};

MoveCompare::MoveCompare(ChessEngine* _engine, U8 _depth) {
	this->engine = _engine;
	this->depth = _depth;
};

// Operator to compare moves for move ordering.
// Return true if A is the better move.
bool MoveCompare::operator()(const Move& a, const Move& b) const {
	// PV Table Matches
	if (depth != 0) {
		if (a == engine->pvTable[depth-1][1]) {
			return true;
		} else if (b == engine->pvTable[depth-1][1]) {
			return false;
		}
	}

	// Killed ordered by most valuable victim then least valuable attacker
	// (MVV/LVA)
	if (a.killed != -1) {
		if (b.killed != -1) {
			// If victims were the same
			if (a.killed == b.killed) {
				// If attacker A is worth less
				if (a.piece < b.piece) {
					return true;

				// If attacker B is worth less
				} else if (a.piece != b.piece) {
					return false;
				}

			// If A has a more valuable victim
			} else if (a.killed > b.killed) {
				return true;
			
			// If B has a more valuable victim
			} else {
				return false;
			}
		} else {
			// If only A is a kill
			return true;
		}

	} else if (b.killed != -1) {
		// If only B is a kill
		return false;
	}

	// Pawn promotions by decreasing value of new piece
	if (a.promoted == CS::QUEEN) {
		if (b.promoted == CS::QUEEN) {
			return false;
		} else {
			return true;
		}
	} else if (b.promoted == CS::QUEEN) {
		return false;
	}

	// Evaluate Killer Heuristic
	if (a == engine->killerHeuristic[depth][!engine->killerHeuristicNext[depth]]) {
		return true;
	} else if (b == engine->killerHeuristic[depth][!engine->killerHeuristicNext[depth]]) {
		return false;
	}

	// Second Killer Heuristic
	if (a == engine->killerHeuristic[depth][engine->killerHeuristicNext[depth]]) {
		return true;
	} else if (b == engine->killerHeuristic[depth][engine->killerHeuristicNext[depth]]) {
		return false;
	}

	// Center bias prioritizes moves toward the center
	if (centerBias[a.end] < centerBias[b.end]) {
		return true;
	} else if (centerBias[a.end] != centerBias[b.end]) {
		return false;
	}

	return false;	// Default
}
