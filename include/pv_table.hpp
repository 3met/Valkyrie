
#pragma once
#ifndef PV_TABLE_HPP
#define PV_TABLE_HPP

#include "move.hpp"

using namespace std;

/* Implementation of a triangular PV table */
class PvTable {
private:
	Move** table;
	int triangleSize;

public:
	PvTable(int maxDepth);
	~PvTable();

	inline Move* operator[](int n) {
		return table[n];
	}

	void clear();
	void copyNext(int target);
};

#endif
