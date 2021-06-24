
#pragma once
#ifndef PV_TABLE_HPP
#define PV_TABLE_HPP

#include "move.hpp"

using namespace std;

// Implementation of a triangular PV table.
// Stores triangular 2D-array of moves lines.
// Used to store principal variation in alpha-beta search.
class PvTable {
private:
	Move** table;
	short triangleSize;

public:
	PvTable(short maxDepth);
	~PvTable();

	inline Move* operator[](short n) {
		return table[n];
	}

	void clear();
	void copyNext(short target);
};

#endif
