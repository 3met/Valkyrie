
/* --- In this File ---
 * 1. ChessEngine Constructor
 * 2. Material value sets
 * 3. Static board evaluation tactics
 * 4. Move ordering (for alpha-beta)
 * 5. Best move calculations (negamax, alpha-beta) */

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

// Factors used in static evaluation
#define USE_MATERIAL_VALUE
#define USE_MATERIAL_PLACEMENT
#define USE_DOUBLED_PAWNS
#define USE_ISOLATED_PAWNS

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

	#ifdef USE_MATERIAL_VALUE
	// --- Adjustment for Material Amount --- 
	// Account for material advantage
	for (i=0; i<6; ++i) {
		rating += pieces[side][i].size() * materialValsLK[i];
	}

	// Bonus for having two bishops
	if (pieces[side][cs->BISHOP].size() == 2) {
		rating += 50;
	}

	// Adjust knight value with pawns
	// +6 for each pawn above 5, -6 for each below
	rating += pieces[side][cs->KNIGHT].size() * ((6*pieces[side][cs->PAWN].size()) - 30);

	// Adjust rook value with pawns
	// -12 for each pawn above 5, +12 for each below
	rating += pieces[side][cs->ROOK].size() * ((-12*pieces[side][cs->PAWN].size()) + 60);
	#endif

	#ifdef USE_MATERIAL_PLACEMENT
	// --- Adjustment for Material Placement ---
	// Knight placement
	for (i=0; i<pieces[side][cs->KNIGHT].size(); ++i) {
		rating += knightBonus[pieces[side][cs->KNIGHT][i]];
	}
	#endif

	// --- Adjustment for Pawn Structure ---
	U8 pawnsPerFile[2][8] = {{0},{0}};	// colors * files 
	U8 file;
	// Fill pawnsPerFile array
	for (i=0; i<pieces[side][cs->PAWN].size(); ++i) {
		file = pieces[side][cs->PAWN][i] % 8;
		pawnsPerFile[side][file] += 1;
	}
	for (i=0; i<pieces[!side][cs->PAWN].size(); ++i) {
		file = pieces[!side][cs->PAWN][i] % 8;
		pawnsPerFile[!side][file] += 1;
	}

	#ifdef USE_DOUBLED_PAWNS
	// Penalize doubled pawns
	// Subtract 40 centipawns for each doubled/tripled pawn
	for (i=0; i<8; ++i) {
		if (pawnsPerFile[side][i] > 1) {
			rating -= (pawnsPerFile[side][i]-1) * 40;
		}
	}

	// TODO: types of doubled pawns
	// https://en.wikipedia.org/wiki/Chess_piece_relative_value
	#endif

	#ifdef USE_ISOLATED_PAWNS
	// Penalize isolated pawns
	// Substract 10 centipawns for each isolated pawn
	i = 0;	// File
	while (i < 7) {	// Files a to g
		if (pawnsPerFile[side][i] > 0) {
			if (pawnsPerFile[side][i+1] == 0) {
				rating -= 10 * pawnsPerFile[side][i];
				i += 2;
			} else {
				i += 3;
			}
		} else {
			++i;
		}
	}
	if (i == 7 && pawnsPerFile[side][7] > 0 && pawnsPerFile[side][6] == 0) {
		// Pawn on last file (h)
		rating -= 10 * pawnsPerFile[side][7];
	}
	#endif

	// Penalize backward pawns

	// Penalize blocked pawns (Covered by mobility bonus?)


	// --- Adjust for Passed Pawns ---
	// Hidden Passed Pawn

	// Distance from opposing king

	// Distance to promotion bonus


	// --- Adjust for King Safty ---
	// Stay in corner during middle game

	// Retain pawn protection

	// Distance from enemy pieces


	// --- Adjustment for Mobility ---
	// TODO

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
	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	// TODO sort moves

	short alpha = -30000;	// -INF; best score current color can achive 
	short beta = 30000;	// INF; best score other color can achive
	U8 bestIndex;
	short score;
	short bestScore;

	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		score = -negamaxSearch(cs, depth-1, -beta, -alpha);
		cs->reverseMove(moves[i]);

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


short ChessEngine::negamaxSearch(ChessState* cs, U8 depth, short alpha, short beta) {
	if (depth == 0) {
		short rating = eval_board(cs);
		if (cs->turn == cs->BLACK) {
			return -rating;
		} else {
			return rating;
		}		 
	}

	vector<Move> moves;
	genAllMoves(cs, &moves);

	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	short score;

	for (U8 i=0; i<moves.size(); ++i) {
		cs->move(moves[i]);
		score = -negamaxSearch(cs, depth-1, -beta, -alpha);
		cs->reverseMove(moves[i]);

		if (score >= beta) {
			return beta;
		}
		if (score > alpha) {
			alpha = score;
		}
	}

	return alpha;
}
