
#include <string>
#include <vector>
#include "testing.hpp"
#include "uci.hpp"

void UCI::inputTest(string input) {

	// Parse input
	std::vector<string> inVec;
	this->splitString(input, &inVec);

	bool verbose = false;

	if (inVec.size() > 1 && inVec[2] == "verbose") {
		verbose = true;
	}

	if (inVec[1] == "perft") {
		perftTest(verbose);
	}

}
