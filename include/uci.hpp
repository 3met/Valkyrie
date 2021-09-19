
#pragma once
#ifndef UCI_HPP
#define UCI_HPP

#include <chrono>
#include <future>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"

using namespace std;

// Stores universal chess interface (UCI) data and
// runs ChessEngine through UCI commands.
class UCI {
private:
	// Stores a UCI option
	class UciOption {
	protected:
		string name;

	public:
		UciOption(string _name="") {
			name = _name;
		};

		virtual ~UciOption() {};

		virtual string to_str() = 0;
	};

	// Spin option for UCI
	class UciSpinOption : public UciOption {
	private:
		int defaultValue;
		int min;
		int max;

	public:
		UciSpinOption(string _name, int _defaultValue, int _min, int _max) : UciOption() {
			name = _name;
			defaultValue = _defaultValue;
			min = _min;
			max = _max;
		};

		string to_str() {
			return "option name " + name + " type spin default "
				+ to_string(defaultValue) + " min " + to_string(min)
				+ " max " + to_string(max);
		}
	};

	// Button Option for UCI
	class UciButtonOption : public UciOption {
	public:
		UciButtonOption(string _name) {
			name = _name;
		};

		string to_str() {
			return "option name " + name + " type button";
		}
	};

	// Check Option for UCI
	class UciCheckOption : public UciOption {
	private:
		bool defaultValue;
	public:
		UciCheckOption(string _name, bool _defaultValue) {
			name = _name;
			defaultValue = _defaultValue;
		};

		string to_str() {
			string str = "option name " + name + " type check default ";
			if (defaultValue) {
				return str + "true";
			} else {
				return str + "false";
			}
		}
	};

	// String Option for UCI
	class UciStringOption : public UciOption {
	private:
		string defaultValue;
	public:
		UciStringOption(string _name, string _defaultValue) {
			name = _name;
			defaultValue = _defaultValue;
		};

		string to_str() {
			return "option name " + name + " type string default " + defaultValue;
		}
	};

	class NoParallelCommand {
	public:
		void (UCI::*regMethod)();
		void (UCI::*stringMethod)(string);
		string input;
		NoParallelCommand(void (UCI::*_method)()) {
			regMethod = _method;
			input = "";
		};
		NoParallelCommand(void (UCI::*_method)(string), string _input) {
			stringMethod = _method;
			input = _input;
		};
	};

	// Calculation Commands
	void inputDivide(string input);
	void inputGo(string input);
	void inputPerft(string input);
	void inputStop();

	// Set State Commands
	void inputMove(string input);
	void inputPosition(string input);
	void inputSetOption(string input);
	void inputReverse();
	void inputUcinewgame();

	// Output Commands
	void inputIsready();
	void inputUCI();

	// Debugging Commands
	void inputClear();
	void inputTest(string input);
	void inputPrint();
	void inputEval();

	// Miscellaneous
	void outputInfo();
	void streamOutputInfo(bool* continueStream);
	static void splitString(string str, vector<string>* strVec);

	// Engine and chess state for computations
	ChessState cs;
	ChessEngine engine;

	// UCI options
	vector<UciOption*> options;

	// Engine options
	int moveOverhead;		// Connection delay (ms)

	// Runtime Config
	static const chrono::microseconds queueWaitTime;
	// Runtime Variables
	bool runPerm = true;	// Whether the object has permission execute commands
	bool noParallelRunning = false;	// Whether a "no parallel" computation is running
	// Command queues and information
	queue<future<void>> parallelFutures;
	future<void> noParallelFuture;
	bool noParallelFutureNull = true;
	queue<NoParallelCommand> noParallelQueue;
	// Handling commands at runtime
	void noParallelManager();

	// Output Mutex
	mutex outputMutex;

public:
	UCI();
	~UCI();

	// Engine Information
	static const string ENGINE_NAME;
	static const string ENGINE_AUTHOR;

	void run();
};

#endif
