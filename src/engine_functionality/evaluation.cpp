
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Factors used in static evaluation
#define USE_MATERIAL_VALUE
#define USE_MATERIAL_PLACEMENT
#define USE_DOUBLED_PAWNS	// Requires USE_MATERIAL_PLACEMENT
#define USE_ISOLATED_PAWNS	// Requires USE_MATERIAL_PLACEMENT
#define USE_BACKWARD_PAWNS	// Requires USE_MATERIAL_PLACEMENT
#define USE_BLOCKED_PAWNS	// Requires USE_MATERIAL_PLACEMENT

// A measure of how far the game has progressed
// 0-50		==> opening
// 51-101	==> early game
// 102-152	==> mid game
// 153-203	==> late game
// 204-255	==> end game
void ChessEngine::setGameStage() {
	gameStage = -(pieceCount[0][0] << 2);
	gameStage -= pieceCount[1][0] << 2;
	gameStage -= pieceCount[0][1] * 10;
	gameStage -= pieceCount[1][1] * 10;
	gameStage -= pieceCount[0][2] * 10;
	gameStage -= pieceCount[1][2] * 10;
	gameStage -= pieceCount[0][3] << 4;
	gameStage -= pieceCount[1][3] << 4;
	gameStage -= pieceCount[0][4] * 24;
	gameStage -= pieceCount[1][4] * 24;	
}

// Prepare the object for evaluation.
// Must be called before any evaluation method other
// than ChessEngine::evalBoard()
void ChessEngine::prepEval(ChessState* cs) {
	// Positions of all the pieces
	cs->pieces[0][PAWN].getPosArr(pawnPosArr[0], &pieceCount[0][PAWN]);
	cs->pieces[0][KNIGHT].getPosArr(knightPosArr[0], &pieceCount[0][KNIGHT]);
	cs->pieces[0][BISHOP].getPosArr(bishopPosArr[0], &pieceCount[0][BISHOP]);
	cs->pieces[0][ROOK].getPosArr(rookPosArr[0], &pieceCount[0][ROOK]);
	cs->pieces[0][QUEEN].getPosArr(queenPosArr[0], &pieceCount[0][QUEEN]);
	kingPos[0] = cs->pieces[0][KING].getFirstPos();
	cs->pieces[1][PAWN].getPosArr(pawnPosArr[1], &pieceCount[1][PAWN]);
	cs->pieces[1][KNIGHT].getPosArr(knightPosArr[1], &pieceCount[1][KNIGHT]);
	cs->pieces[1][BISHOP].getPosArr(bishopPosArr[1], &pieceCount[1][BISHOP]);
	cs->pieces[1][ROOK].getPosArr(rookPosArr[1], &pieceCount[1][ROOK]);
	cs->pieces[1][QUEEN].getPosArr(queenPosArr[1], &pieceCount[1][QUEEN]);
	kingPos[1] = cs->pieces[1][KING].getFirstPos();

	setGameStage();
}

void ChessEngine::evalPawns(bool side) {
	pawnEvalResult[side] = 0;

	#ifdef USE_MATERIAL_VALUE
		pawnEvalResult[side] += pieceCount[side][PAWN] * materialVals[PAWN];
	#endif

	#ifdef USE_MATERIAL_PLACEMENT
		// --- Adjustment for Pawn Structure ---
		U8 pawnsPerRank[2][8] = {{0},{0}};	// colors * ranks
		U8 pawnsPerFile[2][8] = {{0},{0}};	// colors * files
		U8 i;
		// Fill Arrays
		for (i=0; i<pieceCount[side][PAWN]; ++i) {
			pawnsPerRank[side][BOARD_RANK[pawnPosArr[side][i]]] += 1;
			pawnsPerFile[side][BOARD_FILE[pawnPosArr[side][i]]] += 1;
		}
		for (i=0; i<pieceCount[!side][PAWN]; ++i) {
			pawnsPerRank[!side][BOARD_RANK[pawnPosArr[!side][i]]] += 1;
			pawnsPerFile[!side][BOARD_FILE[pawnPosArr[!side][i]]] += 1;
		}

		// Consider pawn placement based on game stage
		if (gameStage <= 51) {
			for (i=0; i<pieceCount[side][PAWN]; ++i) {
				pawnEvalResult[side] += pawnOpeningBonus[side][pawnPosArr[side][i]];
			}
		} else if (gameStage <= 102) {
			for (i=0; i<pieceCount[side][PAWN]; ++i) {
				pawnEvalResult[side] += pawnEarlyBonus[side][pawnPosArr[side][i]];
			}
		} else if (gameStage <= 153) {
			for (i=0; i<pieceCount[side][PAWN]; ++i) {
				pawnEvalResult[side] += pawnMidBonus[side][pawnPosArr[side][i]];
			}
		} else if (gameStage <= 204) {
			for (i=0; i<pieceCount[side][PAWN]; ++i) {
				pawnEvalResult[side] += pawnLateBonus[side][pawnPosArr[side][i]];
			}
		} else {
			for (i=0; i<pieceCount[side][PAWN]; ++i) {
				pawnEvalResult[side] += pawnEndBonus[side][pawnPosArr[side][i]];
			}
		}

		#ifdef USE_DOUBLED_PAWNS
			// Penalize doubled pawns
			// Subtract 40 centipawns for each doubled/tripled pawn
			// Subtract 45 if on the edge
			// Seems to run much faster without a loop
			if (pawnsPerFile[side][0] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][0]-1) * 40;
			if (pawnsPerFile[side][1] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][1]-1) * 40;
			if (pawnsPerFile[side][2] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][2]-1) * 40;
			if (pawnsPerFile[side][3] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][3]-1) * 40;
			if (pawnsPerFile[side][4] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][4]-1) * 40;
			if (pawnsPerFile[side][5] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][5]-1) * 40;
			if (pawnsPerFile[side][6] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][6]-1) * 40;
			if (pawnsPerFile[side][7] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][7]-1) * 40;

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
						pawnEvalResult[side] -= 10 * pawnsPerFile[side][i];
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
				pawnEvalResult[side] -= 10 * pawnsPerFile[side][7];
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
	#endif
}

