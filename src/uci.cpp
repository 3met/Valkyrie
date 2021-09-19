
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
	options.push_back(new UciStringOption("UCI_EngineAbout", "Valkyrie, By Emet Behrendt"));

	// Apply default settings
	engine.transTable->resize(defaultHash * 1000000);
	this->moveOverhead = moveOverhead;
	engine.useOwnBook = defaultOwnBook;
};

UCI::~UCI() {
	// Free memory option storage
	for (size_t i(0); i<options.size(); ++i) {
		delete options[i];
	}
};

// --- Static Members ---
const string UCI::ENGINE_NAME("Valkyrie");
const string UCI::ENGINE_AUTHOR("Emet Behrendt");

const chrono::microseconds UCI::queueWaitTime(1500);


// --- Main Methods ---

void UCI::noParallelManager() {

	while (runPerm) {		
		// Checks if there are commands to run
		if (!noParallelQueue.empty()) {
			// Checks if any "no parallel" command is currently running
			if (noParallelFutureNull || (noParallelFuture.wait_for(chrono::seconds(0)) == future_status::ready)) {
				noParallelFutureNull = false;
				if (noParallelQueue.front().input == "") {
					noParallelFuture = async(noParallelQueue.front().regMethod, this);
				} else {
					noParallelFuture = async(noParallelQueue.front().stringMethod,
						this, noParallelQueue.front().input);
				}
				noParallelQueue.pop();
			}
		}
		
		this_thread::sleep_for(queueWaitTime);
	}
}

// Deals with all UCI communication
void UCI::run() {

	// Reset Variables
	runPerm = true;
	string input;

	// Lauch noParallelManager
	thread noParaMan(&UCI::noParallelManager, this);

	while (true) {
		getline(cin, input);

		if (input == "quit") {

			engine.canSearch = false;
			runPerm = false;

			noParaMan.join();

			// Wait for anything running to finish
			chrono::milliseconds span(1);
			while (true) {
				if (!parallelFutures.empty()) {
					if (parallelFutures.front().wait_for(span) != future_status::timeout) {
					
						parallelFutures.pop();
					}
				} else {
					break;
				}
			}

			// Wait for "no parallel" commands to finish
			if (!noParallelFutureNull) {
				while (noParallelFuture.wait_for(chrono::seconds(0)) != future_status::ready) {
					
					this_thread::sleep_for(span);
				}
			}

			break;
		}

		// Runs command associated with input if such a command exists.
		if (input == "stop") {
			engine.canSearch = false;
		} else if (input.substr(0, 2) == "go") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputGo, input));
		} else if (input.substr(0, 8) == "position") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputPosition, input));
		} else if (input == "ucinewgame") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputUcinewgame));
		} else if (input == "uci") {
			parallelFutures.push(async(&UCI::inputUCI, this));
		} else if (input == "isready") {
			parallelFutures.push(async(&UCI::inputIsready, this));
		} else if (input.substr(0, 10) == "setoption ") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputSetOption, input));
		} else if (input.substr(0, 5) == "move ") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputMove, input));
		} else if (input == "reverse") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputReverse));
		} else if (input == "eval") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputEval));
		} else if (input.substr(0, 6) == "perft ") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputPerft, input));
		} else if (input.substr(0, 7) == "divide ") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputDivide, input));
		} else if (input.substr(0, 5) == "test ") {
			noParallelQueue.push(NoParallelCommand(&UCI::inputTest, input));
		} else if (input == "print") {
			parallelFutures.push(async(&UCI::inputPrint, this));
		} else if (input == "clear" || input == "cls") {
			parallelFutures.push(async(&UCI::inputClear, this));
		} else {
			outputMutex.lock();
			cout << "Unknown Command: " << input << '\n';
			outputMutex.unlock();
		}

		// Delete old futures
		while (true) {
			if (!parallelFutures.empty()
				&& parallelFutures.front().wait_for(chrono::milliseconds(0)) == future_status::ready) {
				
				parallelFutures.pop();
			} else {
				break;
			}
		}
	}
}
