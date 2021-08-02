
#include <vector>
#include "board_defs.hpp"
#include "board_hash.hpp"
#include "size_defs.hpp"
#include "zobrist_values.hpp"

using namespace std;

BoardHash::BoardHash() {
	hash = 0;
};

BoardHash::BoardHash(const Bitboard pieces[2][7], const bool turn,
	const bool castlePerms[2][2], const U8 enPassant) {

	this->makeHash(pieces, turn, castlePerms, enPassant);
}

BoardHash::~BoardHash() {};

ZobristValues BoardHash::zobristValues;

// Create hash from previously generated Zobrist numbers
// using passed games state
void BoardHash::makeHash(const Bitboard pieces[2][7], const bool turn,
	const bool castlePerms[2][2], const U8 enPassant) {

	U8 pieceLocations[10];
	U8 pieceCount;
	U8 j, k;

	// Initial hash value
	hash = 0;

	// Add piece locations to hash
	for (U8 i(0); i<2; ++i) {	// Loop through colors
		for (j=0; j<6; ++j) {	// Loop through pieces
			// Location of all pieces of current type
			pieces[i][j].getPosArr(pieceLocations, &pieceCount);
			// Hash located pieces
			for (k=0; k<pieceCount; ++k) {
				hash ^= zobristValues.pieces[i][j][pieceLocations[k]];
			}
		}
	}

	// Add player turn to hash
	if (turn == WHITE) {
		hash ^= zobristValues.whiteToMove;
	}

	// Add castling permissions to hash
	if (castlePerms[WHITE][KING_SIDE]) {
		hash ^= zobristValues.castlePerms[WHITE][KING_SIDE];
	}	
	if (castlePerms[WHITE][QUEEN_SIDE]) {
		hash ^= zobristValues.castlePerms[WHITE][QUEEN_SIDE];
	}	
	if (castlePerms[BLACK][KING_SIDE]) {
		hash ^= zobristValues.castlePerms[BLACK][KING_SIDE];
	}	
	if (castlePerms[BLACK][QUEEN_SIDE]) {
		hash ^= zobristValues.castlePerms[BLACK][QUEEN_SIDE];
	}

	// Add en passant state to the hash
	if (enPassant != -1) {
		hash ^= zobristValues.enPassant[enPassant % 8];
	}
}

// The following methods update the hash without needing to completely re-hash it.

void BoardHash::updatePiece(bool color, U8 pieceType, U8 pos) {
	hash ^= zobristValues.pieces[color][pieceType][pos];
}

void BoardHash::updateTurn() {
	hash ^= zobristValues.whiteToMove;
}

void BoardHash::updateCastlePerms(bool color, bool side) {
	hash ^= zobristValues.castlePerms[color][side];
}

void BoardHash::updateEnPassant(const U8 oldPos, const U8 newPos) {
	hash ^= zobristValues.enPassant[oldPos % 8];
	hash ^= zobristValues.enPassant[newPos % 8];
}
