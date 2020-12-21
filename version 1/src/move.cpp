
#include <iostream>
#include "../include/move.hpp"

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


void Move::print() {
	cout << start << " to " << end << endl;
}
