
#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>
#include "U8.hpp"
#include "S8.hpp"

using namespace std;

class Move {
public:
	Move();
	Move(U8 piece, U8 start, U8 end);
	Move(U8 piece, U8 start, U8 end, S8 killed);
	Move(U8 piece, U8 start, U8 end, S8 killed, S8 promoted);
	~Move();

	static const char positions[64];

	friend ostream& operator<<(ostream& os, const Move& m);
	friend bool operator==(const Move& a, const Move& b);

	U8 piece;	// Piece being moved
	U8 start;	// Start pos
	U8 end;		// End pos
	S8 killed;	// Piece killed; else -1
	S8 promoted;	// Piece promoted to; else -1

	static string posToCoord(const U8 pos);	// Position to coord (9 ==> b2)
	static U8 coordToPos(const string coord);	// Coord to position (b2 ==> 9)
	void print();
};

#endif
