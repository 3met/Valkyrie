
#include <iostream>
#include <vector>
#include "bitboard.hpp"
#include "U64.hpp"
#include "U8.hpp"

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

// Map rank to positions
const U8 Bitboard::RANK_POSITIONS[64][7] = {
	{1,	2,	3,	4,	5,	6,	7},
	{0,	2,	3,	4,	5,	6,	7},
	{0,	1,	3,	4,	5,	6,	7},
	{0,	1,	2,	4,	5,	6,	7},
	{0,	1,	2,	3,	5,	6,	7},
	{0,	1,	2,	3,	4,	6,	7},
	{0,	1,	2,	3,	4,	5,	7},
	{0,	1,	2,	3,	4,	5,	6},
	{9,	10,	11,	12,	13,	14,	15},
	{8,	10,	11,	12,	13,	14,	15},
	{8,	9,	11,	12,	13,	14,	15},
	{8,	9,	10,	12,	13,	14,	15},
	{8,	9,	10,	11,	13,	14,	15},
	{8,	9,	10,	11,	12,	14,	15},
	{8,	9,	10,	11,	12,	13,	15},
	{8,	9,	10,	11,	12,	13,	14},
	{17,	18,	19,	20,	21,	22,	23},
	{16,	18,	19,	20,	21,	22,	23},
	{16,	17,	19,	20,	21,	22,	23},
	{16,	17,	18,	20,	21,	22,	23},
	{16,	17,	18,	19,	21,	22,	23},
	{16,	17,	18,	19,	20,	22,	23},
	{16,	17,	18,	19,	20,	21,	23},
	{16,	17,	18,	19,	20,	21,	22},
	{25,	26,	27,	28,	29,	30,	31},
	{24,	26,	27,	28,	29,	30,	31},
	{24,	25,	27,	28,	29,	30,	31},
	{24,	25,	26,	28,	29,	30,	31},
	{24,	25,	26,	27,	29,	30,	31},
	{24,	25,	26,	27,	28,	30,	31},
	{24,	25,	26,	27,	28,	29,	31},
	{24,	25,	26,	27,	28,	29,	30},
	{33,	34,	35,	36,	37,	38,	39},
	{32,	34,	35,	36,	37,	38,	39},
	{32,	33,	35,	36,	37,	38,	39},
	{32,	33,	34,	36,	37,	38,	39},
	{32,	33,	34,	35,	37,	38,	39},
	{32,	33,	34,	35,	36,	38,	39},
	{32,	33,	34,	35,	36,	37,	39},
	{32,	33,	34,	35,	36,	37,	38},
	{41,	42,	43,	44,	45,	46,	47},
	{40,	42,	43,	44,	45,	46,	47},
	{40,	41,	43,	44,	45,	46,	47},
	{40,	41,	42,	44,	45,	46,	47},
	{40,	41,	42,	43,	45,	46,	47},
	{40,	41,	42,	43,	44,	46,	47},
	{40,	41,	42,	43,	44,	45,	47},
	{40,	41,	42,	43,	44,	45,	46},
	{49,	50,	51,	52,	53,	54,	55},
	{48,	50,	51,	52,	53,	54,	55},
	{48,	49,	51,	52,	53,	54,	55},
	{48,	49,	50,	52,	53,	54,	55},
	{48,	49,	50,	51,	53,	54,	55},
	{48,	49,	50,	51,	52,	54,	55},
	{48,	49,	50,	51,	52,	53,	55},
	{48,	49,	50,	51,	52,	53,	54},
	{57,	58,	59,	60,	61,	62,	63},
	{56,	58,	59,	60,	61,	62,	63},
	{56,	57,	59,	60,	61,	62,	63},
	{56,	57,	58,	60,	61,	62,	63},
	{56,	57,	58,	59,	61,	62,	63},
	{56,	57,	58,	59,	60,	62,	63},
	{56,	57,	58,	59,	60,	61,	63},
	{56,	57,	58,	59,	60,	61,	62},
};

