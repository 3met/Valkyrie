
#include "chess_state.hpp"
#include "chess_engine.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "size_defs.hpp"

// Zero window search
EvalScore ChessEngine::zwSearch(ChessState* cs, U8 depth, U8 ply, EvalScore beta) {

	if (depth == 0) {
		return quiescence(cs, ply+1, beta-1, beta);
	}

	// Generate and sort moves
	U8 moveCount;		// Number of moves (in moveArr)
	genAllMoves(cs, moveArr[ply], &moveCount);

	// Move ordering
	sortMoves(moveArr[ply], &moveCount, ply, &Move::NULL_MOVE);

	EvalScore score;
	
	for (U8 i(0); i<moveCount; ++i) {

		cs->move(moveArr[ply][i]);

		// Check if move is legal before preceding
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING].getFirstPos())) {
			
			score = -zwSearch(cs, depth-1, ply+1, -beta+1);

			if (score.hasMate()) {
				score.addHalfMoveToMate();
			}

			if (score >= beta) {
				cs->reverseMove();
				return beta;
			}
		}

		cs->reverseMove();
	}

	return beta-1;
}

