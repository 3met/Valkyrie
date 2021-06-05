
#include <chrono>
#include <string>
#include <vector>
#include "testing.hpp"
#include "uci.hpp"

using namespace std::chrono;

/* For UCI commands that start with "perft" */
void UCI::inputPerft(string input) {

	// Parse input
	std::vector<string> inVec;
	this->splitString(input, &inVec);

	if (inVec[1] == "test") {

		perftTest(true);

	} else {
		cout << this->engine.perft(&cs, stoi(inVec[1])) << endl;
	}
}
