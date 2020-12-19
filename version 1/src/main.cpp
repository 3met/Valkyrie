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
#include "..\include\chess_engine.hpp"
#include "..\include\chess_state.hpp"
#include "..\include\bitboard.hpp"

using namespace std;

int main() {
	cout << "start" << endl;

	// Bitboard size check
	if (sizeof(unsigned long long int) != 8) {
		cout << "Warning: Bitboard has incorrect number of bits" << endl;
	}

	ChessState chessState;
	chessState.show();

	chessState.move(12, 28);
	chessState.show();

	chessState.move(52, 36);
	chessState.show();

	ChessEngine ce;

	Bitboard b;

	b.board = 50463488;
	b.show();

	return 0;
}

/*
For Consideration:

 - Instead of calculating all moves everytime, somehow reuse?


*/