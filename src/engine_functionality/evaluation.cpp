
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
// 0   - 31  ==> opening
// 32  - 63  ==> late opening
// 64  - 95  ==> early game
// 96  - 127 ==> late early game
// 128 - 159 ==> mid game
// 160 - 191 ==> late mid game
// 192 - 223 ==> end game
// 224 - 255 ==> late end game
void ChessEngine::setGameStage() {
	gameStage = -(pieceCount[0][PAWN] << 1);
	gameStage -= pieceCount[1][PAWN] << 1;
	gameStage -= pieceCount[0][KNIGHT] * 12;
	gameStage -= pieceCount[1][KNIGHT] * 12;
	gameStage -= pieceCount[0][BISHOP] * 12;
	gameStage -= pieceCount[1][BISHOP] * 12;
	gameStage -= pieceCount[0][ROOK] * 18;
	gameStage -= pieceCount[1][ROOK] * 18;
	gameStage -= pieceCount[0][QUEEN] * 28;
	gameStage -= pieceCount[1][QUEEN] * 28;

	// Consider pawn placement based on game stage
	if (gameStage < 32) {
		gameStage = OPENING;
	} else if (gameStage < 64) {
		gameStage = LATE_OPENING;
	} else if (gameStage < 96) {
		gameStage = EARLY_GAME;
	} else if (gameStage < 128) {
		gameStage = LATE_EARLY_GAME;
	} else if (gameStage < 160) {
		gameStage = MID_GAME;
	} else if (gameStage < 192) {
		gameStage = LATE_MID_GAME;
	} else if (gameStage < 224) {
		gameStage = END_GAME;
	} else {
		gameStage = LATE_END_GAME;
	}
}

// Prepare the object for evaluation.
// Must be called before any evaluation method other
// than ChessEngine::evalBoard()
void ChessEngine::prepEval(ChessState* cs) {
	// Positions of all the pieces
	pieceCount[0][PAWN] = cs->pieces[0][PAWN].getPosArr(pawnPosArr[0]);
	pieceCount[0][KNIGHT] = cs->pieces[0][KNIGHT].getPosArr(knightPosArr[0]);
	pieceCount[0][BISHOP] = cs->pieces[0][BISHOP].getPosArr(bishopPosArr[0]);
	pieceCount[0][ROOK] = cs->pieces[0][ROOK].getPosArr(rookPosArr[0]);
	pieceCount[0][QUEEN] = cs->pieces[0][QUEEN].getPosArr(queenPosArr[0]);
	kingPos[0] = cs->pieces[0][KING].getFirstPos();
	pieceCount[1][PAWN] = cs->pieces[1][PAWN].getPosArr(pawnPosArr[1]);
	pieceCount[1][KNIGHT] = cs->pieces[1][KNIGHT].getPosArr(knightPosArr[1]);
	pieceCount[1][BISHOP] = cs->pieces[1][BISHOP].getPosArr(bishopPosArr[1]);
	pieceCount[1][ROOK] = cs->pieces[1][ROOK].getPosArr(rookPosArr[1]);
	pieceCount[1][QUEEN] = cs->pieces[1][QUEEN].getPosArr(queenPosArr[1]);
	kingPos[1] = cs->pieces[1][KING].getFirstPos();

	setGameStage();
}

