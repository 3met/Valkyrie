
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "move.hpp"
#include "uci.hpp"
#include "U64.hpp"

void UCI::inputGo(string input) {
	// Break down input into words
	std::vector<std::string> inVec;
	UCI::splitString(input, &inVec);	

	//
	std::vector<Move> searchMoves;
	int wTime = -1;
	int bTime = -1;
	int wInc = -1;
	int bInc = -1;
	S8 depth = -1;
	U64 nodes = 0;
	bool infinite = false;
	int moveTime = -1;

	short i = 1;
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
	if (infinite) {
		m = engine.searchInfinite(this->cs);
	} else if (cs.turn == cs.WHITE) {
		m = engine.searchOnTimer(this->cs, wTime, wInc);
	} else {
		m = engine.searchOnTimer(this->cs, bTime, bInc);
	}
	cout << "bestmove " << m << endl;
}
