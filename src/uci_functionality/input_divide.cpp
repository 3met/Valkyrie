
#include <string>
#include <vector>
#include "uci.hpp"

void UCI::inputDivide(string input) {

	std::vector<string> inVec;
	this->splitString(input, &inVec);

	this->engine.divide(&cs, stoi(inVec[1]));
}
