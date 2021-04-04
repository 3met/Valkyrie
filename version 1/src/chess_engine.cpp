
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

	read_move_list(&KMoveDB, "king-moves.movelist");
	read_move_list(&NMoveDB, "knight-moves.movelist");
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

	short rating = 0;

	// Account for material advantage
	for (U8 i=0; i<6; ++i) {
		rating += pieces[side][i].size() * materialValsSTD[i];
	}

	// 

	// Bonus for having two bishops
	if (pieces[side][cs->BISHOP].size() == 2) {
		rating += 50;
	}

	// 

	return rating;
}

short ChessEngine::eval(ChessState* cs) {
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
	short rating = minimax_eval_top(cs, depth, -10000, 10000, &bestMove);

	return make_pair(bestMove, rating);
}

short ChessEngine::minimax_eval_top(ChessState* cs, U8 depth, short alpha, short beta, Move* bestMove) {
	/* Evaluates the passed position.
	 * Alpha represents the min guaranteed eval. 
	 * Beta represents the max guaranteed eval. */ 

	if (depth == 0) {	// Add case if checkmate?
		return eval(cs);
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	short maxEval = -30000;	// Arbitrary low number
	short eval;
	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		eval = -minimax_eval(cs, depth-1, -beta, -alpha);
		if (!cs->turn) {
			eval = -eval;
		}
		cs->reverseMove(moves[i]);

		if (maxEval < eval) {
			maxEval = eval;
			*bestMove = moves[i];
		}
		if (alpha < eval) {
			alpha = eval;
		}
		if (beta <= alpha) {
			break;
		}
	}
	return maxEval;
}

short ChessEngine::minimax_eval(ChessState* cs, U8 depth, short alpha, short beta) {
	/* Evaluates the passed position.
	 * Alpha represents the min guaranteed eval. 
	 * Beta represents the max guaranteed eval. */ 

	if (depth == 0) {	// Add case if checkmate?
		return eval(cs);
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	short maxEval = -30000;	// Arbitrary low number
	short eval;
	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		eval = -minimax_eval(cs, depth-1, -beta, -alpha);
		if (!cs->turn) {
			eval = -eval;
		}
		cs->reverseMove(moves[i]);

		if (maxEval < eval) {
			maxEval = eval;
		}
		if (alpha < eval) {
			alpha = eval;
		}
		if (beta <= alpha) {
			break;
		}
	}
	return maxEval;
}
