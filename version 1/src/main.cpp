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

/*
For Consideration:

 - Instead of calculating all moves everytime, somehow reuse?
 - For rook move generation, remove non-horizontal pieces from bitboard before calculation
 - Add engine settings class for things like material scoring system

*/

#include <iostream>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "U64.hpp"

using namespace std;

void play_self(ChessState* cs, ChessEngine engine, U8 depth, U8 count) {
	pair<Move, float> m;

	for (int i=0; i<count; ++i) {
		cout << "===== NUM " << i << " =====" << endl;
		m = engine.bestMove(cs, depth);
		cout << m.second << endl;
		m.first.print();
		cs->move(m.first);
		cs->show();
	}
}

int main() {
	cout << "----- Chess Engine -----" << endl;
	cout << "By Emet Behrendt" << endl << endl;

	// Bitboard size check
	if (sizeof(U64) != 8) {
		cout << "Warning: Bitboard has incorrect number of bits" << endl;
		return 1;
	}
	
	ChessState cs;
	ChessEngine engine;
	pair<Move, float> m;
	char buffer[10];

	int count = 20;
	int depth = 3;

	cout << "===== Initial =====" << endl;
	cout << "Count: " << count << endl;
	cout << "Depth: " << depth << endl;
	cs.show();

	for (int i=0; i<count; ++i) {
		cout << "===== Move #" << i+1 << " =====" << endl;
		m = engine.bestMove(&cs, depth);
		cout << "Score: " << m.second << endl;
		m.first.print();
		cs.move(m.first);
		cs.show();
	}

	return 0;
}

