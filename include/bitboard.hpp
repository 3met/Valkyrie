
#pragma once
#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <vector>
#include "U64.hpp"
#include "U8.hpp"

using namespace std;

class Bitboard {
private:
	const static U8 LOWEST_BIT_TABLE[256];	// Lowest significant bit for each possible byte
public:
	Bitboard();
	Bitboard(U64 b);
	~Bitboard();

	const static U8 SHOW_ORDER[64];	// Order that the positions are displayed
	const static U8 SHOW_ORDER_FLIPPED[64];
	const static U8 RANK[64];	// Map position to rank
	const static U8 FILE[64];	// Map position to file
	const static U8 RANK_POSITIONS[64][7];	// Map rank to positions
	const static U8 FILE_POSITIONS[64][7];	// Map file to positions

	U64 board;

	// Get and Set methods
	void setPos(U8 pos, bool value);
	void setPosOn(U8 pos);
	void setPosOff(U8 pos);
	bool getPos(U8 pos) const;
	// Get all positive positions in a vector
	void getPosVec(vector<U8>* v) const;
	vector<U8> getPosVec() const;
	void popPosVec(vector<U8>* v);
	vector<U8> popPosVec();
	// Filtered Get/Set
	void getRankPosVec(U8 pos, vector<U8>* v);
	void getFilePosVec(U8 pos, vector<U8>* v);
	void getPosVecCardinal(U8 pos, vector<U8>* v);
	vector<U8> getPosVecCardinal(U8 pos);
	// Other Get/Set
	U8 getFirstPos();
	vector<U8> getFirstPosVec();

	// Miscellaneous Methods
	U8 LSB();	// Lowest significant bit
	U8 popLSB();	// Pop lowest significant bit (LSB)

	// Output Methods
	void show();
};

#endif