// Map file to positions
const U8 Bitboard::FILE_POSITIONS[64][7] = {
	{8,	16,	24,	32,	40,	48,	56},
	{9,	17,	25,	33,	41,	49,	57},
	{10,	18,	26,	34,	42,	50,	58},
	{11,	19,	27,	35,	43,	51,	59},
	{12,	20,	28,	36,	44,	52,	60},
	{13,	21,	29,	37,	45,	53,	61},
	{14,	22,	30,	38,	46,	54,	62},
	{15,	23,	31,	39,	47,	55,	63},
	{0,	16,	24,	32,	40,	48,	56},
	{1,	17,	25,	33,	41,	49,	57},
	{2,	18,	26,	34,	42,	50,	58},
	{3,	19,	27,	35,	43,	51,	59},
	{4,	20,	28,	36,	44,	52,	60},
	{5,	21,	29,	37,	45,	53,	61},
	{6,	22,	30,	38,	46,	54,	62},
	{7,	23,	31,	39,	47,	55,	63},
	{0,	8,	24,	32,	40,	48,	56},
	{1,	9,	25,	33,	41,	49,	57},
	{2,	10,	26,	34,	42,	50,	58},
	{3,	11,	27,	35,	43,	51,	59},
	{4,	12,	28,	36,	44,	52,	60},
	{5,	13,	29,	37,	45,	53,	61},
	{6,	14,	30,	38,	46,	54,	62},
	{7,	15,	31,	39,	47,	55,	63},
	{0,	8,	16,	32,	40,	48,	56},
	{1,	9,	17,	33,	41,	49,	57},
	{2,	10,	18,	34,	42,	50,	58},
	{3,	11,	19,	35,	43,	51,	59},
	{4,	12,	20,	36,	44,	52,	60},
	{5,	13,	21,	37,	45,	53,	61},
	{6,	14,	22,	38,	46,	54,	62},
	{7,	15,	23,	39,	47,	55,	63},
	{0,	8,	16,	24,	40,	48,	56},
	{1,	9,	17,	25,	41,	49,	57},
	{2,	10,	18,	26,	42,	50,	58},
	{3,	11,	19,	27,	43,	51,	59},
	{4,	12,	20,	28,	44,	52,	60},
	{5,	13,	21,	29,	45,	53,	61},
	{6,	14,	22,	30,	46,	54,	62},
	{7,	15,	23,	31,	47,	55,	63},
	{0,	8,	16,	24,	32,	48,	56},
	{1,	9,	17,	25,	33,	49,	57},
	{2,	10,	18,	26,	34,	50,	58},
	{3,	11,	19,	27,	35,	51,	59},
	{4,	12,	20,	28,	36,	52,	60},
	{5,	13,	21,	29,	37,	53,	61},
	{6,	14,	22,	30,	38,	54,	62},
	{7,	15,	23,	31,	39,	55,	63},
	{0,	8,	16,	24,	32,	40,	56},
	{1,	9,	17,	25,	33,	41,	57},
	{2,	10,	18,	26,	34,	42,	58},
	{3,	11,	19,	27,	35,	43,	59},
	{4,	12,	20,	28,	36,	44,	60},
	{5,	13,	21,	29,	37,	45,	61},
	{6,	14,	22,	30,	38,	46,	62},
	{7,	15,	23,	31,	39,	47,	63},
	{0,	8,	16,	24,	32,	40,	48},
	{1,	9,	17,	25,	33,	41,	49},
	{2,	10,	18,	26,	34,	42,	50},
	{3,	11,	19,	27,	35,	43,	51},
	{4,	12,	20,	28,	36,	44,	52},
	{5,	13,	21,	29,	37,	45,	53},
	{6,	14,	22,	30,	38,	46,	54},
	{7,	15,	23,	31,	39,	47,	55},
};

// |~| ----- Get and Set Methods -----

/* Set the given position to the given value */
void Bitboard::setPos(U8 pos, bool value) {
	if (value) {
		board ^= (-U64(1) ^ board) & (1ULL << pos);
	} else {
		board ^= (-U64(0) ^ board) & (1ULL << pos);
	}
}

/* Set the given position on (1) */
void Bitboard::setPosOn(U8 pos) {
	board ^= (-U64(1) ^ board) & (1ULL << pos);
}

/* Set the given position off (0) */
void Bitboard::setPosOff(U8 pos) {
	board ^= (-U64(0) ^ board) & (1ULL << pos);
}

/* Returns the value of the bit position */
bool Bitboard::getPos(U8 pos) const {
	return board >> pos & 1;
}

/* Returns the all positions with a positive value */
void Bitboard::getPosVec(vector<U8>* v) const {
	Bitboard bb(this->board);

	while (bb.board != 0) {
		v->push_back(bb.popLSB());
	}
}

/* Returns the all positions with a positive value */
vector<U8> Bitboard::getPosVec() const {
	vector<U8> v;
	getPosVec(&v);
	return v;
}

/* Returns the all positions with a positive value 
   Resets board to zero */
void Bitboard::popPosVec(vector<U8>* v) {
	while (this->board != 0) {
		v->push_back(this->popLSB());
	}
}

/* Returns the all positions with a positive value
   Resets board to zero */
vector<U8> Bitboard::popPosVec() {
	vector<U8> v;
	popPosVec(&v);
	return v;
}


void Bitboard::getRankPosVec(U8 pos, vector<U8>* v) {
	for (U8 i=0; i<7; ++i) {
		if (this->getPos(RANK_POSITIONS[pos][i])) {
			v->push_back(RANK_POSITIONS[pos][i]);
		}
	}
}

void Bitboard::getFilePosVec(U8 pos, vector<U8>* v) {
	for (U8 i=0; i<7; ++i) {
		if (this->getPos(FILE_POSITIONS[pos][i])) {
			v->push_back(FILE_POSITIONS[pos][i]);
		}
	}
}

void Bitboard::getPosVecCardinal(U8 pos, vector<U8>* v) {
	getRankPosVec(pos, v);
	getFilePosVec(pos, v);
}

vector<U8> Bitboard::getPosVecCardinal(U8 pos) {
	vector<U8> v;
	getPosVecCardinal(pos, &v);
	return v;
}

/* Return the position of the first "true" bit */
U8 Bitboard::getFirstPos() {
	return LSB();
}

/* Return the position of the first "true" bit */
vector<U8> Bitboard::getFirstPosVec() {
	return vector<U8>(1, LSB());
}

/* Pop lowest significant bit */
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


/* Pop lowest significant bit */
inline U8 Bitboard::popLSB() {
	U8 lsb(this->LSB());
	this->board &= this->board - 1;
	return lsb;
}

// |~| ----- Output Methods -----
/* Displays Bitboard on Console */
void Bitboard::show() {
	cout << "---------------" << endl;

	for (U8 i(0); i<64; ++i) {
		cout << ((board >> SHOW_ORDER[i]) & 1) << ' ';

		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}

	cout << "---------------" << endl;
}
