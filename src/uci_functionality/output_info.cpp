
#include <chrono>
#include <iostream>
#include <thread>
#include "size_defs.hpp"
#include "uci.hpp"

using namespace std::chrono;

// Prints search data with "info" output.
void UCI::outputInfo() {
	cout << "info depth " << engine.currDepth;
	
	if (engine.currScore.hasMate()) {
		if (engine.currScore.eval < 0) {
			cout << " score mate -" << short(engine.currScore.halfMovesToMate())/2;
		} else {
			cout << " score mate " << short(engine.currScore.halfMovesToMate())/2;
		}
	} else {
		cout << " score cp " << engine.currScore.eval;
	}

	cout << " nodes " << engine.nodesTotal;

	U64 time(duration_cast<microseconds>(high_resolution_clock::now() - engine.startTime).count());
	if (time != 0) {
		cout << " time " << (time / 1000)
			 << " nps " << ((engine.nodesTotal * 1000000) / time);
	}
	cout << endl;
}

// Continuously streams outputInfo
void UCI::streamOutputInfo(bool* continueStream) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	while (*continueStream) {
		this->outputInfo();
	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
