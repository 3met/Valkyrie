
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <thread>
#include "size_defs.hpp"
#include "uci.hpp"

// Splits a string into seperate words by using ' ' (space) as a break point
void UCI::splitString(string str, vector<string>* strVec) {
	size_t pos(str.find(' '));
	size_t startPos(0);

	while (pos != std::string::npos) {
		strVec->push_back(str.substr(startPos, pos-startPos));
		startPos = pos + 1;

		pos = str.find(' ', startPos);
	}
	strVec->push_back(str.substr(startPos, pos-startPos));
}

using namespace std::chrono;

// Prints search data with "info" output.
void UCI::outputInfo() {
	outputMutex.lock();
	printf("info depth %d", engine.uciDepth);
	
	if (engine.currScore.hasMate()) {
		if (engine.currScore.eval < 0) {
			printf(" score mate -%d", short(engine.currScore.halfMovesToMate())/2);
		} else {
			printf(" score mate %d", short(engine.currScore.halfMovesToMate())/2);
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

	// Print the PV if it exists
	if (!engine.pvTable[0][0].isNull()) {
		printf(" pv ");

		// Loop through moves in the PV
		for (short i(0); i<engine.pvTable.size(); ++i) {
			// Quit if move to print is NULL_MOVE meaning end of PV
			if (engine.pvTable[0][i].isNull()) {
				break;
			} else {
				cout << engine.pvTable[0][i] << ' ';
			}
		}
	}

	printf("\n");
	outputMutex.unlock();
}

// Continuously streams outputInfo
void UCI::streamOutputInfo(bool* continueStream) {
	std::this_thread::sleep_for(milliseconds(500));
	while (*continueStream) {
		this->outputInfo();
		std::this_thread::sleep_for(milliseconds(500));
	}
}

