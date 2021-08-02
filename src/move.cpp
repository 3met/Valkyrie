
#include <string>
#include <iostream>
#include "board_defs.hpp"
#include "size_defs.hpp"
#include "move.hpp"

using namespace std;

// -----------------------
// Base Move Functionality
// -----------------------

// Creates a null BaseMove
BaseMove::BaseMove() {
	data = 0;
}

BaseMove::BaseMove(U16 _data) {
	data = _data;
}

BaseMove::BaseMove(U8 start, U8 end, U8 flags) {
	this->setData(start, end, flags);
}

BaseMove::~BaseMove() {}

const BaseMove BaseMove::NULL_MOVE = BaseMove();

// Returns a chess board coordinate from a given position.
// Ex: 9 --> b2
string BaseMove::posToCoord(const U8 pos) {
	char file('a');
	file += pos % 8;
	return file + to_string((pos/8) + 1);
}

// Turns a chess board coordinate to a position.
// Ex: b2 --> 9
U8 BaseMove::coordToPos(const string coord) {
	// Returns 8 * (rank-1) + file[0-7]
	return ((coord[1] - '1') << 3) + (coord[0] - 'a');
}

// Prints a move
ostream& operator<<(ostream &os, const BaseMove &m) {
	if (m.isNull()) {
		os << "NullBaseMove";
	} else {
		os << BaseMove::posToCoord(m.getStart()) << BaseMove::posToCoord(m.getEnd());
		if (m.isPromotion()) {
			os << PIECE_NAMES[BLACK][m.promotionPiece()];
		}
	}
	return os;
}

bool operator==(const BaseMove& a, const BaseMove& b) { 
	return (a.data == b.data);
}

// Returns the promotion piece associated with the move flag.
// Assumes there is a promotion value set.
U8 BaseMove::promotionPiece() const {
	switch (getFlags()) {
		case QUEEN_PROMO:
		case QUEEN_PROMO_CAPTURE:
			return QUEEN;
		case KNIGHT_PROMO:
		case KNIGHT_PROMO_CAPTURE:
			return KNIGHT;
		case ROOK_PROMO:
		case ROOK_PROMO_CAPTURE:
			return ROOK;
		case BISHOP_PROMO:
		case BISHOP_PROMO_CAPTURE:
			return BISHOP;
		default:
			return NULL_PIECE;
	}
}

// Assumes there is a promotion value set
U8 BaseMove::promotionFlag(U8 piece, bool addCapture) {
	if (addCapture) {
		switch (piece) {
			case QUEEN:
				return QUEEN_PROMO_CAPTURE;
			case KNIGHT:
				return KNIGHT_PROMO_CAPTURE;
			case ROOK:
				return ROOK_PROMO_CAPTURE;
			case BISHOP:
				return BISHOP_PROMO_CAPTURE;
		}
	} else {
		switch (piece) {
			case QUEEN:
				return QUEEN_PROMO;
			case KNIGHT:
				return KNIGHT_PROMO;
			case ROOK:
				return ROOK_PROMO;
			case BISHOP:
				return BISHOP_PROMO;
		}
	}
}


// ------------------
// Move Functionality
// ------------------

const Move Move::NULL_MOVE = Move();

// Creates a null move
Move::Move() {
	data = 0;
	extraData = UNKNOWN_EXTRA_DATA;
}

// Make Move from a BaseMove
Move::Move(BaseMove m) {
	data = m.data;
	extraData = UNKNOWN_EXTRA_DATA;
}

Move::Move(BaseMove m, U8 piece) {
	data = m.data;
	setExtraData(piece, UNKNOWN_PIECE);
}

Move::Move(BaseMove m, U8 piece, U8 captured) {
	data = m.data;
	setExtraData(piece, captured);
}

Move::Move(U8 start, U8 end, U8 flags) {
	setData(start, end, flags);
	extraData = UNKNOWN_EXTRA_DATA;
}

Move::Move(U8 start, U8 end, U8 flags, U8 piece) {
	setData(start, end, flags);
	setExtraData(piece, UNKNOWN_PIECE);
}

Move::Move(U8 start, U8 end, U8 flags, U8 piece, U8 captured) {
	setData(start, end, flags);
	setExtraData(piece, captured);
}

Move::~Move() {}

Move Move::toMove() {
	return Move(data);
}
