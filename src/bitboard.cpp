
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
	U64 mask = U64(1) << pos;
	board = (board & ~mask) | (U64(value) << pos);
}

// Set the given position on (ie 1)
void Bitboard::setPosOn(U8 pos) {
	board |= U64(1) << pos;
}

// Set the given position off (ie 0)
void Bitboard::setPosOff(U8 pos) {
	board &= ~(U64(1) << pos);
}

// Returns the value of the bit position
bool Bitboard::getPos(U8 pos) const {
	return (board >> pos) & 1;
}

// Stores the positions of all 1s and returns the number of 1s found
U8 Bitboard::getPosArr(U8 arr[]) const {
	Bitboard bb(this->board);

	U8 n(0);
	while (bb.board != 0) {
		arr[n] = bb.popLSB();
		++n;
	}
	return n;
}

// Stores the positions of all 1s and returns the number of 1s found
// Resets board to zero.
U8 Bitboard::popPosArr(U8 arr[]) {
	U8 n(0);
	while (this->board != 0) {
		arr[n] = this->popLSB();
		++n;
	}
	return n;
}

// Return the position of the first "true" bit
U8 Bitboard::getFirstPos() {
	return LSB();
}


// Wrapper function for LSB using intrinsics
#if defined(__GNUC__) && (defined(__x86_64__) || defined(_M_X64))
	#include <immintrin.h> // Include the header for intrinsics

	inline U8 lsb_intrinsic(U64 board) {
		//return _tzcnt_u64(board);
		return __builtin_ctzll(board);
	}
#else
	// Portable implementation for other compilers
	inline U8 lsb_portable(U64 board) {
		U8* bytes = (U8*)(board);

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
#endif

// Pop lowest significant bit using the fastest implementation
inline U8 Bitboard::LSB() {
#if defined(__GNUC__) && (defined(__x86_64__) || defined(_M_X64))
	return lsb_intrinsic(this->board);
#else
	return lsb_portable(this->board);
#endif
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
