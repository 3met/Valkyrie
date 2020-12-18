
#include <iostream>
#include "bitboard.hpp"

using namespace std;

Bitboard::Bitboard() {
	board = 0;
}

Bitboard::~Bitboard() {};

const short Bitboard::show_order[64] = {
	56, 57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 2, 3, 4, 5, 6, 7,
};

void Bitboard::show() {
	/* Displays Bitboard on Console */

	// Start at top left of board
	for (int i=0; i<64; ++i) {
		cout << ((board >> show_order[i]) & 1) << ' ';

		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}
}

void Bitboard::setPos(bool value, short pos) {
	/* Set the given position to the given value */

	unsigned long long int n;	// Needs to be same size as board

	if (value) {
		n = 1;
	} else {
		n = 0;
	}

	board |= n << pos;
}
