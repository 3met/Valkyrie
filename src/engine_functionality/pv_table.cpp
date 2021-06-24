
#include "pv_table.hpp"

/* Allocate memory for triangular PV Table */
PvTable::PvTable(int maxDepth) {
	table = new Move* [maxDepth];

	for (int i(0); i<maxDepth; ++i) {
		table[i] = new Move [maxDepth - i]();
	}

	this->triangleSize = maxDepth;
}

/* Delete to prevent memory leak */
PvTable::~PvTable() {
	for (int i(0); i<triangleSize; ++i) {
		delete [] table[i];
	}

	delete [] table;
}

/* Reset table to null moves */
void PvTable::clear() {
	int j;
	for (int i(0); i<triangleSize; ++i) {
		for (j=0; j<(triangleSize-i); ++j) {
			table[i][j] = Move();
		}
	}
}

/* Copies and overwrites from next line */
void PvTable::copyNext(int target) {
	memcpy(table[target] + 1, table[target+1], (triangleSize-target-1) * sizeof(Move));
}
