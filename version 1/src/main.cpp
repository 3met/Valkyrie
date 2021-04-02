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
 - For rook move generation, remove non-horizontal pieces from board before calculation
 - Add engine settings class for things like material scoring system

*/

#include <iostream>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "U64.hpp"

using namespace std;

ChessState play_self(ChessState cs) {

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

	ChessState cs_buffer;
	Move move_buffer;

	cs.clear();
	
	// cs.place(cs.white, cs.knight, 26);
	// cs.place(cs.white, cs.king, 56);
	// cs.place(cs.black, cs.pawn, 46);
	// cs.place(cs.black, cs.king, 61);

	cout << "===== Initial =====" << endl;
	cs.show();

	cout << "===== One =====" << endl;
	m = engine.bestMove(cs, 2);
	cout << m.second << endl;
	cs.move(m.first);
	cs.show();

	// Show engine planning
	// cs_buffer = cs;
	// for (int i=0; i<6; ++i) {
	// 	move_buffer = engine.stateTree.get(cs_buffer);
	// 	cs_buffer.move(move_buffer);
	// 	move_buffer.print();
	// 	cs.show();
	// }

	cout << "===== Two =====" << endl;
	m = engine.bestMove(cs, 2);
	cout << m.second << endl;
	cs.move(m.first);
	cs.show();
	
	cout << "===== Three =====" << endl;
	m = engine.bestMove(cs, 2);
	cout << m.second << endl;
	cs.move(m.first);
	cs.show();

	cout << "===== Four =====" << endl;
	m = engine.bestMove(cs, 2);
	cout << m.second << endl;
	cs.move(m.first);
	cs.show();

	return 0;
}

