
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "board_defs.hpp"
#include "move.hpp"
#include "size_defs.hpp"
#include "uci.hpp"

// Handles "go" input commands.
// Runs engine searches based on input.
void UCI::inputGo(string input) {

	// Break down input into words
	std::vector<std::string> inVec;
	UCI::splitString(input, &inVec);	

	// "go" command variables
	std::vector<Move> searchMoves;
	U64 wTime(0);
	U64 bTime(0);
	U64 wInc(0);
	U64 bInc(0);
	S8 depth(-1);
	U64 nodes(0);
	bool infinite(false);
	U64 moveTime(-1);

	// Fetching search specifications
	short i(1);
	while (true) {
		if (i == inVec.size()) {
			break;
		}

		if (inVec[i] == "searchmoves") {

		} else if (inVec[i] == "wtime") {
			i += 1;
			wTime = max(stoi(inVec[i]) - moveOverhead, 1) * 1000;
		} else if (inVec[i] == "btime") {
			i += 1;
			bTime = max(stoi(inVec[i]) - moveOverhead, 1) * 1000;
		} else if (inVec[i] == "winc") {
			i += 1;
			wInc = stoi(inVec[i]) * 1000;
		} else if (inVec[i] == "binc") {
			i += 1;
			bInc = stoi(inVec[i]) * 1000;
		} else if (inVec[i] == "depth") {
			i += 1;
			depth = stoi(inVec[i]);
		} else if (inVec[i] == "nodes") {
			i += 1;
			nodes = stoi(inVec[i]);
		} else if (inVec[i] == "infinite") {
			infinite = true;
		} else if (inVec[i] == "movetime") {
			i += 1;
			moveTime = stoi(inVec[i]) * 1000;
		}

		i += 1;
	}

	this->engine.canSearch = true;

	Move m;
	bool continueStream(true);
	thread outputThread(&UCI::streamOutputInfo, this, &continueStream);
	if (infinite) {
		m = engine.searchInfinite(this->cs);
	} else if (depth != -1) {
		m = engine.searchDepth(this->cs, depth);
	} else if (cs.turn == WHITE) {
		m = engine.searchOnTimer(this->cs, wTime, wInc);
	} else {
		m = engine.searchOnTimer(this->cs, bTime, bInc);
	}
	this->outputInfo();

	// Discontinue output stream before printing the
	// bestmove to avoid printing at the same time
	continueStream = false;

	outputMutex.lock();
	cout << "bestmove " << m << '\n';
	outputMutex.unlock();

	outputThread.join();
}
