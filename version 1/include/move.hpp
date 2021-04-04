
#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>
#include "U8.hpp"

using namespace std;

class Move {
public:
	Move();
	Move(short piece, short start, short end);
	Move(short piece, short start, short end, short killed);
	Move(short piece, short start, short end, short killed, short promoted);
	~Move();

	static const char positions[64];
	
	short start;
	short end;
	
	short piece;	// Piece being moved
	short killed;	// Piece killed; else -1
	short promoted;	// Piece promoted to; else -1

	static string posToCoord(U8 pos);	// Position to coord (9 ==> b2)
	void print();
};

#endif
