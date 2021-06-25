
#pragma once
#ifndef UCI_HPP
#define UCI_HPP

#include <string>
#include <vector>
#include "chess_engine.hpp"

using namespace std;

// Stores universal chess interface (UCI) data and
// runs ChessEngine through UCI commands.
class UCI {
private:
	ChessState cs;
	ChessEngine engine;

	bool isRunning = false;
	bool runPerm = true;	// Whether the object has permission to run

	// Calculation Commands
	void inputDivide(string input);
	void inputGo(string input);
	void inputPerft(string input);
	void inputStop();

	// Set State Commands
	void inputMove(string input);
	void inputPosition(string input);
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
