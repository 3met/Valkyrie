
#include <future>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include "uci.hpp"

#include <chrono>	// TEMP
using namespace std::chrono;
high_resolution_clock::time_point moveStart, moveEnd;
long long int moveTotal = 0;
high_resolution_clock::time_point evalStart, evalEnd;
long long int evalTotal = 0;

using namespace std;

UCI::UCI() {};

UCI::~UCI() {};

// --- Static Members ---
const string UCI::ENGINE_NAME = "Chess Engine v1";
const string UCI::ENGINE_AUTHOR = "Emet Behrendt";

// --- Main Methods ---
void UCI::runCommand(string input) {
	if (input == "stop") {
		engine.canSearch = false;
	} else if (input.substr(0, 2) == "go") {
		this->inputGo(input);
	} else if (input.substr(0, 8) == "position") {
		this->inputPosition(input);
	} else if (input == "ucinewgame") {
		this->inputUcinewgame();
	} else if (input == "uci") {
		this->inputUCI();
	} else if (input == "isready") {
		this->inputIsready();
	} else if (input.substr(0, 5) == "move ") {
		this->inputMove(input);
	} else if (input.substr(0, 6) == "perft ") {
		this->inputPerft(input);
	} else if (input.substr(0, 7) == "divide ") {
		this->inputDivide(input);
	} else if (input == "print") {
		this->inputPrint();
		cout << "MOVE: " << moveTotal << endl;
		cout << "EVAL: " << evalTotal << endl;
	} else if (input == "clear" || input == "cls") {
		this->inputClear();
	} else {
		cout << "Unknown Command: " << input << endl;
	}
}

/* Deals with all UCI communication */
void UCI::run() {
	string input;

	vector<future<void>> futures;

	while (true) {
		getline(cin, input);

		if (input == "quit") {
			break;
		}

		futures.push_back(async(&UCI::runCommand, this, input));
	}
}


// void UCI::run() {
// 	string input;
// 	thread* thEngine;

// 	while (true) {
// 		getline(cin, input);

// 		if (input == "quit") {
// 			engine.canSearch = false;
// 			thEngine->join();
// 			break;
// 		}

// 		thEngine = new thread(&UCI::runCommand, this, input);
// 	}
// }
