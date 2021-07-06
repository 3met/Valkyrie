
#pragma once
#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>
#include "size_defs.hpp"

using namespace std;

// Defines a chess move.
// Contains the piece moving, start and end positions,
// any piece killed, and any pawn promotions.
// Also supports null moves.
class Move {
public:
	Move();		// Creates a null move
	Move(S8 piece, U8 start, U8 end);
	Move(S8 piece, U8 start, U8 end, S8 killed);
	Move(S8 piece, U8 start, U8 end, S8 killed, S8 promoted);
	~Move();

	friend ostream& operator<<(ostream& os, const Move& m);
	friend bool operator==(const Move& a, const Move& b);

	S8 piece;		// Piece being moved; -1 for null move
	U8 start;		// Start pos
	U8 end;			// End pos
	S8 killed;		// Piece killed; else -1
	S8 promoted;	// Piece promoted to; else -1

	static string posToCoord(const U8 pos);		// Position to coord (9 ==> b2)
	static U8 coordToPos(const string coord);	// Coord to position (b2 ==> 9)
	
	inline bool isNull() {
		return (piece == -1);
	}
	void print();
};

#endif
