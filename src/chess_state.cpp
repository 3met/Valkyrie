
#include <iostream>
#include <string>
#include <utility>
#include "chess_state.hpp"
#include "size_defs.hpp"

const char ChessState::piece_names[2][6] = {	// Note: must match piece indexing
	{'P', 'N', 'B', 'R', 'Q', 'K'},
	{'p', 'n', 'b', 'r', 'q', 'k'},
};

const U8 ChessState::KING_START[2] = {4, 60};	// {white, black}

const U8 ChessState::ROOK_START[2][2] = {{7, 0}, {63, 56}};	// {{WK, WQ}, {BK, BQ}}

ChessState::ChessState() {
	this->reset();
};

ChessState::ChessState(string FEN) {
	this->loadFEN(FEN);
};

ChessState::~ChessState() {};

// Returns most recent move
Move ChessState::lastMove() {
	return moveHistory[moveNumber];
}

// Returns the type of piece at the given position
S8 ChessState::getPieceType(bool color, U8 pos) {

	for (U8 i(0); i<6; ++i) {
		if (pieces[color][i].getPos(pos)) {
			return i;
		}
	}

	return -1;
}

// Updates the all pieces bitboard for the passed color
void ChessState::updateAllBitboard(bool color) {
	pieces[color][ALL_PIECES].board = pieces[color][PAWN].board;
	pieces[color][ALL_PIECES].board |= pieces[color][KNIGHT].board;
	pieces[color][ALL_PIECES].board |= pieces[color][BISHOP].board;
	pieces[color][ALL_PIECES].board |= pieces[color][ROOK].board;
	pieces[color][ALL_PIECES].board |= pieces[color][QUEEN].board;
	pieces[color][ALL_PIECES].board |= pieces[color][KING].board;
}

// Converts chess notation to Move object (a7b8q ==> Move)
Move ChessState::notationToMove(string notation) {
	U8 start(Move::coordToPos(notation.substr(0, 2)));
	U8 end(Move::coordToPos(notation.substr(2, 2)));
	U8 pieceType(getPieceType(turn, start));

	U8 killed;
	// Check for en passant killing
	if (end == enPassantHistory[moveNumber-1] && pieceType == PAWN) {
		killed = PAWN;
	} else {
		killed = getPieceType(!turn, end);
	}
	S8 promoted;
	// Check for pawn promotion
	if (pieceType == PAWN && (end >= 56 || end <= 7)) {
		if (notation.size() >= 5) {
			promoted = charToPiece(notation[4]).second;
		} else {
			promoted = QUEEN;
		}
	} else {
		promoted = -1;
	}
	
	return Move(pieceType,
		start,
		end,
		killed,
		promoted);
}

// Returns color and types of piece
pair<bool, U8> ChessState::charToPiece(char piece) {
	switch (piece) {
		case 'P':
			return make_pair(WHITE, PAWN);
		case 'p':
			return make_pair(BLACK, PAWN);
		case 'N':
			return make_pair(WHITE, KNIGHT);
		case 'n':
			return make_pair(BLACK, KNIGHT);
		case 'B':
			return make_pair(WHITE, BISHOP);
		case 'b':
			return make_pair(BLACK, BISHOP);
		case 'R':
			return make_pair(WHITE, ROOK);
		case 'r':
			return make_pair(BLACK, ROOK);
		case 'Q':
			return make_pair(WHITE, QUEEN);
		case 'q':
			return make_pair(BLACK, QUEEN);
		case 'K':
			return make_pair(WHITE, KING);
		case 'k':
			return make_pair(BLACK, KING);
		default:
			cout << "ERROR: \"" << piece << "\" is an invalid character" << endl;
			return make_pair(WHITE, -1);
	}
}
