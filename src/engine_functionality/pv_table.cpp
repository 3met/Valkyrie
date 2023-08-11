
#include <cstring>
#include "pv_table.hpp"

// Allocate memory for triangular PV Table
PvTable::PvTable(short maxDepth) {
	table = new Move* [maxDepth];

	for (short i(0); i<maxDepth; ++i) {
		table[i] = new Move [maxDepth - i]();
	}

	this->triangleSize = maxDepth;
}

// Delete to prevent memory leak
PvTable::~PvTable() {
	for (short i(0); i<triangleSize; ++i) {
		delete [] table[i];
	}

	delete [] table;
}

// Returns the size of the PV table
U16 PvTable::size() {
	return this->triangleSize;
}

// Reset table to null moves
void PvTable::clear() {
	short j;
	for (short i(0); i<triangleSize; ++i) {
		for (j=0; j<(triangleSize-i); ++j) {
			table[i][j] = Move::NULL_MOVE;
		}
	}
}

// Copies and overwrites from next line
void PvTable::copyNext(U16 target) {
	memcpy(table[target] + 1, table[target+1], (triangleSize-target-1) * sizeof(Move));
}

// Shift moves in all ply forward by a given distance
// This allows shifting the table from a previous searc
// to better align with the current search
void PvTable::shiftForward(U16 shiftDist) {
	// Loop through non-null rows
	for (U16 ply(0); ply<triangleSize && !table[ply][0].isNull(); ++ply) {
		// Loop through non-null targets to write over
		U16 moveIndex(shiftDist);
		for ( ; moveIndex<(triangleSize-ply) && !table[ply][moveIndex-shiftDist].isNull(); ++moveIndex) {
			table[ply][moveIndex-shiftDist] = table[ply][moveIndex];
		}
		// Replace last "shiftDist" moves with a NULL_MOVE
		for (U16 i(moveIndex-shiftDist); i<moveIndex; ++i) {
			table[ply][i] = Move::NULL_MOVE;
		}
	}
}
