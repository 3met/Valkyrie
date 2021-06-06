
#pragma once
#ifndef MOVE_COMPARE_HPP
#define MOVE_COMPARE_HPP

#include "chess_engine.hpp"
#include "move.hpp"
#include "transposition_table.hpp"
#include "U8.hpp"

/* Class to compare moves for moving ording prior to
   alpha-beta pruning search. */
class MoveCompare {
private:
	ChessEngine* engine;
	U8 depth;

public:
	MoveCompare(ChessEngine* _engine, U8 _depth) {
		engine = _engine;
		depth = _depth;
		int n = 2 + 2;
	};

	bool operator()(const Move& a, const Move& b) const {
		if (a.killed > b.killed) {
			return true;
		} else if (b.killed > a.killed) {
			return false;
		}

		if (a.promoted > b.promoted) {
			return true;
		} else if (b.promoted > a.promoted) {
			return false;
		}

		for (U8 i=0; i<engine->killerHeuristic[depth].size(); ++i) {
			if (a == engine->killerHeuristic[depth][i]) {
				return true;
			} else if (b == engine->killerHeuristic[depth][i]) {
				return false;
			}
		}

		return a.start > b.start;	// Default
	}
};

#endif
