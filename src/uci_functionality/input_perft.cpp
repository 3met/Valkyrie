
#include <string>
#include <vector>
#include "uci.hpp"

void UCI::inputPerft(string input) {

	std::vector<string> inVec;
	this->splitString(input, &inVec);

	cout << this->engine.perft(&cs, stoi(inVec[1])) << endl;
}
