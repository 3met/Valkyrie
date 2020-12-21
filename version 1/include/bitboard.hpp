
#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <vector>

using namespace std;

class Bitboard {
public:
	Bitboard();
	Bitboard(unsigned long long int b);
	~Bitboard();

	const static short show_order[64];
	unsigned long long int board;

	// Get and Set methods
	void setPos(short pos, bool value);
	bool getPos(short pos);
	vector<short> getPosVector();
	vector<short> getPosVector(short limit);

	unsigned long long int posToBoard(short pos);

	// Output Methods
	void show();
};

#endif
