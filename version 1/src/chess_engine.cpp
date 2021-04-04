
/* --- In this File ---
 * 1. ChessEngine Constructor
 * 2. Calculating Psudo-legal piece moves
 * 3. Calculating material value and advantages
 * 4. Calculate the best move from a given game state */

#include <algorithm>
#include <fstream>
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

	ifstream db_file;

	// Load king move database
	db_file.open("../data/king-moves.movelist");
	if (!db_file) {
		cout << "Fatal Error: Unable to read king moves file" << endl;
		return;
	} else {
		U64 a, b;
		for (U8 i=0; i<64; ++i) {	
			db_file >> a >> b;
			KMoveDB[i] = Bitboard(b);
		}
	}
	db_file.close();

	db_file.open("../data/knight-moves.movelist");
	if (!db_file) {
		cout << "Fatal Error: Unable to read knight moves file" << endl;
		return;
	} else {
		U64 a, b;
		for (U8 i=0; i<64; ++i) {
			db_file >> a >> b;
			NMoveDB[i] = Bitboard(b);
		}
	}
	db_file.close();
}

ChessEngine::~ChessEngine() {}

// ----- Scoring Game State -----
// Standard material valuation
const float ChessEngine::materialValsSTD[2][6] = {
	{1, 3, 3, 5, 9, 200},
	{-1, -3, -3, -5, -9, -200},
};

float ChessEngine::scoreMaterialSTD(ChessState *cs) {
	/*	Provides a score based on the material on the board 
	 * 	using the standard system */

	float total = 0;

	for (U8 i=0; i<6; ++i) {
		total += cs->pieces[0][i]->getPosVector().size() * materialValsSTD[0][i];
		total += cs->pieces[1][i]->getPosVector().size() * materialValsSTD[1][i];
	}

	return total;
}

// Larry Kaufman's material valuation
const float ChessEngine::materialValsLK[2][6] = {
	{1, 3.25, 3.25, 5, 9.75, 200},
	{-1, -3.25, -3.25, -5, -9.75, -200},
};

float ChessEngine::scoreMaterialLK(ChessState* cs) {
	/*	Provides a score based on the material on the board
	 * 	using the Larry Kaufman's system
	 *	https://web.archive.org/web/20160314214435/http://www.danheisman.com/Articles/evaluation_of_material_imbalance.htm */

	return 0;
}

// Hans Berliner's material valuation
const float ChessEngine::materialValsHB[2][6] = {
	{1, 3.2, 3.33, 5.1, 8.8, 200},
	{-1, -3.2, -3.33, -5.1, -8.8, -200},
};

float ChessEngine::scoreMaterialHB(ChessState* cs) {
	/*	Provides a score based on the material on the board
	 * 	using the Hans Berliner's system
	 *	https://en.wikipedia.org/wiki/Chess_piece_relative_value#Hans_Berliner's_system */
	return 0;
}

// ----- Primary Operations -----
float ChessEngine::rate(ChessState* cs) {
	/* Rates the status of game in terms of advantage */
	float rating = this->scoreMaterialSTD(cs);



	return rating;
}

pair<Move, float> ChessEngine::bestMove(ChessState* cs, U8 depth) {
	Move bestMove;
	float rating = minimax_eval_top(cs, depth, -10000, 10000, &bestMove);

	return make_pair(bestMove, rating);
}

float ChessEngine::minimax_eval_top(ChessState* cs, U8 depth, float alpha, float beta, Move* bestMove) {
	/* Evaluates the passed position.
	 * Alpha represents the min guaranteed eval. 
	 * Beta represents the max guaranteed eval. */ 

	if (depth == 0) {	// Add case if checkmate?
		return rate(cs);
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	if (cs->turn) {
		float maxEval = -10000;	// Arbitrary low number
		float eval;
		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
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

	} else {	// Black's turn
		float minEval = 10000;	// Arbitrary high number
		float eval;
		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
			cs->reverseMove(moves[i]);

			if (minEval > eval) {
				minEval = eval;
				*bestMove = moves[i];
			}
			if (beta > eval) {
				beta = eval;
			}
			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	}
}

float ChessEngine::minimax_eval(ChessState* cs, U8 depth, float alpha, float beta) {
	/* Evaluates the passed position.
	 * Alpha represents the min guaranteed eval. 
	 * Beta represents the max guaranteed eval. */ 

	if (depth == 0) {	// Add case if checkmate?
		return rate(cs);
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	if (cs->turn) {
		float maxEval = -10000;	// Arbitrary low number
		float eval;
		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
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

	} else {	// Black's turn
		float minEval = 10000;	// Arbitrary high number
		float eval;
		for (U8 i=0; i<moves.size(); ++i) {
			cs->move(moves[i]);
			eval = minimax_eval(cs, depth-1, alpha, beta);
			cs->reverseMove(moves[i]);

			if (minEval > eval) {
				minEval = eval;
			}
			if (beta > eval) {
				beta = eval;
			}
			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	}
}
