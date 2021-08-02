
#include <iostream>
#include <stdio.h>
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

using namespace std;

// Similar to perft, except prints node count for each starting child node
U64 ChessEngine::divide(ChessState* cs, U8 depth) {

	if (depth == 0) {
		return 1;
	}

	printf("--- Divide %d Start ---\n", short(depth));

	U8 moveCount;		// Number of moves (in moveArr)
	genAllMoves(cs, moveArr[depth], &moveCount);
	U64 total(0);
	U64 moveTotal(0);

	for (U8 i(0); i<moveCount; ++i) {
		cs->move(moveArr[depth][i]);
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][KING].getFirstPos())) {
			moveTotal = perft(cs, depth-1);
			cout << moveArr[depth][i] << ": " << moveTotal << '\n';
			total += moveTotal;
		}
		cs->reverseMove();
	}

	cout << "Total: " << total << '\n';
	printf("--- Divide %d End ---\n\n", short(depth));

	return total;
}

// Calculates how many moves combinations there are for a given depth.
U64 ChessEngine::perft(ChessState* cs, U8 depth) {

	if (depth == 0) {
		return 1;
	}

	U8 moveCount;		// Number of moves (in moveArr)
	genAllMoves(cs, moveArr[depth], &moveCount);
	U64 total(0);

	for (U8 i(0); i<moveCount; ++i) {
		cs->move(moveArr[depth][i]);
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][KING].getFirstPos())) {
			total += perft(cs, depth-1);
		}
		cs->reverseMove();
	}

	return total;
}
