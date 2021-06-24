
#include <string>
#include <vector>
#include "uci.hpp"
#include "U8.hpp"

/* For UCI commands starting with "position"
   Assumes input contains the full command starting
   with "position" */
void UCI::inputPosition(string input) {
	// Break down input into words
	std::vector<std::string> inVec;
	UCI::splitString(input, &inVec);

	short pos(2);

	// Set position
	if (inVec[1] == "fen") {
		string fen;
		while (pos != inVec.size() && inVec[pos] != "moves") {
			fen += inVec[pos] + ' ';
			pos += 1;
		}
		fen.erase(fen.size()-1, 1);	// Erase last space character
		this->cs.loadFEN(fen);
	} else if (inVec[1] == "startpos") {
		this->cs.reset();
	}

	// Make moves if listed
	if (pos != inVec.size() && inVec[pos] == "moves") {
		pos += 1;

		while (pos != inVec.size()) {
			cs.move(cs.notationToMove(inVec[pos]));
			pos += 1;
		}		
	}
}
