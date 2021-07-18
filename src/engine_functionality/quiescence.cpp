
#include <algorithm>
#include "chess_state.hpp"
#include "chess_engine.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "size_defs.hpp"

// Move ordering for kill moves
bool killMoveOrdering(const Move& a, const Move& b) {
	// Killed ordered by most valuable victim then least valuable attacker
	// (MVV/LVA)
	// If victims were the same
	if (a.killed == b.killed) {
		// If attacker A is worth less
		if (a.piece < b.piece) {
			return true;

		// If attacker B is worth less
		} else if (a.piece != b.piece) {
			return false;
		}

	// If A has a more valuable victim
	} else if (a.killed > b.killed) {
		return true;
	
	// If B has a more valuable victim
	} else {
		return false;
	}

	return false;
}

EvalScore ChessEngine::quiescence(ChessState* cs, U8 depth, EvalScore alpha, EvalScore beta) {

	if (depth == this->maxDepth) return EvalScore(evalBoard(cs, cs->turn));;

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

	sort(moveArr[depth], moveArr[depth] + moveCount, killMoveOrdering);

	for (U8 i(0); i<moveCount; ++i) {
		cs->move(moveArr[depth][i]);
		score = -quiescence(cs, depth+1, -beta, -alpha);

		if(score >= beta) {
			cs->reverseMove();
			return beta;
		}
		if(score > alpha) {
			alpha = score;
		}
		cs->reverseMove();
	}

	// TODO: Add promotions?
	// TODO: Add checks?

	return alpha;
}
