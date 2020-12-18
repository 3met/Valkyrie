
#ifndef BITBOARD_H
#define BITBOARD_H

class Bitboard {
public:
	Bitboard();
	~Bitboard();
	const static short show_order[64];
	unsigned long long int board;
	void show();
	void setPos(bool value, short pos);
};

#endif
