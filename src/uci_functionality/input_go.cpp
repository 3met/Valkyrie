
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "move.hpp"
#include "uci.hpp"
#include "U64.hpp"

// Handles "go" input commands.
// Runs engine searches based on input.
void UCI::inputGo(string input) {
	this->isSearching = true;

	// Break down input into words
	std::vector<std::string> inVec;
	UCI::splitString(input, &inVec);	

	// "go" command variables
	std::vector<Move> searchMoves;
	int wTime(0);
	int bTime(0);
	int wInc(0);
	int bInc(0);
	S8 depth(-1);
	U64 nodes(0);
	bool infinite(false);
	int moveTime(-1);

	// Fetching search specifications
	short i(1);
	while (true) {
		if (i == inVec.size()) {
			break;
		}

		if (inVec[i] == "searchmoves") {

		} else if (inVec[i] == "wtime") {
			i += 1;
			wTime = stoi(inVec[i]);
		} else if (inVec[i] == "btime") {
			i += 1;
			bTime = stoi(inVec[i]);
		} else if (inVec[i] == "winc") {
			i += 1;
			wInc = stoi(inVec[i]);
		} else if (inVec[i] == "binc") {
			i += 1;
			bInc = stoi(inVec[i]);
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
			moveTime = stoi(inVec[i]);
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
	} else if (cs.turn == cs.WHITE) {
		m = engine.searchOnTimer(this->cs, wTime, wInc);
	} else {
		m = engine.searchOnTimer(this->cs, bTime, bInc);
	}
	this->outputInfo();

	cout << "bestmove " << m << endl;
	this->isSearching = false;
	continueStream = false;

	cout << "info trans-table-size: " << engine.transTable.size() << endl;

	outputThread.join();
}
