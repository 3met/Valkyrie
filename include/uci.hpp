
#pragma once
#ifndef UCI_HPP
#define UCI_HPP

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

	// Engine and chess state for computations
	ChessState cs;
	ChessEngine engine;

	// UCI options
	vector<UciOption*> options;

	// Status variables
	bool isRunning = false;	// Whether a computation is running
	bool runPerm = true;	// Whether the object has permission execute commands

	// Engine options
	int moveOverhead;		// Connection delay (ms)

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

public:
	UCI();
	~UCI();

	// Engine Information
	static const string ENGINE_NAME;
	static const string ENGINE_AUTHOR;

	void run();
	void runCommand(string input);
};

#endif
