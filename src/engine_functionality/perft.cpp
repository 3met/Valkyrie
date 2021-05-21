
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "U8.hpp"
#include "U64.hpp"

using namespace std;

/*
Perft Tests:
http://talkchess.com/forum3/viewtopic.php?t=47318
*/


U64 ChessEngine::divide(ChessState* cs, U8 depth) {

	if (depth == 0) {
		return 1;
	}

	cout << "--- Divide " << short(depth) << " Start ---" << endl;

	vector<Move> moves;
	genAllMoves(cs, &moves);
	U64 total = 0;
	int moveTotal = 0;

	short nMoves = moves.size();
	for (U8 i=0; i<nMoves; ++i) {
		cs->move(moves[i]);
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING].getFirstPos())) {
			moveTotal = perft(cs, depth-1);
			cout << moves[i] << ": " << moveTotal << endl;
			total += moveTotal;
		}
		cs->reverseMove();
	}

	cout << "Total: " << total << endl;
	cout << "--- Divide " << short(depth) << " End ---" << endl << endl;

	return total;
}

U64 ChessEngine::perft(ChessState* cs, U8 depth) {

	if (depth == 0) {
		return 1;
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);
	U64 total = 0;

	short nMoves = moves.size();
	for (U8 i=0; i<nMoves; ++i) {
		cs->move(moves[i]);
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING].getFirstPos())) {
			total += perft(cs, depth-1);
		}
		cs->reverseMove();
	}

	return total;
}
