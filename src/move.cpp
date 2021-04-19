
#include <string>
#include <iostream>
#include "move.hpp"

using namespace std;

Move::Move() {}

Move::Move(U8 p, U8 s, U8 e) {
	piece = p;
	start = s;
	end = e;
	killed = -1;	// Piece killed; else -1
	promoted = -1;	// Piece promoted to; else -1
}

Move::Move(U8 p, U8 s, U8 e, S8 k) {
	piece = p;
	start = s;
	end = e;
	killed = k;	// Piece killed; else -1
	promoted = -1;	// Piece promoted to; else -1
}

Move::Move(U8 p, U8 s, U8 e, S8 k, S8 pro) {
	piece = p;
	start = s;
	end = e;
	killed = k;	// Piece killed; else -1
	promoted = pro;	// Piece promoted to; else -1
}

Move::~Move() {}

string Move::posToCoord(const U8 pos) {
	char file = 'a';
	file += pos % 8;
	return file + to_string((pos/8) + 1);
}

U8 Move::coordToPos(const string coord) {
	// Returns 8 * (rank-1) + file[0-7]
	return (8 * (coord[1] - '1')) + (coord[0] - 'a');	
}

void Move::print() {
	cout << Move::posToCoord(start) << " to " << Move::posToCoord(end) << endl;
}
