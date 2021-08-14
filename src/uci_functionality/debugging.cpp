
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "board_defs.hpp"
#include "eval_score.hpp"
#include "testing.hpp"
#include "uci.hpp"

using namespace std;

// Prints static evaluation of current board
void UCI::inputEval() {
	engine.evalBoard(&cs, WHITE);	
	outputMutex.lock();
	printf("-----------------------------\n");
	printf("Game Stage: %d\n", engine.gameStage);
	printf("\n");
	printf("            \tWHITE\tBLACK\n");
	printf("Pawn:       \t%d\t%d\n", engine.pawnEvalResult[WHITE], engine.pawnEvalResult[BLACK]);
	printf("Knight:     \t%d\t%d\n", engine.knightEvalResult[WHITE], engine.knightEvalResult[BLACK]);
	printf("Bishop:     \t%d\t%d\n", engine.bishopEvalResult[WHITE], engine.bishopEvalResult[BLACK]);
	printf("Rook:       \t%d\t%d\n", engine.rookEvalResult[WHITE], engine.rookEvalResult[BLACK]);
	printf("Queen:      \t%d\t%d\n", engine.queenEvalResult[WHITE], engine.queenEvalResult[BLACK]);
	printf("King:       \t%d\t%d\n", engine.kingEvalResult[WHITE], engine.kingEvalResult[BLACK]);
	printf("King Safety:\t%d\t%d\n", engine.kingSafetyEvalResult[WHITE], engine.kingSafetyEvalResult[BLACK]);
	printf("\n");
	cout << "Overall: " << EvalScore(engine.evalBoard(&cs, WHITE)) << '\n';
	printf("-----------------------------\n");
	outputMutex.unlock();
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
		outputMutex.lock();
		cout << "Unknown Test: " << inVec[1] << '\n';
		outputMutex.unlock();
	}
}

// Clears the console.
void UCI::inputClear() {
	outputMutex.lock();
	#ifdef WINDOWS
		system("cls");
	#elif defined LINUX
		system("clear");
	#endif
	outputMutex.unlock();
}
