
#include <string>
#include <iostream>
#include "chess_state.hpp"
#include "move.hpp"

using namespace std;

// Creates a null move
Move::Move() {
	piece = -1;
}

Move::Move(S8 p, U8 s, U8 e) {
	piece = p;
	start = s;
	end = e;
	killed = -1;	// Piece killed; else -1
	promoted = -1;	// Piece promoted to; else -1
}

Move::Move(S8 p, U8 s, U8 e, S8 k) {
	piece = p;
	start = s;
	end = e;
	killed = k;		// Piece killed; else -1
	promoted = -1;	// Piece promoted to; else -1
}

Move::Move(S8 p, U8 s, U8 e, S8 k, S8 pro) {
	piece = p;
	start = s;
	end = e;
	killed = k;		// Piece killed; else -1
	promoted = pro;	// Piece promoted to; else -1
}

Move::~Move() {}

const Move Move::NULL_MOVE = Move();

// Returns a chess board coordinate from a given position.
// Ex: 9 --> b2
string Move::posToCoord(const U8 pos) {
	char file('a');
	file += pos % 8;
	return file + to_string((pos/8) + 1);
}

// Turns a chess board coordinate to a position.
// Ex: b2 --> 9
U8 Move::coordToPos(const string coord) {
	// Returns 8 * (rank-1) + file[0-7]
	return ((coord[1] - '1') << 3) + (coord[0] - 'a');
}

// Prints a move
ostream& operator<<(ostream &os, const Move &m) {
	if (m.piece != -1) {
		os << Move::posToCoord(m.start) << Move::posToCoord(m.end);
		if (m.promoted != -1) {
			os << ChessState::piece_names[ChessState::BLACK][m.promoted];
		}
	} else {
		os << "NullMove";
	}
	return os;
}

bool operator==(const Move& a, const Move& b) { 
	if (a.start == b.start && a.end == b.end && a.promoted == b.promoted) {
		return true;
	}
	return false;
}
