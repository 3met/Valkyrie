
#pragma once
#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <vector>
#include "U64.hpp"
#include "U8.hpp"

using namespace std;

class Bitboard {
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
	bool getPos(U8 pos) const;
	// Get all positive positions in a vector
	void getPosVector(vector<U8>* v) const;
	vector<U8> getPosVector() const;
	vector<U8> getPosVector(U8 limit);
	// Filtered Get/Set
	void getRankPosVec(U8 pos, vector<U8>* v);
	void getFilePosVec(U8 pos, vector<U8>* v);
	void getPosVecCardinal(U8 pos, vector<U8>* v);
	vector<U8> getPosVecCardinal(U8 pos);

	// Other Get/Set
	U8 fillPosArray(U8 arr[]);
	U8 getFirstPos();
	vector<U8> getFirstPosVec();

	U64 posToBoard(short pos);

	// Output Methods
	void show();
};

#endif
