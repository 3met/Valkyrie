
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Factors used in static evaluation
#define USE_MATERIAL_VALUE
#define USE_MATERIAL_PLACEMENT
#define USE_DOUBLED_PAWNS
#define USE_ISOLATED_PAWNS
#define USE_BACKWARD_PAWNS
#define USE_BLOCKED_PAWNS

// A measure of how far the game has progressed
// 0-50		==> opening
// 51-101	==> early game
// 102-152	==> mid game
// 153-203	==> late game
// 204-255	==> end game
U8 ChessEngine::rateGameStage(U8 pieceCount[2][5]) {
	U8 gameStage(0);

	gameStage -= pieceCount[0][0] << 2;
	gameStage -= pieceCount[1][0] << 2;
	gameStage -= pieceCount[0][1] * 10;
	gameStage -= pieceCount[1][1] * 10;
	gameStage -= pieceCount[0][2] * 10;
	gameStage -= pieceCount[1][2] * 10;
	gameStage -= pieceCount[0][3] << 4;
	gameStage -= pieceCount[1][3] << 4;
	gameStage -= pieceCount[0][4] * 24;
	gameStage -= pieceCount[1][4] * 24;	

	return gameStage;
}

// Evaluates a score for a single color/side
short ChessEngine::evalSide(ChessState* cs, bool side) {

	U8 i;
	short rating(0);
	U8 gameStage(pieceCount[2][5]);

	#ifdef USE_MATERIAL_VALUE
		// --- Adjustment for Material Amount --- 
		// Account for general material value
		rating += pieceCount[side][cs->PAWN] * materialVals[cs->PAWN];
		rating += pieceCount[side][cs->KNIGHT] * materialVals[cs->KNIGHT];
		rating += pieceCount[side][cs->BISHOP] * materialVals[cs->BISHOP];
		rating += pieceCount[side][cs->ROOK] * materialVals[cs->ROOK];
		rating += pieceCount[side][cs->QUEEN] * materialVals[cs->QUEEN];

		// Bonus for having two bishops
		if (pieceCount[side][cs->BISHOP] == 2) {
			rating += 50;
		}

		// Adjust knight value with pawns
		// +6 for each pawn above 5, -6 for each below
		rating += pieceCount[side][cs->KNIGHT] * ((6*pieceCount[side][cs->PAWN]) - 30);

		// Adjust rook value with pawns
		// -12 for each pawn above 5, +12 for each below
		rating += pieceCount[side][cs->ROOK] * ((-12*pieceCount[side][cs->PAWN]) + 60);
	#endif

	#ifdef USE_MATERIAL_PLACEMENT
		// --- Adjustment for Material Placement ---
		// Knight placement
		for (i=0; i<pieceCount[side][cs->KNIGHT]; ++i) {
			rating += knightBonus[knightPosArr[side][i]];
		}
		// Bishop placement
		for (i=0; i<pieceCount[side][cs->BISHOP]; ++i) {
			rating += bishopBonus[bishopPosArr[side][i]];
		}
		// Queen placement
		for (i=0; i<pieceCount[side][cs->QUEEN]; ++i) {
			rating += queenBonus[queenPosArr[side][i]];
		}
		// King placement
		if (gameStage <= 51) {
			rating += kingOpeningBonus[side][kingPos[side]];
		} else if (gameStage <= 102) {
			rating += kingEarlyBonus[side][kingPos[side]];
		} else if (gameStage <= 153) {
			rating += kingMidBonus[side][kingPos[side]];
		} else if (gameStage <= 204) {
			rating += kingLateBonus[side][kingPos[side]];
		} else {
			rating += kingEndBonus[side][kingPos[side]];
		}
	#endif

	// --- Adjustment for Pawn Structure ---
	U8 pawnsPerRank[2][8] = {{0},{0}};	// colors * ranks 
	U8 pawnsPerFile[2][8] = {{0},{0}};	// colors * files 
	// Fill Arrays
	for (i=0; i<pieceCount[side][cs->PAWN]; ++i) {
		pawnsPerRank[side][Bitboard::RANK[pawnPosArr[side][i]]] += 1;
		pawnsPerFile[side][Bitboard::FILE[pawnPosArr[side][i]]] += 1;
	}
	for (i=0; i<pieceCount[!side][cs->PAWN]; ++i) {
		pawnsPerRank[!side][Bitboard::RANK[pawnPosArr[!side][i]]] += 1;
		pawnsPerFile[!side][Bitboard::FILE[pawnPosArr[!side][i]]] += 1;
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

// Evaluates the state of the entire board
// A positive number indicates perspective player has an advantage
// A negative number indicates other player has an advantage
// The magnatude of the number represents the size of the advantage
short ChessEngine::evalBoard(ChessState* cs, bool perspective) {

	nodesTotal += 1;

	// Positions of all the pieces
	cs->pieces[0][cs->PAWN].getPosArr(pawnPosArr[0], &pieceCount[0][cs->PAWN]);
	cs->pieces[0][cs->KNIGHT].getPosArr(knightPosArr[0], &pieceCount[0][cs->KNIGHT]);
	cs->pieces[0][cs->BISHOP].getPosArr(bishopPosArr[0], &pieceCount[0][cs->BISHOP]);
	cs->pieces[0][cs->ROOK].getPosArr(rookPosArr[0], &pieceCount[0][cs->ROOK]);
	cs->pieces[0][cs->QUEEN].getPosArr(queenPosArr[0], &pieceCount[0][cs->QUEEN]);
	kingPos[0] = cs->pieces[0][cs->KING].getFirstPos();
	cs->pieces[1][cs->PAWN].getPosArr(pawnPosArr[1], &pieceCount[1][cs->PAWN]);
	cs->pieces[1][cs->KNIGHT].getPosArr(knightPosArr[1], &pieceCount[1][cs->KNIGHT]);
	cs->pieces[1][cs->BISHOP].getPosArr(bishopPosArr[1], &pieceCount[1][cs->BISHOP]);
	cs->pieces[1][cs->ROOK].getPosArr(rookPosArr[1], &pieceCount[1][cs->ROOK]);
	cs->pieces[1][cs->QUEEN].getPosArr(queenPosArr[1], &pieceCount[1][cs->QUEEN]);
	kingPos[1] = cs->pieces[1][cs->KING].getFirstPos();
	
	short rating(evalSide(cs, perspective) - evalSide(cs, !perspective));

	return rating;
}
