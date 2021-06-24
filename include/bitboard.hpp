
#pragma once
#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <vector>
#include "U8.hpp"
#include "U64.hpp"

using namespace std;

// Serves as a wrapper class for an unsigned 64-bit number
class Bitboard {
private:
	const static U8 LOWEST_BIT_TABLE[256];	// Lowest significant bit for each possible byte

public:
	Bitboard();
	Bitboard(U64 b);
	~Bitboard();

	const static U8 SHOW_ORDER[64];				// Order that the positions are printed
	const static U8 SHOW_ORDER_FLIPPED[64];		// SHOW_ORDER flipped over x axis
	const static U8 RANK[64];					// Map position to rank
	const static U8 FILE[64];					// Map position to file

	U64 board;

	// Set and Get Methods
	void setPos(U8 pos, bool value);
	void setPosOn(U8 pos);
	void setPosOff(U8 pos);
	bool getPos(U8 pos) const;

	// Get all positive positions in a vector
	void getPosVec(vector<U8>* v) const;
	vector<U8> getPosVec() const;
	void popPosVec(vector<U8>* v);
	vector<U8> popPosVec();

	// Other Get/Set
	U8 getFirstPos();
	vector<U8> getFirstPosVec();

	// Miscellaneous Methods
	U8 LSB();		// Lowest significant bit
	U8 popLSB();	// Pop lowest significant bit (LSB)

	// Output Methods
	void show();
};

#endif
