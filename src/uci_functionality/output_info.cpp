
#include <chrono>
#include <iostream>
#include <thread>
#include "UCI.hpp"

using namespace std::chrono;

void UCI::outputInfo() {
	cout << "info depth " << engine.currDepth;
	
	if (engine.currScore.foundMate) {
		cout << " score mate " << short(engine.currScore.movesToMate) * engine.currScore.eval;
	} else {
		cout << " score cp " << engine.currScore;
	}

	cout << " nodes " << engine.nodesTotal;

	int time = duration_cast<milliseconds>(high_resolution_clock::now() - engine.startTime).count();
	if (time != 0) {
		cout << " time " << time
			 << " nps " << ((engine.nodesTotal * 1000) / time);
	}
	cout << endl;
}

void UCI::streamOutputInfo(bool* continueStream) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	while (*continueStream) {
		this->outputInfo();
	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
