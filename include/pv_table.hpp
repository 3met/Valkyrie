
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

	U16 size();	// Returns the size of the PvTable
	void clear();	// Clears data in PvTable to NULL_MOVE
	void copyNext(U16 target);			// Copies from next row
	void shiftForward(U16 shiftDist);	// Shift all moves forward by one
};

#endif
