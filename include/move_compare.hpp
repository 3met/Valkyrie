
#pragma once
#ifndef MOVE_COMPARE_HPP
#define MOVE_COMPARE_HPP

#include "move.hpp"
#include "transposition_table.hpp"

/* Class to compare moves for moving ording prior to
   alpha-beta pruning search. */
class MoveCompare {
private:
	TranspositonTable* transTable;

public:
	MoveCompare();
	MoveCompare(TranspositonTable* _transTable) { transTable = _transTable; };

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

		return a.start > b.start;	// Default
	}
};

#endif
