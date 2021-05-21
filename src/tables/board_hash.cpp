
#include <vector>
#include "board_hash.hpp"
#include "U8.hpp"
#include "zobrist_values.hpp"

using namespace std;

BoardHash::BoardHash() {
	hash = 0;
};

BoardHash::BoardHash(const ChessState* cs) {
	this->makeHash(cs);
}

BoardHash::~BoardHash() {};

ZobristValues BoardHash::zobristValues;

/* Create Zobrist hash from previously generated numbers
 * using passed games state */
void BoardHash::makeHash(const ChessState* cs) {
	vector<U8> pieceLocations;
	U8 i, j, k;

	// Initial hash value
	hash = 0;

	// Add piece locations to hash
	for (i=0; i<2; ++i) {	// Loop through colors
		for (j=0; j<6; ++j) {	// Loop through pieces
			// Location of all pieces of current type
			pieceLocations = cs->pieces[i][j].getPosVector();
			// Hash located pieces
			for (k=0; k<pieceLocations.size(); ++k) {
				hash ^= zobristValues.pieces[i][j][pieceLocations[k]];
			}
		}
	}

	// Add player turn to hash
	if (cs->turn == cs->WHITE) {
		hash ^= zobristValues.whiteToMove;
	}

	// Add castling permissions to hash
	if (cs->castlePerms[cs->WHITE][cs->KING_SIDE]) {
		hash ^= zobristValues.castlePerms[cs->WHITE][cs->KING_SIDE];
	}	
	if (cs->castlePerms[cs->WHITE][cs->QUEEN_SIDE]) {
		hash ^= zobristValues.castlePerms[cs->WHITE][cs->QUEEN_SIDE];
	}	
	if (cs->castlePerms[cs->BLACK][cs->KING_SIDE]) {
		hash ^= zobristValues.castlePerms[cs->BLACK][cs->KING_SIDE];
	}	
	if (cs->castlePerms[cs->BLACK][cs->QUEEN_SIDE]) {
		hash ^= zobristValues.castlePerms[cs->BLACK][cs->QUEEN_SIDE];
	}

	// Add en passant state to the hash
	if (cs->enPassant != -1) {
		hash ^= zobristValues.enPassant[cs->enPassant % 8];
	}
}