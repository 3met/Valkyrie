
#include <string>
#include <vector>
#include "size_defs.hpp"
#include "uci.hpp"

// For UCI commands that start with "perft"
void UCI::inputPerft(string input) {
	// Parse input
	std::vector<string> inVec;
	this->splitString(input, &inVec);
	
	U64 n(this->engine.perft(&cs, stoi(inVec[1])));

	outputMutex.lock();
	cout << this->engine.perft(&cs, stoi(inVec[1])) << '\n';
	outputMutex.unlock();
}
