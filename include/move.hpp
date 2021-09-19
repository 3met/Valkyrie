
#pragma once
#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>
#include "board_defs.hpp"
#include "size_defs.hpp"

using namespace std;

// Minimum information for a move.
// Contains the start and end positions, as well as
// ep passant, promotion, and capture information.
// Supports null moves.
class BaseMove {
public:
	// First 6 bits for start position
	// Next 6 bits for end position
	// Last 4 bits for flags
	U16 data;

	// BaseMove Flags
	// Partly based on: https://www.chessprogramming.org/Encoding_BaseMoves#Information_Required
	// Based on 4 bit system.
	// First bit indicates promotions.
	// Second bit indicates captures.
	static const U8 QUIET = 0;
	static const U8 UNKNOWN_FLAG = 1;
	static const U8 KING_CASTLE = 2;
	static const U8 QUEEN_CASTLE = 3;
	static const U8 CAPTURE = 4;
	static const U8 EP_CAPTURE = 5;
	// static const U8 OTHER_CAPTURE = 6;    // For special use
	static const U8 KNIGHT_PROMO = 8;
	static const U8 BISHOP_PROMO = 9;
	static const U8 ROOK_PROMO = 10;
	static const U8 QUEEN_PROMO = 11;
	static const U8 KNIGHT_PROMO_CAPTURE = 12;
	static const U8 BISHOP_PROMO_CAPTURE = 13;
	static const U8 ROOK_PROMO_CAPTURE = 14;
	static const U8 QUEEN_PROMO_CAPTURE = 15;
	
	BaseMove();		// Creates a null move
	BaseMove(U16 _data);
	BaseMove(U8 start, U8 end, U8 flags);
	~BaseMove();

	friend ostream& operator<<(ostream& os, const BaseMove& m);
	friend bool operator==(const BaseMove& a, const BaseMove& b);

	static const BaseMove NULL_MOVE;

	static string posToCoord(const U8 pos);		// Position to coord (9 ==> b2)
	static U8 coordToPos(const string coord);	// Coord to position (b2 ==> 9)
	static U8 promotionFlag(U8 piece, bool addCapture);
	U8 promotionPiece() const;

	inline U8 getStart() const {
		return (data >> 10);
	}

	inline U8 getEnd() const {
		return ((data >> 4) & 0x3f);
	}

	inline U8 getFlags() const {
		return (data & 0xf);
	}

	inline bool isNull() const {
		return (data == 0);
	}

	inline bool isCapture() const {
		return (data & 0x4);
	}

	inline bool isPromotion() const {
		return (data & 0x8);
	}

protected:
	// Encodes move information into data.
	// Assumes move information is correct.
	inline void setData(U8 start, U8 end, U8 flags) {
		data = start;
		data <<= 6;
		data |= end;
		data <<= 4;
		data |= flags;
	}
};

// Full move with extra information.
// Extra information includes piece moving and piece killed.
class Move : public BaseMove {
public:
	// Unknown piece moving and captured
	// Equal to ((UNKNOWN_PIECE << 3) + UNKNOWN_PIECE)
	static const U8 UNKNOWN_EXTRA_DATA = 63;

	// First 2 bits are spare (always 00)
	// Next 3 bits for piece moving
	// Last 3 bits for piece captured
	// (000 to 101 for piece captured, 110 for no capture, 111 for unknown status)
	U8 extraData;

	Move();
	Move(BaseMove m);
	Move(BaseMove m, U8 piece);
	Move(BaseMove m, U8 piece, U8 captured);
	Move(U8 start, U8 end, U8 flags);
	Move(U8 start, U8 end, U8 flags, U8 piece);
	Move(U8 start, U8 end, U8 flags, U8 piece, U8 captured);
	~Move();

	static const Move NULL_MOVE;

	inline BaseMove toBaseMove() const {
		return BaseMove(data);
	}

	inline void setMovingPiece(U8 piece) {
		extraData = (extraData & 0x7) | (piece << 3);
	}

	inline void setCapturedPiece(U8 piece) {
		extraData = (extraData & 0x38) | piece;
	}

	inline U8 getMovingPiece() const {
		return (extraData >> 3);
	}

	inline U8 getCapturedPiece() const {
		return (extraData & 0x7);
	}

	Move toMove();

protected:
	inline void setExtraData(U8 movingPiece, U8 capturedPiece) {
		extraData = movingPiece;
		extraData <<= 3;
		extraData |= capturedPiece;
	}
};

#endif
