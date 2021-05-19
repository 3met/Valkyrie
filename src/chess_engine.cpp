
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
#include "eval_score.hpp"
#include "move.hpp"
#include "opening_table.hpp"
#include "U64.hpp"

using namespace std;

ChessEngine::ChessEngine() {
	this->load();
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

void ChessEngine::load() {
	isLoaded = false;

	srand(time(0));

	readMoveTable(KMoveDB, "king-moves.movetable");
	readMoveTable(NMoveDB, "knight-moves.movetable");

	readBonusTable(&knightBonus, "knight-bonus.bonustable");
	readBonusTable(&bishopBonus, "bishop-bonus.bonustable");
	readBonusTable(&queenBonus, "queen-bonus.bonustable");

	readOpeningBook(&openingTable, "opening_book.dat");

	isLoaded = true;
}


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

	sortMoves(&moves);

	EvalScore alpha(-1, true, 0);	// -INF; best score current color can achive 
	EvalScore beta(1, true, 0);	// INF; best score other color can achive
	short bestIndex = -1;	// -1 as default
	EvalScore score;

	for (short i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		score = -negamaxSearch(cs, 0, depth, -beta, -alpha);

		if (score > alpha 
			&& !isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING]->getFirstPos())) {
			
			alpha = score;
			bestIndex = i;
		}

		cs->reverseMove();
	}

	if (bestIndex == -1) {
		throw ChessState::NoMoves();
	}

	if (cs->turn == cs->WHITE) {
		return make_pair(moves[bestIndex], alpha);
	} else {
		return make_pair(moves[bestIndex], -alpha);
	}
}


EvalScore ChessEngine::negamaxSearch(ChessState* cs, U8 depth, U8 depthTarget, EvalScore alpha, EvalScore beta) {
	// Check for king death
	if (cs->wK.board == 0) {
		return EvalScore(-1, true, 0);
	} else if (cs->bK.board == 0) {
		return EvalScore(1, true, 0);
	}

	if (depth == depthTarget) {
		// Extend if last move was a kill
		if (cs->lastMove().killed != -1) {
			depthTarget += 1;
		// Extend if a pawn was promoted
		} else if (cs->lastMove().promoted != -1) {
			depthTarget += 1;
		// Extend if move was a check
		// } else if (isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][cs->KING]->getFirstPos())) {
			// depthTarget += 2;
		// Returns evaluation score if there is not search extension
		} else if (cs->turn == cs->BLACK) {
			return -EvalScore(evalBoard(cs));
		} else {
			return EvalScore(evalBoard(cs));
		}		 
	}

	// Generate and sort moves
	vector<Move> moves;
	genAllMoves(cs, &moves);
	
	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
		// return 0;
	}

	sortMoves(&moves);

	EvalScore score;
	EvalScore initAlpha = alpha;

	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		score = -negamaxSearch(cs, depth+1, depthTarget, -beta, -alpha);
		
		if (score >= beta 
			&& !isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING]->getFirstPos())) {
			
			cs->reverseMove();
			return beta;
		}

		if (score > alpha 
			&& !isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING]->getFirstPos())) {

			alpha = score;
		}

		cs->reverseMove();
	}

	// If 
	if (alpha == initAlpha) {

	}

	return alpha;
}