void ChessEngine::evalPawns(bool side) {
	
	// Penalty for having no pawns
	if (pieceCount[side][PAWN] == 0) {
		pawnEvalResult[side] = -50;
		return;
	}

	pawnEvalResult[side] = 0;

	#ifdef USE_MATERIAL_VALUE
		pawnEvalResult[side] += pieceCount[side][PAWN] * materialVals[PAWN];
	#endif

	#ifdef USE_MATERIAL_PLACEMENT
		// --- Adjustment for Pawn Structure ---
		U8 pawnsPerRank[2][8] = {{0},{0}};	// colors * ranks
		U8 pawnsPerFile[2][8] = {{0},{0}};	// colors * files
		U8 i;
		// Loop through friendly pawn positions
		for (i=0; i<pieceCount[side][PAWN]; ++i) {
			pawnsPerRank[side][BOARD_RANK[pawnPosArr[side][i]]] += 1;
			pawnsPerFile[side][BOARD_FILE[pawnPosArr[side][i]]] += 1;

			// Consider pawn placement based on game stage
			pawnEvalResult[side] += pawnBonus[gameStage][side][pawnPosArr[side][i]];
		}
		// Loop through enemy pawn positions
		for (i=0; i<pieceCount[!side][PAWN]; ++i) {
			pawnsPerRank[!side][BOARD_RANK[pawnPosArr[!side][i]]] += 1;
			pawnsPerFile[!side][BOARD_FILE[pawnPosArr[!side][i]]] += 1;
		}

		#ifdef USE_DOUBLED_PAWNS
			// Penalize doubled pawns
			// Subtract 40 centipawns for each doubled/tripled pawn
			// Subtract 45 if on the edge
			// Seems to run much faster without a loop
			if (pawnsPerFile[side][0] > 1)
				pawnEvalResult[side] -= (pawnsPerFile[side][0]-1) * 45;
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
				pawnEvalResult[side] -= (pawnsPerFile[side][7]-1) * 45;

			// TODO: types of doubled pawns
			// https://en.wikipedia.org/wiki/Chess_piece_relative_value
		#endif

		#ifdef USE_ISOLATED_PAWNS
			// Substract 15 centipawns for each edge isolated pawn
			// Substract 10 centipawns for each non-edge isolated pawn
			// a file
			if (pawnsPerFile[side][1] == 0)
				pawnEvalResult[side] -= 15 * pawnsPerFile[side][0];
			// b file
			if (pawnsPerFile[side][0] == 0 && pawnsPerFile[side][2] == 0)
				pawnEvalResult[side] -= 10 * pawnsPerFile[side][1];
			// c file
			if (pawnsPerFile[side][1] == 0 && pawnsPerFile[side][3] == 0)
				pawnEvalResult[side] -= 10 * pawnsPerFile[side][2];
			// d file
			if (pawnsPerFile[side][2] == 0 && pawnsPerFile[side][4] == 0)
				pawnEvalResult[side] -= 10 * pawnsPerFile[side][3];
			// e file
			if (pawnsPerFile[side][3] == 0 && pawnsPerFile[side][5] == 0)
				pawnEvalResult[side] -= 10 * pawnsPerFile[side][4];
			// f file
			if (pawnsPerFile[side][4] == 0 && pawnsPerFile[side][6] == 0)
				pawnEvalResult[side] -= 10 * pawnsPerFile[side][5];
			// g file
			if (pawnsPerFile[side][5] == 0 && pawnsPerFile[side][7] == 0)
				pawnEvalResult[side] -= 10 * pawnsPerFile[side][6];
			// h file
			if (pawnsPerFile[side][6] == 0)
				pawnEvalResult[side] -= 15 * pawnsPerFile[side][7];
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

		// --- Small bonus for pinning or attacking enemy king ---
		// Only applies from midgame
		if (gameStage >= MID_GAME) {
			// One bishops on side
			if (pieceCount[side][BISHOP] == 1) {
				// Check same diagonal
				if (POSITIVE_DIAG[bishopPosArr[side][0]] == POSITIVE_DIAG[kingPos[!side]]
					|| NEGATIVE_DIAG[bishopPosArr[side][0]] == NEGATIVE_DIAG[kingPos[!side]]) {

					bishopEvalResult[side] += 10;
				}

			// Two bishops on side
			} else if (pieceCount[side][BISHOP] == 2) {
				// Check same diagonal
				if (POSITIVE_DIAG[bishopPosArr[side][0]] == POSITIVE_DIAG[kingPos[!side]]
					|| NEGATIVE_DIAG[bishopPosArr[side][0]] == NEGATIVE_DIAG[kingPos[!side]]) {

					bishopEvalResult[side] += 10;
				}
				if (POSITIVE_DIAG[bishopPosArr[side][1]] == POSITIVE_DIAG[kingPos[!side]]
					|| NEGATIVE_DIAG[bishopPosArr[side][1]] == NEGATIVE_DIAG[kingPos[!side]]) {

					bishopEvalResult[side] += 10;
				}
			}
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

	#ifdef USE_MATERIAL_PLACEMENT

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

		kingEvalResult[side] += kingBonus[gameStage][side][kingPos[side]];
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

	return pawnEvalResult[side] + knightEvalResult[side] + bishopEvalResult[side]
		+ rookEvalResult[side] + queenEvalResult[side] + kingEvalResult[side]
		+ kingSafetyEvalResult[side];
}

// Evaluates the state of the entire board
// A positive number indicates side player has an advantage
// A negative number indicates other player has an advantage
// The magnatude of the number represents the size of the advantage
short ChessEngine::evalBoard(ChessState* cs, bool side) {

	nodesTotal += 1;

	this->prepEval(cs);

	return evalSide(side) - evalSide(!side);
}
