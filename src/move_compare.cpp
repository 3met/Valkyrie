
#include "board_defs.hpp"
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

MoveCompare::MoveCompare(ChessEngine* _engine, ChessState* _cs, U8 _depth, const BaseMove* _hashMove) {
	this->engine = _engine;
	this->cs = _cs;
	this->depth = _depth;
	this->hashMove = _hashMove;
};

// Operator to compare moves for move ordering.
// Return true if A is the better move.
bool MoveCompare::operator()(Move& a, Move& b) const {
	// PV Table Matches
	if (depth != 0) {
		if (a == engine->pvTable[depth-1][1]) {
			return true;
		} else if (b == engine->pvTable[depth-1][1]) {
			return false;
		}
	}

	// Hash moves from previous calculations
	if (a == *hashMove) {
		return true;
	} else if (b == *hashMove) {
		return false;
	}

	// Killed ordered by most valuable victim then least valuable attacker
	// (MVV/LVA)
	if (a.isCapture()) {
		if (b.isCapture()) {
			cs->updateCapturedPiece(&a);
			cs->updateCapturedPiece(&b);

			// If victims were the same
			if (a.getCapturedPiece() == b.getCapturedPiece()) {
				cs->updateMovingPiece(&a);
				cs->updateMovingPiece(&b);

				// If attacker A is worth less
				if (a.getMovingPiece() < b.getMovingPiece()) {
					return true;

				// If attacker B is worth less
				} else if (a.getMovingPiece() != b.getMovingPiece()) {
					return false;
				}

			// If A has a more valuable victim
			} else if (a.getCapturedPiece() > b.getCapturedPiece()) {
				return true;
			
			// If B has a more valuable victim
			} else {
				return false;
			}
		} else {
			// If only A is a kill
			return true;
		}

	} else if (b.isCapture()) {
		// If only B is a kill
		return false;
	}

	// Pawn promotions by decreasing value of new piece
	if (a.isPromotion() && a.promotionPiece() == QUEEN) {
		if (b.isPromotion() && b.promotionPiece() == QUEEN) {
			return false;
		} else {
			return true;
		}
	} else if (b.isPromotion() && b.promotionPiece() == QUEEN) {
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
	if (centerBias[a.getEnd()] < centerBias[b.getEnd()]) {
		return true;
	} else if (centerBias[a.getEnd()] != centerBias[b.getEnd()]) {
		return false;
	}

	return false;	// Default
}
