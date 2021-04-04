
#include <string>
#include <iostream>
#include "move.hpp"

using namespace std;

Move::Move() {}

Move::Move(short p, short s, short e) {
	piece = p;
	start = s;
	end = e;

	killed = -1;	// Piece killed; else -1
	promoted = -1;	// Piece promoted to; else -1
}

Move::Move(short p, short s, short e, short k) {
	piece = p;
	start = s;
	end = e;

	killed = k;	// Piece killed; else -1
	promoted = -1;	// Piece promoted to; else -1
}

Move::~Move() {}

string Move::posToCoord(U8 pos) {
	char col = 'a';
	col += pos % 8;
	return col + to_string((pos/8) + 1);
}

void Move::print() {
	cout << Move::posToCoord(start) << " to " << Move::posToCoord(end) << endl;
}
