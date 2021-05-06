
#include <algorithm>
#include <iostream>
#include "chess_engine.hpp"
#include "move.hpp"

using namespace std;

/*	Sorts moves for alpha beta pruning
	Order:
		Captures ordered by decreasing value of piece killed
		Pawn promotions
		Leftover moves in order of 
*/

bool moveCmp(Move const &a, Move const &b) {
	if (a.killed > b.killed) {
		return true;
	} else if (b.killed > a.killed) {
		return false;
	}

	if (a.promoted > b.promoted) {
		return true;
	} else if (b.promoted > a.promoted) {
		return false;
	}

	return a.start > b.start;	// Default
}

void ChessEngine::sortMoves(vector<Move>* moves) {
	sort(moves->begin(), moves->end(), moveCmp);
}
