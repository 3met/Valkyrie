
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

pair<Move, float> ChessEngine::bestMove(ChessState* cs, short depth) {

	U8 i;
	vector<Move> validMoves;
	vector<pair<Move, float>> ratedMoves;

	// Generate all moves
	genAllMoves(cs, &validMoves);

	// Check if valid moves were generated
	if (validMoves.size() == 0) {
		throw ChessState::NoMoves();
	}

	// --- Recursively Calculate Best Move ---
	if (depth <= 1) {
		// Create a vector of ratings paired with moves
		for (i=0; i<validMoves.size(); ++i) {
			cs->move(validMoves[i]);
			ratedMoves.push_back(make_pair(validMoves[i], this->rate(cs)));
			cs->reverseMove(validMoves[i]);
		}

	} else {
		// Make each move then recursively calculate its rating
		// before reversing the move. 
		for (i=0; i<validMoves.size(); ++i) {
			cs->move(validMoves[i]);
			try {
				ratedMoves.push_back(
					make_pair(validMoves[i], this->bestMove(cs, depth-1).second));
			} catch (ChessState::NoMoves& e) {
				ratedMoves.push_back(
					make_pair(validMoves[i], 0));
			}
			cs->reverseMove(validMoves[i]);
		}
	}

	// Return the best move for the current player
	sort(ratedMoves.begin(), ratedMoves.end(), this->sortRatedMove);

	// Return highest rating for white, lowest for black
	if (cs->turn) {
		return ratedMoves[0];
	} else {
		return ratedMoves[ratedMoves.size() - 1];
	}
}


float ChessEngine::minimax_eval(ChessState* cs, short depth, float alpha, float beta) {
	/* Evaluates the passed position.
	 * Alpha represents the min guaranteed eval. 
	 * Beta represents the max guaranteed eval. */ 

	if (depth == 0) {	// Add case if checkmate?
		return rate(cs);
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	if (cs->turn == cs->WHITE) {
		float maxEval = -10000;	// Arbitrary low number
		float eval;
		for (U8 i=0; i<moves.size(); ++i) {
			eval = minimax_eval(cs, depth-1, alpha, beta);

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
			eval = minimax_eval(cs, depth-1, alpha, beta);

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
