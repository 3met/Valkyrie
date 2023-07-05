
#pragma once
#ifndef BOARD_DEFS_HPP
#define BOARD_DEFS_HPP

#include "size_defs.hpp"

// Board colours
const U8 WHITE = 0;
const U8 BLACK = 1;

// Piece Information
const U8 PAWN = 0;
const U8 KNIGHT = 1;
const U8 BISHOP = 2;
const U8 ROOK = 3;
const U8 QUEEN = 4;
const U8 KING = 5;
const U8 ALL_PIECES = 6;       // Used for indexing "all" bitboard
const U8 NULL_PIECE = 6;
const U8 UNKNOWN_PIECE = 7;    // Placeholder for when piece type in unknown

// Castling
const U8 KING_SIDE = 0;
const U8 QUEEN_SIDE = 1;

// Map position to rank
const U8 BOARD_RANK[64] = {
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
const U8 BOARD_FILE[64] = {
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
};

// Map position to positive-sloped diagonal
const U8 POSITIVE_DIAG[64] = {
	0,	1,	2,	3,	4,	5,	6,	7,
	1,	2,	3,	4,	5,	6,	7,	8,
	2,	3,	4,	5,	6,	7,	8,	9,
	3,	4,	5,	6,	7,	8,	9,	10,
	4,	5,	6,	7,	8,	9,	10,	11,
	5,	6,	7,	8,	9,	10,	11,	12,
	6,	7,	8,	9,	10,	11,	12,	13,
	7,	8,	9,	10,	11,	12,	13,	14,
};
// Map position to negative-sloped diagonal
const U8 NEGATIVE_DIAG[64] = {
	22,	23,	24,	25,	26,	27,	28,	29,
	21,	22,	23,	24,	25,	26,	27,	28,
	20,	21,	22,	23,	24,	25,	26,	27,
	19,	20,	21,	22,	23,	24,	25,	26,
	18,	19,	20,	21,	22,	23,	24,	25,
	17,	18,	19,	20,	21,	22,	23,	24,
	16,	17,	18,	19,	20,	21,	22,	23,
	15,	16,	17,	18,	19,	20,	21,	22,
};

// Order in which the positions are displayed
const U8 SHOW_ORDER[64] = {
	56,	57,	58,	59,	60,	61,	62,	63,
	48,	49,	50,	51,	52,	53,	54,	55,
	40,	41,	42,	43,	44,	45,	46,	47,
	32,	33,	34,	35,	36,	37,	38,	39,
	24,	25,	26,	27,	28,	29,	30,	31,
	16,	17,	18,	19,	20,	21,	22,	23,
	8,	9,	10,	11,	12,	13,	14,	15,
	0,	1,	2,	3,	4,	5,	6,	7,
};
const U8 SHOW_ORDER_FLIPPED[64] = {
	0,	1,	2,	3,	4,	5,	6,	7,
	8,	9,	10,	11,	12,	13,	14,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31,
	32,	33,	34,	35,	36,	37,	38,	39,
	40,	41,	42,	43,	44,	45,	46,	47,
	48,	49,	50,	51,	52,	53,	54,	55,
	56,	57,	58,	59,	60,	61,	62,	63,
};

// Names of the piece for printing
const char PIECE_NAMES[2][6] = {    // Note: must match piece indexing
	{'P', 'N', 'B', 'R', 'Q', 'K'},
	{'p', 'n', 'b', 'r', 'q', 'k'},
};

#endif
