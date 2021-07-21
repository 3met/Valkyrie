
#include <iostream>
#include <vector>
#include "bitboard.hpp"
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

// Order that the positions are displayed
const U8 Bitboard::SHOW_ORDER[64] = {
	56,	57,	58,	59,	60,	61,	62,	63,
	48,	49,	50,	51,	52,	53,	54,	55,
	40,	41,	42,	43,	44,	45,	46,	47,
	32,	33,	34,	35,	36,	37,	38,	39,
	24,	25,	26,	27,	28,	29,	30,	31,
	16,	17,	18,	19,	20,	21,	22,	23,
	8,	9,	10,	11,	12,	13,	14,	15,
	0,	1,	2,	3,	4,	5,	6,	7,
};
const U8 Bitboard::SHOW_ORDER_FLIPPED[64] = {
	0,	1,	2,	3,	4,	5,	6,	7,
	8,	9,	10,	11,	12,	13,	14,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31,
	32,	33,	34,	35,	36,	37,	38,	39,
	40,	41,	42,	43,	44,	45,	46,	47,
	48,	49,	50,	51,	52,	53,	54,	55,
	56,	57,	58,	59,	60,	61,	62,	63,
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
void Bitboard::getPosArr(U8 arr[], U8* n) const {
	Bitboard bb(this->board);

	for (*n=0; bb.board != 0; ++(*n)) {
		arr[*n] = bb.popLSB();
	}
}

// Returns the all positions with a positive value .
// Resets board to zero.
void Bitboard::popPosArr(U8 arr[], U8* n) {
	for (*n=0; this->board != 0; ++(*n)) {
		arr[*n] = this->popLSB();
	}
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
