
#include <iostream>
#include <string>
#include <vector>
#include "testing.hpp"
#include "uci.hpp"

using namespace std;

// Handles "test" input commands.
// Used for debugging.
void UCI::inputTest(string input) {

	// Parse input
	std::vector<string> inVec;
	this->splitString(input, &inVec);

	bool verbose = false;
	if (inVec.size() > 2 && inVec[2] == "verbose") {
		verbose = true;
	}

	// 
	if (inVec[1] == "perft") {
		perftTest(verbose);
	} else if (inVec[1] == "depth") {
		depthTest(verbose);
	} else {
		cout << "Unknown Test: " << inVec[1] << endl;
	}
}
