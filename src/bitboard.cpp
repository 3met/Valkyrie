
#include <iostream>
#include <vector>
#include "bitboard.hpp"
#include "board_defs.hpp"
#include "size_defs.hpp"

using namespace std;

Bitboard::Bitboard() {}

Bitboard::Bitboard(U64 b) {
	board = b;
}

Bitboard::~Bitboard() {}

// Lowest significant bit for each possible byte
const U8 Bitboard::LOWEST_BIT_TABLE[256] = {
	0, 0, 1, 0, 2, 0, 1, 0,	3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0,	3, 0, 1, 0, 2, 0, 1, 0,
	5, 0, 1, 0, 2, 0, 1, 0,	3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0,	3, 0, 1, 0, 2, 0, 1, 0,
	6, 0, 1, 0, 2, 0, 1, 0,	3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0,	3, 0, 1, 0, 2, 0, 1, 0,
	5, 0, 1, 0, 2, 0, 1, 0,	3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
};


// ----- Get and Set Methods -----

// Set the given position to the given value
void Bitboard::setPos(U8 pos, bool value) {
	if (value) {
		board ^= (-U64(1) ^ board) & (1ULL << pos);
	} else {
		board ^= (-U64(0) ^ board) & (1ULL << pos);
	}
}

// Set the given position on (1)
void Bitboard::setPosOn(U8 pos) {
	board ^= (-U64(1) ^ board) & (1ULL << pos);
}

// Set the given position off (0)
void Bitboard::setPosOff(U8 pos) {
	board ^= (-U64(0) ^ board) & (1ULL << pos);
}

// Returns the value of the bit position
bool Bitboard::getPos(U8 pos) const {
	return board >> pos & 1;
}

// Returns the all positions with a positive value
U8 Bitboard::getPosArr(U8 arr[]) const {
	Bitboard bb(this->board);

	U8 n(0);
	for ( ; bb.board != 0; ++n) {
		arr[n] = bb.popLSB();
	}
	return n;
}

// Returns the all positions with a positive value .
// Resets board to zero.
U8 Bitboard::popPosArr(U8 arr[]) {
	U8 n(0);
	for ( ; this->board != 0; ++n) {
		arr[n] = this->popLSB();
	}
	return n;
}

// Return the position of the first "true" bit
U8 Bitboard::getFirstPos() {
	return LSB();
}

// Pop lowest significant bit
inline U8 Bitboard::LSB() {
	U8* bytes = (U8*)(&this->board);

	if (bytes[0]) {
		return LOWEST_BIT_TABLE[bytes[0]];
	} else if (bytes[1]) {
		return LOWEST_BIT_TABLE[bytes[1]] + 8;
	} else if (bytes[2]) {
		return LOWEST_BIT_TABLE[bytes[2]] + 16;
	} else if (bytes[3]) {
		return LOWEST_BIT_TABLE[bytes[3]] + 24;
	} else if (bytes[4]) {
		return LOWEST_BIT_TABLE[bytes[4]] + 32;
	} else if (bytes[5]) {
		return LOWEST_BIT_TABLE[bytes[5]] + 40;
	} else if (bytes[6]) {
		return LOWEST_BIT_TABLE[bytes[6]] + 48;
	} else {
		return LOWEST_BIT_TABLE[bytes[7]] + 56;  
	}
}


// Pop lowest significant bit
inline U8 Bitboard::popLSB() {
	U8 lsb(this->LSB());
	this->board &= this->board - 1;
	return lsb;
}

// ----- Output Methods -----
// Displays Bitboard on Console
void Bitboard::show() {
	printf("---------------\n");

	for (U8 i(0); i<64; ++i) {
		printf("%d ", short((board >> SHOW_ORDER[i]) & 1));

		if ((i+1) % 8 == 0) {
			printf("\n");
		}
	}

	printf("---------------\n");
}
