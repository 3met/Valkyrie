
/* --- In this File ---
 * 1. ChessEngine Constructor
 * 2. Material value sets
 * 3. Static board evaluation tactics
 * 4. Move ordering (for alpha-beta)
 * 5. Best move calculations (negamax, alpha-beta) */

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "opening_table.hpp"
#include "U64.hpp"

using namespace std;

ChessEngine::ChessEngine() {
	cout << "Loading Engine..." << endl;

	srand(time(0));

	read_move_table(&KMoveDB, "king-moves.movetable");
	read_move_table(&NMoveDB, "knight-moves.movetable");

	read_bonus_table(&knightBonus, "knight-bonus.bonustable");

	read_opening_book(&openingTable, "opening_book.dat");

	cout << "Engine Loading Complete" << endl << endl;
}

ChessEngine::~ChessEngine() {}

// ----- Scoring Game State -----
// Standard material valuation
const short ChessEngine::materialValsSTD[6] = {
	100, 300, 300, 500, 900, 30000,
};

// Larry Kaufman's material valuation
const short ChessEngine::materialValsLK[6] = {
	// https://web.archive.org/web/20160314214435/http://www.danheisman.com/Articles/evaluation_of_material_imbalance.htm
	100, 325, 325, 500, 975, 30000,
};

// Hans Berliner's material valuation
const short ChessEngine::materialValsHB[6] = {
	// https://en.wikipedia.org/wiki/Chess_piece_relative_value#Hans_Berliner's_system
	100, 320, 333, 510, 880, 30000,
};


// ----- Primary Operations -----
pair<Move, EvalScore> ChessEngine::bestMove(ChessState* cs, U8 depth) {
	vector<Move> moves;

	// Check if position in opening book
	if (openingTable.contains(cs)) {
		cout << "Using Opening Book" << endl;
		moves = openingTable.get(cs);
		return make_pair(moves[rand() % moves.size()], evalBoard(cs));
	}

	genAllMoves(cs, &moves);
	
	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	// TODO sort moves

	EvalScore alpha(-1, true, 0);	// -INF; best score current color can achive 
	EvalScore beta(1, true, 0);	// INF; best score other color can achive
	U8 bestIndex;
	EvalScore score;

	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		score = -negamaxSearch(cs, depth-1, -beta, -alpha);
		cs->reverseMove();

		if (score > alpha) {
			alpha = score;
			bestIndex = i;
		}
	}

	if (cs->turn == cs->WHITE) {
		return make_pair(moves[bestIndex], alpha);
	} else {
		return make_pair(moves[bestIndex], -alpha);
	}
}


EvalScore ChessEngine::negamaxSearch(ChessState* cs, U8 depth, EvalScore alpha, EvalScore beta) {
	if (depth == 0) {
		EvalScore es(evalBoard(cs));

		if (es.eval > 20000) {	// If black king is dead
			es.eval = 1;
			es.foundMate = true;
			es.movesToMate = 0;
		} else if (es.eval < -20000) {	// If white king is dead
			es.eval = -1;
			es.foundMate = true;
			es.movesToMate = 0;
		}

		if (cs->turn == cs->BLACK) {
			return -es;
		} else {
			return es;
		}		 
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
		// return 0;
	}

	EvalScore score;

	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		score = -negamaxSearch(cs, depth-1, -beta, -alpha);
		cs->reverseMove();

		if (score >= beta) {
			return beta;
		}

		if (score.eval > 20000) {	// If black king is dead
			score.eval = 1;
			score.foundMate = true;
			score.movesToMate = 0;
		} else if (score.eval < -20000) {	// If white king is dead
			score.eval = -1;
			score.foundMate = true;
			score.movesToMate = 0;
		}

		if (score > alpha) {
			alpha = score;
		}
	}

	return alpha;
}
