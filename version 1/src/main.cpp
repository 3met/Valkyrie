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
	char buffer[10];

	
	cs.clear();
	cs.place(cs.white, cs.knight, 26);
	cs.place(cs.white, cs.king, 56);
	cs.place(cs.black, cs.pawn, 46);
	cs.place(cs.black, cs.king, 61);
	cs.show();

	cout << "===== One =====" << endl;
	cs.pieces[cs.turn][cs.all_pieces]->show();
	cout << endl;
	cs.pieces[!cs.turn][cs.all_pieces]->show();

	cout << "===== Two =====" << endl;
	cs.move(Move(cs.knight, 26, 36));
	cs.pieces[cs.turn][cs.all_pieces]->show();
	cout << endl;
	cs.pieces[!cs.turn][cs.all_pieces]->show();

	cout << "===== Three =====" << endl;
	cs.move(Move(cs.king, 61, 52));
	cs.pieces[cs.turn][cs.all_pieces]->show();
	cout << endl;
	cs.pieces[!cs.turn][cs.all_pieces]->show();
	
	cout << endl;
	cs.move(Move(cs.knight, 36, 26));
	engine.genKMoves(cs);

	/*
	cs.clear();
	cs.place(cs.white, cs.knight, 26);
	cs.place(cs.white, cs.king, 56);
	cs.place(cs.black, cs.pawn, 46);
	cs.place(cs.black, cs.king, 61);
	cs.show();

	m = engine.bestMove(cs, 3);
	cs.move(m.first);
	cs.show();
	cin >> buffer;

	m = engine.bestMove(cs, 3);
	cs.move(m.first);
	cs.show();
	cin >> buffer;

	m = engine.bestMove(cs, 3);
	cs.move(m.first);
	cs.show();
	cin >> buffer;

	m = engine.bestMove(cs, 3);
	cs.move(m.first);
	cs.show();
	*/


	/*
	cs.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
	cs.show();

	cs.loadFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1")
	cs.show();

	cs.loadFEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2")
	cs.show();

	cs.loadFEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2")
	cs.show();
	*/

	/*
	cs.move(Move(cs.king, 4, 3));
	cs.move(Move(cs.knight, 57, 40));
	cs.move(Move(cs.knight, 6, 23));
	cs.move(Move(cs.knight, 40, 25));
	cs.move(Move(cs.knight, 23, 38));
	cs.move(Move(cs.knight, 25, 8, cs.pawn));
	cs.move(Move(cs.knight, 38, 55, cs.pawn));
	cs.reverseMove(Move(cs.knight, 38, 55, cs.pawn));

	cs.show();
	cout << endl;
	cs.wAll.show();
	cout << endl;
	cs.bAll.show();
	*/

	/*
	for (short i=0; i<50; ++i) {
		cout << "===============" << endl;
		m = engine.bestMove(cs, 7);
		cs.move(m.first);
		cout << "Killed: " << m.first.killed << endl;
		cs.wAll.show();
		cout << endl;
		cs.bAll.show();
		cout << endl;
		cout << "Move: " << m.first.start << " to " << m.first.end << endl;
		cout << "Rating: " << m.second << endl;
		cs.show();
		cout << "===============" << endl;
		cin >> buffer;
	}
	*/

	return 0;
}

/*
For Consideration:

 - Instead of calculating all moves everytime, somehow reuse?
 - For rook move generation, remove non-horizontal pieces from board before calculation
 - Add engine settings class for things like material scoring system

*/