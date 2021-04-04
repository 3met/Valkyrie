
/* --- In this File ---
 * 1. ChessEngine Constructor
 * 2. Calculating Psudo-legal piece moves
 * 3. Calculating material value and advantages
 * 4. Calculate the best move from a given game state */

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "U64.hpp"

using namespace std;

ChessEngine::ChessEngine() {

	srand(time(0));

	read_move_table(&KMoveDB, "king-moves.movetable");
	read_move_table(&NMoveDB, "knight-moves.movetable");

	read_bonus_table(&knightBonus, "knight-bonus.bonustable");
}

ChessEngine::~ChessEngine() {}

// ----- Scoring Game State -----
// Standard material valuation
const short ChessEngine::materialValsSTD[6] = {
	100, 300, 300, 500, 900, 20000,
};

// Larry Kaufman's material valuation
const short ChessEngine::materialValsLK[6] = {
	// https://web.archive.org/web/20160314214435/http://www.danheisman.com/Articles/evaluation_of_material_imbalance.htm
	100, 325, 325, 500, 975, 20000,
};

// Hans Berliner's material valuation
const short ChessEngine::materialValsHB[6] = {
	// https://en.wikipedia.org/wiki/Chess_piece_relative_value#Hans_Berliner's_system
	100, 320, 333, 510, 880, 20000,
};

// ----- Primary Operations -----
short ChessEngine::eval_side(ChessState* cs, bool side, vector<U8> pieces[2][6]) {

	U8 i;
	short rating = 0;

	// Account for material advantage
	for (i=0; i<6; ++i) {
		rating += pieces[side][i].size() * materialValsSTD[i];
	}

	// Bonus for having two bishops
	if (pieces[side][cs->BISHOP].size() == 2) {
		rating += 50;
	}

	// --- Adjustment for material placement ---
	// Knight placement
	for (i=0; i<pieces[side][cs->KNIGHT].size(); ++i) {
		rating += knightBonus[pieces[side][cs->KNIGHT][i]];
	}

	return rating;
}

short ChessEngine::eval_board(ChessState* cs) {
	/* Rates the status of game in terms of advantage */

	U8 i;

	// Positions of all the pieces
	vector<U8> pieces[2][6] = {
		// White
		{
			cs->pieces[0][0]->getPosVector(),
			cs->pieces[0][1]->getPosVector(),
			cs->pieces[0][2]->getPosVector(),
			cs->pieces[0][3]->getPosVector(),
			cs->pieces[0][4]->getPosVector(),
			cs->pieces[0][5]->getPosVector(),
		},
		// Black
		{
			cs->pieces[1][0]->getPosVector(),
			cs->pieces[1][1]->getPosVector(),
			cs->pieces[1][2]->getPosVector(),
			cs->pieces[1][3]->getPosVector(),
			cs->pieces[1][4]->getPosVector(),
			cs->pieces[1][5]->getPosVector(),
		}
	};

	short rating = eval_side(cs, cs->WHITE, pieces) - eval_side(cs, cs->BLACK, pieces);

	return rating;
}

pair<Move, short> ChessEngine::bestMove(ChessState* cs, U8 depth) {
	Move bestMove;
	short rating = minimax_eval_top(cs, depth, -30000, 30000, &bestMove);

	return make_pair(bestMove, rating);
}

short ChessEngine::minimax_eval_top(ChessState* cs, U8 depth, short alpha, short beta, Move* bestMove) {
	/* Evaluates the passed position.
	 * Alpha represents the min guaranteed eval. 
	 * Beta represents the max guaranteed eval. */ 

	if (depth == 0) {	// Add case if checkmate?
		return eval_board(cs);
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	short eval;

	if (cs->turn == cs->WHITE) {
		short maxEval = -30000;	// Arbitrary low number

		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
			cs->reverseMove(moves[i]);

			if (maxEval < eval) {
				maxEval = eval;
				*bestMove = moves[i];
			}
			if (alpha < maxEval) {
				alpha = maxEval;
			}
			if (alpha >= beta) {
				break;
			}
		}
		return maxEval;

	} else {	// Black to play
		short minEval = 30000;	// Arbitrary high number

		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
			cs->reverseMove(moves[i]);

			if (minEval > eval) {
				minEval = eval;
				*bestMove = moves[i];
			}
			if (beta > minEval) {
				beta = minEval;
			}
			if (beta <= alpha) {
				break;
			}
		}

		return minEval;
	}
}

short ChessEngine::minimax_eval(ChessState* cs, U8 depth, short alpha, short beta) {
	/* Evaluates the passed position.
	 * Alpha represents the min guaranteed eval. 
	 * Beta represents the max guaranteed eval. */ 

	if (depth == 0) {	// Add case if checkmate?
		return eval_board(cs);
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	short eval;

	if (cs->turn == cs->WHITE) {
		short maxEval = -30000;	// Arbitrary low number

		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
			cs->reverseMove(moves[i]);

			if (maxEval < eval) {
				maxEval = eval;
			}
			if (alpha < maxEval) {
				alpha = maxEval;
			}
			if (alpha >= beta) {
				break;
			}
		}
		return maxEval;

	} else {	// Black to play
		short minEval = 30000;	// Arbitrary high number

		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
			cs->reverseMove(moves[i]);

			if (minEval > eval) {
				minEval = eval;
			}
			if (beta > minEval) {
				beta = minEval;
			}
			if (beta <= alpha) {
				break;
			}
		}

		return minEval;
	}
}
