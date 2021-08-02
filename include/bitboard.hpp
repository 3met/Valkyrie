
#pragma once
#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <vector>
#include "size_defs.hpp"

using namespace std;

// Serves as a wrapper class for an unsigned 64-bit number
class Bitboard {
private:
	const static U8 LOWEST_BIT_TABLE[256];	// Lowest significant bit for each possible byte

	// Miscellaneous Methods
	U8 LSB();		// Lowest significant bit
	U8 popLSB();	// Pop lowest significant bit (LSB)

public:
	Bitboard();
	Bitboard(U64 b);
	~Bitboard();

	U64 board;

	// Set and Get Methods
	void setPos(U8 pos, bool value);
	void setPosOn(U8 pos);
	void setPosOff(U8 pos);
	bool getPos(U8 pos) const;

	// Get all positive positions in a vector/array
	U8 getPosArr(U8 arr[]) const;
	U8 popPosArr(U8 arr[]);

	// Other Get/Set
	U8 getFirstPos();

	// Output Methods
	void show();
};

#endif
