
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
	
	// Generate psudo-legal moves
	vector<Move> moves;
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
	HashScore hashScore;	// Transposition table entry

	for (short i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);

		#ifdef USE_TRANS_TABLE
		if (this->useTransTable && this->transTable.contains(cs)) {
			hashScore = this->transTable.get(cs);
			
			if (hashScore.depth >= depth) {
				if (score > alpha) {
					alpha = score;
					bestIndex = i;
					cs->reverseMove();
					continue;
				}
			}
		}
		#endif

		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING]->getFirstPos())) {
			
			score = -negamaxSearch(cs, 0, depth-1, -beta, -alpha);

			if (score.foundMate) {
				score.movesToMate += 1;
			}

			#ifdef USE_TRANS_TABLE
			this->transTable.add(cs, score, depth);
			#endif

			if (score > alpha) {
				alpha = score;
				bestIndex = i;
			}
		}

		cs->reverseMove();
	}

	// Throw error if no move were legal
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
		} else {
			return EvalScore(evalBoard(cs, cs->turn));
		}		 
	}

	// Generate and sort moves
	vector<Move> moves;
	genAllMoves(cs, &moves);
	
	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	sortMoves(&moves);

	bool hasValidMove = false;
	EvalScore score;
	HashScore hashScore;	// Transposition table entry

	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);

		#ifdef USE_TRANS_TABLE
		if (this->transTable.contains(cs)) {
			hasValidMove = true;

			hashScore = this->transTable.get(cs);

			if (hashScore.depth >= depth) {
				if (score >= beta) {
					cs->reverseMove();
					return beta;
				}

				if (score > alpha) {
					alpha = score;
					cs->reverseMove();
					continue;
				}
			}
		}
		#endif

		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING]->getFirstPos())) {
			score = -negamaxSearch(cs, depth+1, depthTarget, -beta, -alpha);

			hasValidMove = true;

			if (score.foundMate) {
				score.movesToMate += 1;
			}

			#ifdef USE_TRANS_TABLE
			this->transTable.add(cs, score, depth);
			#endif

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

	// If there is no valid move, it is either checkmate or stalemate
	if (!hasValidMove) {
		// If the active player's king is not being attacked
		// then the situation is stalemate
		if (!isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][cs->KING]->getFirstPos())) {
			return EvalScore(0);
		}

		// Else there is checkmate
		return EvalScore(-1, true, 0);
	}

	return alpha;
}
