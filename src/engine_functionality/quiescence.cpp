
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "size_defs.hpp"

// Quiescence search used for avoiding the horizon effect
EvalScore ChessEngine::quiescence(ChessState* cs, U8 depth, EvalScore alpha, EvalScore beta) {

	EvalScore score = EvalScore(evalBoard(cs, cs->turn));
	if (score >= beta) {
		return beta;
	}
	if (score > alpha) {
		alpha = score;
	}

	// Generate and sort moves
	U8 moveCount;		// Number of moves (in moveArr)
	genAllKillMoves(cs, moveArr[depth], &moveCount);

	if (moveCount > 2) {
		sortMVVLVA(moveArr[depth], 0, moveCount-1, cs);
	}

	for (U8 i(0); i<moveCount; ++i) {
		cs->move(moveArr[depth][i]);

		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][KING].getFirstPos())) {

			score = -quiescence(cs, depth+1, -beta, -alpha);

			if (score >= beta) {
				cs->reverseMove();
				return beta;
			}
			if (score > alpha) {
				alpha = score;
			}
		}

		cs->reverseMove();
	}

	// TODO: Add promotions?
	// TODO: Add checks?

	return alpha;
}
