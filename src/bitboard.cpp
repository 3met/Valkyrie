
#include <iostream>
#include <vector>
#include "bitboard.hpp"
#include "U64.hpp"
#include "U8.hpp"

using namespace std;

Bitboard::Bitboard() {
	board = 0;
}

Bitboard::Bitboard(U64 b) {
	board = b;
}

Bitboard::~Bitboard() {};

// Order that the positions are displayed
const U8 Bitboard::SHOW_ORDER[64] = {
	56,	57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	8,	9,	10, 11, 12, 13, 14, 15,
	0,	1,	2,	3,	4,	5,	6,	7,
};

// Map position to rank
const U8 Bitboard::RANK[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7,
};

// Map position to file
const U8 Bitboard::FILE[64] = {
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
};

// |~| ----- Get and Set Methods -----
void Bitboard::setPos(U8 pos, bool value) {
	/* Set the given position to the given value */

	U64 n;	// Needs to be same size as board

	if (value) {
		n = 1;
	} else {
		n = 0;
	}
	board ^= (-n ^ board) & (1ULL << pos);
}

/* Returns the value of the bit position */
inline bool Bitboard::getPos(U8 pos) {
	return (board >> pos) & 1;
}

/* Returns the all positions with a positive value */
vector<U8> Bitboard::getPosVector() {
	vector<U8> v;

	for (U8 i=0; i<64; ++i) {
		if (this->getPos(i)) {
			v.push_back(i);
		}
	}

	return v;
}

/* Returns the all positions with a positive value */
vector<U8> Bitboard::getPosVector(U8 limit) {
	vector<U8> v;

	for (U8 i=0; i<64; ++i) {
		if (this->getPos(i)) {
			v.push_back(i);

			if (v.size() == limit) {
				break;
			}
		}
	}

	return v;
}

/* Return the position of the first "true" bit */
U8 Bitboard::getFirstPos() {
	for (U8 i=0; i<64; ++i) {
		if (this->getPos(i)) {
			return i;
		}
	}

	cout << "Warning: No bit found in \"getFirstPos()\"" << endl;
	return 0;
}

// |~| ----- Output Methods -----
void Bitboard::show() {
	/* Displays Bitboard on Console */

	for (U8 i=0; i<64; ++i) {
		cout << ((board >> SHOW_ORDER[i]) & 1) << ' ';

		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}
}
