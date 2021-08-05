
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
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
	sortMainNoHash(moveArr[ply], 0, moveCount-1, cs, ply);

	EvalScore score;
	bool hasValidMove(false);
	
	for (U8 i(0); i<moveCount; ++i) {

		cs->move(moveArr[ply][i]);

		// Check if move is legal before preceding
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][KING].getFirstPos())) {
			
			hasValidMove = true;
			score = -zwSearch(cs, depth-1, ply+1, -beta+1);

			if (score >= beta) {
				cs->reverseMove();
				return beta;
			}
		}

		cs->reverseMove();
	}

	// If there is no valid move, it is either checkmate or stalemate
	if (!hasValidMove) {
		// If the active player's king is not being attacked
		// then the situation is stalemate
		if (!isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][KING].getFirstPos())) {
			return EvalScore::DRAW;
		}

		// Else there is checkmate
		return -EvalScore::MATE_IN_0 + ply;
	}

	return beta-1;
}

