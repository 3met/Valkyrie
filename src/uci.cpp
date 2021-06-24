
#include <future>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include "uci.hpp"

using namespace std;

UCI::UCI() {};

UCI::~UCI() {};

// --- Static Members ---
const string UCI::ENGINE_NAME = "Chess Engine v1";
const string UCI::ENGINE_AUTHOR = "Emet Behrendt";

// --- Main Methods ---
// Runs command associated with input if such a command exists.
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
	} else if (input == "reverse") {
		this->inputReverse();
	} else if (input == "eval") {
		this->inputEval();
	} else if (input.substr(0, 6) == "perft ") {
		this->inputPerft(input);
	} else if (input.substr(0, 7) == "divide ") {
		this->inputDivide(input);
	} else if (input.substr(0, 5) == "test ") {
		this->inputTest(input);
	} else if (input == "print") {
		this->inputPrint();
	} else if (input == "clear" || input == "cls") {
		this->inputClear();
	} else {
		cout << "Unknown Command: " << input << endl;
	}
}

// Deals with all UCI communication
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
