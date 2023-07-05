
#include <iostream>
#include <string>
#include <utility>
#include "bitboard.hpp"
#include "board_defs.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

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
U8 ChessState::getPieceType(const bool color, const U8 pos) {

	for (U8 i(0); i<6; ++i) {
		if (pieces[color][i].getPos(pos)) {
			return i;
		}
	}

	return NULL_PIECE;
}

// Updates the all pieces bitboard for the passed color
void ChessState::updateAllBitboard(const bool color) {
	pieces[color][ALL_PIECES].board = pieces[color][PAWN].board;
	pieces[color][ALL_PIECES].board |= pieces[color][KNIGHT].board;
	pieces[color][ALL_PIECES].board |= pieces[color][BISHOP].board;
	pieces[color][ALL_PIECES].board |= pieces[color][ROOK].board;
	pieces[color][ALL_PIECES].board |= pieces[color][QUEEN].board;
	pieces[color][ALL_PIECES].board |= pieces[color][KING].board;
}

// Converts chess notation to Move object (a7b8q ==> Move)
Move ChessState::notationToMove(const string notation) {
	U8 start(BaseMove::coordToPos(notation.substr(0, 2)));
	U8 end(BaseMove::coordToPos(notation.substr(2, 2)));
	U8 pieceType(getPieceType(turn, start));
	U8 capturePiece(getPieceType(!turn, end));

	if (pieceType == PAWN) {
		// Check for en passant killing
		if (end == enPassantHistory[moveNumber-1]) {
			return Move(start, end, Move::EP_CAPTURE, PAWN, PAWN);
		}

		// Check for pawn promotion
		if (BOARD_RANK[end] == 0 || BOARD_RANK[end] == 7) {
			// Get promotion type
			U8 promotionType;
			if (notation.size() >= 5) {
				promotionType = charToPiece(notation[4]).second;
			} else {
				// Assume queen promotion if not specified
				promotionType = QUEEN;
			}

			// Check for capture + promotion
			if (capturePiece != NULL_PIECE) {
				return Move(start, end, Move::promotionFlag(promotionType, true), PAWN, capturePiece);
			} else {
				return Move(start, end, Move::promotionFlag(promotionType, false), PAWN, NULL_PIECE);
			}
		}

	} else if (pieceType == KING) {
		if (end == start+2)
			return Move(start, end, Move::KING_CASTLE, KING);
		if (end == start-2)
			return Move(start, end, Move::QUEEN_CASTLE, KING);
	}

	if (capturePiece != NULL_PIECE) {
		return Move(start, end, Move::CAPTURE, pieceType, capturePiece);
	} else {
		return Move(start, end, Move::QUIET, pieceType);
	}
}

// Returns color and types of piece
pair<bool, U8> ChessState::charToPiece(const char piece) {
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
			printf("ERROR: \"%c\" is an invalid piece character\n", piece);
			return make_pair(WHITE, NULL_PIECE);
	}
}

// Evaluates whether the current position should
// be treated as though it could be a zugzwang
bool ChessState::zugzwangSafe() {
	// Check if major or minor pieces on the board
	if (pieces[turn][KNIGHT].board != 0) return true;
	if (pieces[turn][BISHOP].board != 0) return true;
	if (pieces[turn][ROOK].board != 0) return true;
	if (pieces[turn][QUEEN].board != 0) return true;
	return false;
}

// Checks whether a three-move repetition draw has occured 
bool ChessState::isThreeRepetition() {
	// Count repetitions
	U8 repetitionCount(0);
	for (short i(moveNumber); i>=0; i-=2) {
		if (this->bh == hashHistory[moveNumber-i]) {
			++repetitionCount;
		}
	}
	// Check for two previous instances + current instance
	if (repetitionCount >= 2) {
		return true;
	} else {
		return false;
	}
}

// Checks whether a 50-move draw has occured
bool ChessState::is50MoveDraw() {
	return this->halfmoveClock >= 50;
}
