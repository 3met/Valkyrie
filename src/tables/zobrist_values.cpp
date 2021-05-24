
#include <random>
#include <time.h>
#include "U8.hpp"
#include "zobrist_values.hpp"

using namespace std;

ZobristValues::ZobristValues() {
	random_device rd;	// Seed
	default_random_engine generator(rd());
	// Random number distribution
	uniform_int_distribution<U64> distribution(0,0xFFFFFFFFFFFFFFFF);

	U8 i, j;

	for (i=0; i<64; ++i) {	// Loop through tiles 
		for (j=0; j<6; ++j) {	// Loop through pieces
			pieces[0][j][i] = distribution(generator);
			pieces[1][j][i] = distribution(generator);
		}
	}

	whiteToMove = distribution(generator);

	castlePerms[0][0] = distribution(generator);
	castlePerms[0][1] = distribution(generator);
	castlePerms[1][0] = distribution(generator);
	castlePerms[1][1] = distribution(generator);

	for (i=0; i<8; ++i) {
		enPassant[i] = distribution(generator);	// En Passant file
	}
}

ZobristValues::~ZobristValues() {};
