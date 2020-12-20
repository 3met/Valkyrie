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

	// Bitboard size check
	if (sizeof(unsigned long long int) != 8) {
		cout << "Warning: Bitboard has incorrect number of bits" << endl;
	}

	bool turn = true;
	int nums[5] = {1, 2, 3, 4, 5};

	ChessState cs;
	cs.show();

	/*
	cs.move(12, 28);
	cs.show();

	cs.move(52, 36);
	cs.show();
	*/

	ChessEngine ce;

	Move m = ce.bestMove(cs);
	cs.move(m.start, m.end);
	cout << m.start << " to " << m.end << endl;
	cs.show();
	
	m = ce.bestMove(cs);
	cs.move(m.start, m.end);
	cout << m.start << " to " << m.end << endl;
	cs.show();

	m = ce.bestMove(cs);
	cs.move(m.start, m.end);
	cout << m.start << " to " << m.end << endl;
	cs.show();

	m = ce.bestMove(cs);
	cs.move(m.start, m.end);
	cout << m.start << " to " << m.end << endl;
	cs.show();

	return 0;
}

/*
For Consideration:

 - Instead of calculating all moves everytime, somehow reuse?


*/