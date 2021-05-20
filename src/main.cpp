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
 - For rook move generation, remove non-horizontal pieces from bitboard before calculation
 - Add engine settings class for things like material scoring system
 - Pawn structure rating lookup
 - End search function early

 - Move time function: https://www.desmos.com/calculator/v9kcbgamx3

 - Name: Coronation Chess

 - TODO: compile with "libwinpthread-1.dll"

*/

#include <string>
#include <iostream>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "testing.hpp"
#include "U64.hpp"
#include "uci.hpp"

using namespace std;

#include <time.h>	// TEMP
clock_t start_t, end_t;
double total_time = 0;

int main() {
	cout << "Chess Engine | By Emet Behrendt" << endl;

	// Bitboard size check
	if (sizeof(U64) != 8) {
		cout << "ERROR: Bitboard has incorrect number of bits" << endl;
		return 1;
	}

	ChessState cs;
	ChessEngine engine;

	UCI uci(&cs, &engine);

	uci.run();

	// cout << "Loading Engine..." << endl;

	// pair<Move, EvalScore> m;
	// char buffer[10];
	// string playerInput;
	// cout << "Engine Loading Complete" << endl << endl;

	// int count = 10;
	// int depth = 3;

	// cout << perftTest() << endl;


	// cout << "===== Initial =====" << endl;
	// cout << "Count: " << count << endl;
	// cout << "Depth: " << depth << endl;
	// cs.show();
	
	// for (int i=0; i<count; ++i) {
		// cout << endl << "===== Turn #" << i+1 << " =====" << endl;

		// start_t = clock();
		// m = engine.bestMove(&cs, depth);
		// end_t = clock();	// TEMP
		// total_time = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;	// TEMP
		
		// cout << "Calc. Time: " << total_time << endl;
		// cout << "Score: " << m.second << endl;
		// cout << "Move: " << m.first << endl;
		// cs.move(m.first);
		// cs.show();

		// while (true) {
		// 	cout << endl << "Enter move:" << endl;
		// 	cout << ">>> ";
		// 	cin >> playerInput;

		// 	if (playerInput == "reverse") {
		// 		cs.reverseMove();
		// 		cs.reverseMove();
		// 		cout << endl << "Move Reversed." << endl;
		// 		cs.show();
		// 		continue;
		// 	}

		// 	break;
		// }

		// if (playerInput == "exit") {
		// 	break;
		// }

		// Move m2 = cs.notationToMove(playerInput);

		// cs.move(m2);
	// }
	
	return 0;
}