void ChessEngine::evalKnights(bool side) {
	knightEvalResult[side] = 0;

	#ifdef USE_MATERIAL_VALUE
		knightEvalResult[side] += pieceCount[side][KNIGHT] * materialVals[KNIGHT];

		// Adjust knight value based on number of pawns
		// +6 for each pawn above 5, -6 for each below
		knightEvalResult[side] += pieceCount[side][KNIGHT] * ((6*pieceCount[side][PAWN]) - 30);
	#endif

	#ifdef USE_MATERIAL_PLACEMENT
		// Consider knight placement
		for (U8 i(0); i<pieceCount[side][KNIGHT]; ++i) {
			knightEvalResult[side] += knightBonus[knightPosArr[side][i]];
		}
	#endif
}

void ChessEngine::evalBishops(bool side) {
	bishopEvalResult[side] = 0;

	#ifdef USE_MATERIAL_VALUE
		bishopEvalResult[side] += pieceCount[side][BISHOP] * materialVals[BISHOP];

		// Bonus for having two bishops
		if (pieceCount[side][BISHOP] == 2) {
			bishopEvalResult[side] += 50;
		}
	#endif

	#ifdef USE_MATERIAL_PLACEMENT
		// Consider bishop placement
		for (U8 i(0); i<pieceCount[side][BISHOP]; ++i) {
			bishopEvalResult[side] += bishopBonus[bishopPosArr[side][i]];
		}
	#endif
}

void ChessEngine::evalRooks(bool side) {
	rookEvalResult[side] = 0;

	#ifdef USE_MATERIAL_VALUE
		rookEvalResult[side] += pieceCount[side][ROOK] * materialVals[ROOK];

		// Adjust rook value based on number of pawns
		// -12 for each pawn above 5, +12 for each below
		rookEvalResult[side] += pieceCount[side][ROOK] * ((-12*pieceCount[side][PAWN]) + 60);
	#endif
}

void ChessEngine::evalQueens(bool side) {
	queenEvalResult[side] = 0;

	#ifdef USE_MATERIAL_VALUE
		queenEvalResult[side] += pieceCount[side][QUEEN] * materialVals[QUEEN];
	#endif

	#ifdef USE_MATERIAL_PLACEMENT
		// Consider queen placement
		for (U8 i(0); i<pieceCount[side][QUEEN]; ++i) {
			queenEvalResult[side] += queenBonus[queenPosArr[side][i]];
		}
	#endif
}

void ChessEngine::evalKings(bool side) {
	kingEvalResult[side] = 0;

	#ifdef USE_MATERIAL_PLACEMENT
		// Consider king placement based on game stage
		if (gameStage <= 51) {
			kingEvalResult[side] += kingOpeningBonus[side][kingPos[side]];
		} else if (gameStage <= 102) {
			kingEvalResult[side] += kingEarlyBonus[side][kingPos[side]];
		} else if (gameStage <= 153) {
			kingEvalResult[side] += kingMidBonus[side][kingPos[side]];
		} else if (gameStage <= 204) {
			kingEvalResult[side] += kingLateBonus[side][kingPos[side]];
		} else {
			kingEvalResult[side] += kingEndBonus[side][kingPos[side]];
		}
	#endif
}

// Evaluates the safety of the king
void ChessEngine::evalKingSafety(bool side) {
	kingSafetyEvalResult[side] = 0;

	// --- Adjust for King Safty ---
	// Retain pawn protection

	// Distance from enemy pieces
}

// Evaluates a score for a single color/side
short ChessEngine::evalSide(bool side) {

	this->evalPawns(side);
	this->evalKnights(side);
	this->evalBishops(side);
	this->evalRooks(side);
	this->evalQueens(side);
	this->evalKings(side);
	this->evalKingSafety(side);

	short rating(pawnEvalResult[side]);
	rating += knightEvalResult[side];
	rating += bishopEvalResult[side];
	rating += rookEvalResult[side];
	rating += queenEvalResult[side];
	rating += kingEvalResult[side];
	rating += kingSafetyEvalResult[side];

	// --- Adjustment for Mobility ---
	// TODO

	return rating;
}

// Evaluates the state of the entire board
// A positive number indicates side player has an advantage
// A negative number indicates other player has an advantage
// The magnatude of the number represents the size of the advantage
short ChessEngine::evalBoard(ChessState* cs, bool side) {

	nodesTotal += 1;

	this->prepEval(cs);

	short rating(evalSide(side) - evalSide(!side));

	return rating;
}
