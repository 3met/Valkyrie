
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

// Factors used in static evaluation
#define USE_MATERIAL_VALUE
#define USE_MATERIAL_PLACEMENT
#define USE_DOUBLED_PAWNS
#define USE_ISOLATED_PAWNS
#define USE_BACKWARD_PAWNS
#define USE_BLOCKED_PAWNS

/* A measure of how far the game has progressed
 * 0-50		==> opening
 * 51-101	==> early game
 * 102-152	==> mid game
 * 153-203	==> late game
 * 204-255	==> end game */
U8 ChessEngine::rateGameStage(ChessState* cs, vector<U8> pieces[2][6]) {
	U8 gameStage = 0;
	
	gameStage -= pieces[0][cs->PAWN].size() * 4;
	gameStage -= pieces[1][cs->PAWN].size() * 4;
	gameStage -= pieces[0][cs->KNIGHT].size() * 10;
	gameStage -= pieces[1][cs->KNIGHT].size() * 10;
	gameStage -= pieces[0][cs->BISHOP].size() * 10;
	gameStage -= pieces[1][cs->BISHOP].size() * 10;
	gameStage -= pieces[0][cs->ROOK].size() * 16;
	gameStage -= pieces[1][cs->ROOK].size() * 16;
	gameStage -= pieces[0][cs->QUEEN].size() * 24;
	gameStage -= pieces[1][cs->QUEEN].size() * 24;	

	return gameStage;
}

/* Evaluates a score for a single color/side */
short ChessEngine::evalSide(ChessState* cs, bool side, vector<U8> pieces[2][6]) {

	U8 i;
	short rating = 0;
	U8 gameStage = rateGameStage(cs, pieces);

	#ifdef USE_MATERIAL_VALUE
		// --- Adjustment for Material Amount --- 
		// Account for general material value
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
		// Bishop placement
		for (i=0; i<pieces[side][cs->KNIGHT].size(); ++i) {
			rating += bishopBonus[pieces[side][cs->KNIGHT][i]];
		}
		// Queen placement
		for (i=0; i<pieces[side][cs->KNIGHT].size(); ++i) {
			rating += queenBonus[pieces[side][cs->KNIGHT][i]];
		}
		// King placement
		if (gameStage <= 51) {
			rating += kingOpeningBonus[side][pieces[side][cs->KING][0]];
		} else if (gameStage <= 102) {
			rating += kingEarlyBonus[side][pieces[side][cs->KING][0]];
		} else if (gameStage <= 153) {
			rating += kingMidBonus[side][pieces[side][cs->KING][0]];
		} else if (gameStage <= 204) {
			rating += kingLateBonus[side][pieces[side][cs->KING][0]];
		} else {
			rating += kingEndBonus[side][pieces[side][cs->KING][0]];
		}
	#endif

	// --- Adjustment for Pawn Structure ---
	U8 pawnsPerRank[2][8] = {{0},{0}};	// colors * ranks 
	U8 pawnsPerFile[2][8] = {{0},{0}};	// colors * files 
	// Fill Arrays
	for (i=0; i<pieces[side][cs->PAWN].size(); ++i) {
		pawnsPerRank[side][Bitboard::RANK[pieces[side][cs->PAWN][i]]] += 1;
		pawnsPerFile[side][Bitboard::FILE[pieces[side][cs->PAWN][i]]] += 1;
	}
	for (i=0; i<pieces[!side][cs->PAWN].size(); ++i) {
		pawnsPerRank[!side][Bitboard::RANK[pieces[!side][cs->PAWN][i]]] += 1;
		pawnsPerFile[!side][Bitboard::FILE[pieces[!side][cs->PAWN][i]]] += 1;
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
	#ifdef USE_BACKWARD_PAWNS

	#endif

	// Penalize blocked pawns (Covered by mobility bonus?)
	#ifdef USE_BLOCKED_PAWNS

	#endif

	// --- Adjust for Passed Pawns ---
	// Hidden Passed Pawn

	// Distance from opposing king

	// Distance to promotion bonus


	// --- Adjust for King Safty ---
	// Retain pawn protection

	// Distance from enemy pieces


	// --- Adjustment for Mobility ---
	// TODO

	return rating;
}

/* Evaluates the state of the entire board
 * A positive number indicates perspective player has an advantage
 * A negative number indicates other player has an advantage
 * The magnatude of the number represents the size of the advantage */
short ChessEngine::evalBoard(ChessState* cs, bool perspective) {

	nodesTotal += 1;

	// Positions of all the pieces
	vector<U8> pieces[2][6] = {
		// White
		{
			cs->pieces[0][0].getPosVector(),	// Pawn to king
			cs->pieces[0][1].getPosVector(),
			cs->pieces[0][2].getPosVector(),
			cs->pieces[0][3].getPosVector(),
			cs->pieces[0][4].getPosVector(),
			cs->pieces[0][5].getFirstPosVec(),
		},
		// Black
		{
			cs->pieces[1][0].getPosVector(),
			cs->pieces[1][1].getPosVector(),
			cs->pieces[1][2].getPosVector(),
			cs->pieces[1][3].getPosVector(),
			cs->pieces[1][4].getPosVector(),
			cs->pieces[1][5].getFirstPosVec(),
		}
	};

	short rating = evalSide(cs, perspective, pieces) - evalSide(cs, !perspective, pieces);;

	return rating;
}

