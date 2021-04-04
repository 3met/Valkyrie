
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

	const static U8 show_order[64];	// Order to display the bits
	U64 board;

	// Get and Set methods
	void setPos(U8 pos, bool value);
	bool getPos(U8 pos);
	vector<U8> getPosVector();
	vector<U8> getPosVector(U8 limit);

	U64 posToBoard(short pos);

	// Output Methods
	void show();
};

#endif
