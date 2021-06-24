
#include "move_compare.hpp"

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

// Operator to compare moves for move ordering
bool MoveCompare::operator()(const Move& a, const Move& b) const {
	// PV Table Matches
	if (depth > 0) {
		if (a == engine->pvTable[depth-1][1]) {
			return true;
		} else if (b == engine->pvTable[depth-1][1]) {
			return false;
		}
	}

	// Killed pieces by decreasing value
	if (a.killed > b.killed) {
		return true;
	} else if (b.killed > a.killed) {
		return false;
	}

	// Pawn promotions by decreasing value of new piece
	if (a.promoted > b.promoted) {
		return true;
	} else if (b.promoted > a.promoted) {
		return false;
	}

	// Killer heuristic moves
	for (U8 i(0); i<engine->killerHeuristic[depth].size(); ++i) {
		if (a == engine->killerHeuristic[depth][i]) {
			return true;
		} else if (b == engine->killerHeuristic[depth][i]) {
			return false;
		}
	}

	// Center bias prioritizes moves toward the center
	if (centerBias[a.end] < centerBias[b.end]) {
		return true;
	} else if (centerBias[b.end] < centerBias[a.end]) {
		return false;
	}

	return false;	// Default
}
