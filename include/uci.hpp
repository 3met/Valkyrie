
#pragma once
#ifndef UCI_HPP
#define UCI_HPP

#include <string>
#include <vector>
#include "chess_engine.hpp"
#include "chess_engine.hpp"

using namespace std;

class UCI {
private:
	ChessState cs;
	ChessEngine engine;

	// --- UCI Commands ---
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
	void inputPrint();
	void inputEval();

	// Miscellaneous
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
