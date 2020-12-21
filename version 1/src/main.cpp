/*
 *	Chess Engine
 *	Version 1
 *	By Emet Behrendt
 *	
 *	w/b ==> white/black
 *	Capital/lowercase ==> White/black
 *	
 *	P/p ==> Pawn
 *	N/n ==> Knight
 *	B/b ==> Bishop
 *	R/r ==> Rook
 *	Q/q ==> Queen
 *	K/k ==> King
 *	
 */

#include <iostream>
#include "..\include\bitboard.hpp"
#include "..\include\chess_engine.hpp"
#include "..\include\chess_state.hpp"
#include "..\include\move.hpp"

using namespace std;

int main() {
	cout << "----- Chess Engine -----" << endl;
	cout << "By Emet Behrendt" << endl << endl;

	// Bitboard size check
	if (sizeof(unsigned long long int) != 8) {
		cout << "Warning: Bitboard has incorrect number of bits" << endl;
		return 1;
	}

	ChessState cs;
	ChessEngine engine;
	pair<Move, float> m;
	cs.show();

	for (short i=0; i<50; ++i) {
		cout << "===============" << endl;
		m = engine.bestMove(cs, 6);
		cs.move(m.first);
		cout << "Move: " << m.first.start << " to " << m.first.end << endl;
		cout << "Rating: " << m.second << endl;
		cs.show();
		cout << "===============" << endl;
	}

	return 0;
}

/*
For Consideration:

 - Instead of calculating all moves everytime, somehow reuse?
 - For rook move generation, remove non-horizontal pieces from board before calculation
 - Add engine settings class for things like material scoring system

*/