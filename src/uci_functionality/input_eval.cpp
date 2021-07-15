
#include <iostream>
#include <stdio.h>
#include "eval_score.hpp"
#include "uci.hpp"

// Prints static evaluation of current board
void UCI::inputEval() {
	engine.evalBoard(&cs, cs.WHITE);	

	printf("-----------------------------\n");
	printf("Game Stage: %d\n", engine.gameStage);
	printf("\n");
	printf("            \tWHITE\tBLACK\n");
	printf("Pawn:       \t%d\t%d\n", engine.pawnEvalResult[cs.WHITE], engine.pawnEvalResult[cs.BLACK]);
	printf("Knight:     \t%d\t%d\n", engine.knightEvalResult[cs.WHITE], engine.knightEvalResult[cs.BLACK]);
	printf("Bishop:     \t%d\t%d\n", engine.bishopEvalResult[cs.WHITE], engine.bishopEvalResult[cs.BLACK]);
	printf("Rook:       \t%d\t%d\n", engine.rookEvalResult[cs.WHITE], engine.rookEvalResult[cs.BLACK]);
	printf("Queen:      \t%d\t%d\n", engine.queenEvalResult[cs.WHITE], engine.queenEvalResult[cs.BLACK]);
	printf("King:       \t%d\t%d\n", engine.kingEvalResult[cs.WHITE], engine.kingEvalResult[cs.BLACK]);
	printf("King Safety:\t%d\t%d\n", engine.kingSafetyEvalResult[cs.WHITE], engine.kingSafetyEvalResult[cs.BLACK]);
	printf("\n");
	cout << "Overall: " << EvalScore(engine.evalBoard(&cs, cs.WHITE)) << endl;
	printf("-----------------------------\n");
}
