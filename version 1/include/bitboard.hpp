
#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <vector>
#include "U64.hpp"

using namespace std;

class Bitboard {
public:
	Bitboard();
	Bitboard(U64 b);
	~Bitboard();

	const static short show_order[64];	// Order to display the bits
	U64 board;

	// Get and Set methods
	void setPos(short pos, bool value);
	bool getPos(short pos);
	vector<short> getPosVector();
	vector<short> getPosVector(short limit);

	U64 posToBoard(short pos);

	// Output Methods
	void show();
};

#endif
