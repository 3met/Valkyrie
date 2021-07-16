
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include "uci.hpp"

using namespace std;

UCI::UCI() {
	// Set UCI variables
	int defaultHash = 64;
	moveOverhead = 10;
	int defaultThreads = 1;
	bool defaultOwnBook = true;

	// Create UCI options
	// Note: options match those in UCI::setOption()
	options.push_back(new UciSpinOption("Hash", defaultHash, 1, 2048));
	options.push_back(new UciButtonOption("Clear Hash"));
	options.push_back(new UciSpinOption("Move Overhead", moveOverhead, 0, 10000));
	options.push_back(new UciSpinOption("Threads", defaultThreads, 1, 1));
	options.push_back(new UciCheckOption("OwnBook", defaultOwnBook));

	// Apply default settings
	engine.transTable->resize(defaultHash * 1000000);
	this->moveOverhead = moveOverhead;
	engine.useOwnBook = defaultOwnBook;
};

UCI::~UCI() {
	// Free memory option storage
	for (int i(0); i<options.size(); ++i) {
		delete options[i];
	}
};

// --- Static Members ---
const string UCI::ENGINE_NAME("Chess Engine v1");
const string UCI::ENGINE_AUTHOR("Emet Behrendt");

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
	} else if (input.substr(0, 10) == "setoption ") {
		this->inputSetOption(input);
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
			engine.canSearch = false;
			runPerm = false;

			// Wait for anything running to finish
			chrono::milliseconds span(25);
			for (int i=0; i<futures.size(); ++i) {
				while (futures[i].wait_for(span) == std::future_status::timeout) {}
			}

			break;
		}

		futures.push_back(async(&UCI::runCommand, this, input));
	}
}
