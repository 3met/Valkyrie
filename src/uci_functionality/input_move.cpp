	
#include "uci.hpp"

/* Makes move in ChessState */
void UCI::inputMove(string input) {
	// Break down input into words
	std::vector<std::string> inVec;
	UCI::splitString(input, &inVec);

	cs.move(cs.notationToMove(inVec[1]));
}
