
#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>
#include "U8.hpp"

using namespace std;

class Move {
public:
	Move();
	Move(U8 piece, U8 start, U8 end);
	Move(U8 piece, U8 start, U8 end, short killed);
	Move(U8 piece, U8 start, U8 end, short killed, short promoted);
	~Move();

	static const char positions[64];
	
	U8 start;
	U8 end;
	
	U8 piece;	// Piece being moved
	short killed;	// Piece killed; else -1
	short promoted;	// Piece promoted to; else -1

	static string posToCoord(U8 pos);	// Position to coord (9 ==> b2)
	void print();
};

#endif
