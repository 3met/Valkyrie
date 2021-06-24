
#include <string>
#include <vector>
#include "uci.hpp"

// For UCI commands that start with "perft"
void UCI::inputPerft(string input) {
	// Parse input
	std::vector<string> inVec;
	this->splitString(input, &inVec);
	
	cout << this->engine.perft(&cs, stoi(inVec[1])) << endl;
}
