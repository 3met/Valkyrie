
#include <utility>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "S8.hpp"
#include "U8.hpp"

// Set position increments
const S8 ChessEngine::CARDINAL_POS_INC[4] = {
	8, 1, -8, -1,
};
const S8 ChessEngine::DIAGONAL_POS_INC[4] = {
	9, -7, -9, 7,
};

/* Returns the color and position of the next piece
   encountered using ray movement.
   Returns piece as -1 nothing is found. */

pair<bool, S8> ChessEngine::nextPieceCardinal(ChessState* cs, U8 pos, U8 direction) {

	U8 termLoc;	// Terminating rank or file
	const U8* termArr;

	// Set terminating rank/file
	if (direction == UP || direction == RIGHT) {
		termLoc = 7;
	} else {
		termLoc = 0;
	}

	if (direction == UP || direction == DOWN) {
		termArr = Bitboard::RANK;
	} else {
		termArr = Bitboard::FILE;
	}

	while (termArr[pos] != termLoc) {	// Not terminating rank/file
		pos += CARDINAL_POS_INC[direction];
		// If an allied piece at target position
		if (cs->pieces[cs->turn][cs->ALL_PIECES].getPos(pos)) {
			return make_pair(cs->turn, pos);
		// If an enemy piece at target position
		} else if (cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(pos)) {
			return make_pair(!cs->turn, pos);
		}
	}

	return make_pair(false, -1);
}

pair<bool, S8> ChessEngine::nextPieceDiagonal(ChessState* cs, U8 pos, U8 direction) {

	U8 termRank;	// Terminating rank
	U8 termFile;	// Terminating file

	if (direction == UP_RIGHT || direction == UP_LEFT) {
		termRank = 7;
	} else {
		termRank = 0;
	}

	if (direction == UP_RIGHT || direction == DOWN_RIGHT) {
		termFile = 7;
	} else {
		termFile = 0;
	}

	while (Bitboard::RANK[pos] != termRank && Bitboard::FILE[pos] != termFile) {	// Not top row or right col
		pos += DIAGONAL_POS_INC[direction];
		// If an allied piece at target position
		if (cs->pieces[cs->turn][cs->ALL_PIECES].getPos(pos)) {
			return make_pair(cs->turn, pos);
		// If an enemy piece at target position
		} else if (cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(pos)) {
			return make_pair(!cs->turn, pos);
		}
	}

	return make_pair(false, -1);
}
