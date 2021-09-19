	
#include "uci.hpp"

// Makes move in ChessState.
// Used for debugging.
void UCI::inputMove(string input) {
	// Break down input into words
	std::vector<std::string> inVec;
	UCI::splitString(input, &inVec);

	size_t pos(1);
	while (pos != inVec.size()) {
		cs.move(cs.notationToMove(inVec[pos]));
		pos += 1;
	}
}
