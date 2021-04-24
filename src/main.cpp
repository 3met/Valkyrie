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
 - Sort moves by likelyhood it is best move in alpha beta pruning
 - Instead of calculating all moves everytime, somehow reuse?
 - For rook move generation, remove non-horizontal pieces from bitboard before calculation
 - Add engine settings class for things like material scoring system
 - Pawn structure lookup

*/

#include <string>
#include <iostream>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "U64.hpp"

using namespace std;

#include <time.h>	// TEMP
clock_t start_t, end_t;
double total_time = 0;

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
	pair<Move, EvalScore> m;
	char buffer[10];
	string playerInput;

	int count = 30;
	int depth = 4;

	cs.loadFEN("8/2k5/8/7K/8/8/4R3/3R4 w - - 0 1");

	cout << "===== Initial =====" << endl;
	cout << "Count: " << count << endl;
	cout << "Depth: " << depth << endl;
	cs.show();
	
	for (int i=0; i<count; ++i) {
		cout << endl << "===== Move #" << i+1 << " =====" << endl;

		start_t = clock();
		m = engine.bestMove(&cs, depth);
		end_t = clock();	// TEMP
		total_time = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;	// TEMP
		
		cout << "Processing Time: " << total_time << endl;
		cout << "Score: " << m.second << endl;
		m.first.print();
		cs.move(m.first);
		cs.show();

		cout << endl << "Enter move:" << endl;
		cout << ">>> ";
		cin >> playerInput;
		Move m2 = cs.notationToMove(playerInput);

		cs.move(m2);
	}
	
	return 0;
}

