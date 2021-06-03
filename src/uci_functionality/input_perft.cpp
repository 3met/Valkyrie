
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

		auto start = high_resolution_clock::now();

		bool passed = perftTest();

		if (passed) {
			cout << "PASSED" << endl;
		} else {
			cout << "FAILED" << endl;
		}

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "Duration: " << duration.count() << endl;
	} else {
		cout << this->engine.perft(&cs, stoi(inVec[1])) << endl;
	}
}
