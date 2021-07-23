
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "eval_score.hpp"
#include "testing.hpp"
#include "uci.hpp"

using namespace std;

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
	cout << "Overall: " << EvalScore(engine.evalBoard(&cs, cs.WHITE)) << '\n';
	printf("-----------------------------\n");
}

// Prints the current chess state.
void UCI::inputPrint() {
	cs.show();
}

// Handles "test" input commands.
// Used for debugging.
void UCI::inputTest(string input) {

	// Parse input
	std::vector<string> inVec;
	this->splitString(input, &inVec);

	bool verbose(false);
	if (inVec.size() > 2 && inVec[2] == "verbose") {
		verbose = true;
	}

	if (inVec[1] == "perft") {
		perftTest(verbose);
	} else if (inVec[1] == "depth") {
		depthTest(verbose);
	} else if (inVec[1] == "ebf" || inVec[1] == "EBF") {
		EBF_Test(verbose);
	} else {
		cout << "Unknown Test: " << inVec[1] << '\n';
	}
}

// Clears the console.
void UCI::inputClear() {
	#ifdef WINDOWS
		system("cls");
	#elif defined LINUX
		system("clear");
	#endif
}
