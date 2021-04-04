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

#include <time.h>
clock_t start_t, end_t;
double total_time = 0;

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

	start_t = clock();
	for (int i=0; i<count; ++i) {
		cout << "===== Move #" << i+1 << " =====" << endl;
		m = engine.bestMove(&cs, depth);
		cout << "Score: " << m.second << endl;
		m.first.print();
		cs.move(m.first);
		cs.show();
	}
	end_t = clock();	// TEMP
	total_time += ((double) (end_t - start_t)) / CLOCKS_PER_SEC;	// TEMP

	cout << "per_count=" << (total_time/count) << endl;
	// count = 20; depth=3
	// per_count=0.3092
	// per_count=0.3098
	// per_count=0.30835
	// per_count=0.31355
	// per_count=0.33455
	// per_count=0.3038		UPDATE change cs from obj to ref in bestMove
	// per_count=0.31035
	// per_count=0.3049
	// per_count=0.29985	UPDATE change move vector to pass by ref
	// per_count=0.2988

	return 0;
}

