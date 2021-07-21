
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <thread>
#include "size_defs.hpp"
#include "uci.hpp"

using namespace std::chrono;

// Prints search data with "info" output.
void UCI::outputInfo() {
	printf("info depth %d", engine.currDepth);
	
	if (engine.currScore.hasMate()) {
		if (engine.currScore.eval < 0) {
			printf(" score mate -%d\n", short(engine.currScore.halfMovesToMate())/2);
		} else {
			printf(" score mate %d\n", short(engine.currScore.halfMovesToMate())/2);
		}
	} else {
		printf(" score cp %d", engine.currScore.eval);
	}

	cout << " nodes " << engine.nodesTotal;

	U64 time(duration_cast<microseconds>(high_resolution_clock::now() - engine.startTime).count());
	if (time != 0) {
		cout << " time " << (time / 1000)
			 << " nps " << ((engine.nodesTotal * 1000000) / time);
	}
	printf("\n");
}

// Continuously streams outputInfo
void UCI::streamOutputInfo(bool* continueStream) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	while (*continueStream) {
		this->outputInfo();
	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
