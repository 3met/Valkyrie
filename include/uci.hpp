
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

	// UCI Commands
	void inputIsready();
	void inputUCI();
	void inputPosition(string input);
	void inputPrint();
	void inputGo(string input);
	void inputUcinewgame();

	// Miscellaneous
	static void splitString(string str, vector<string>* strVec);

public:
	UCI();
	~UCI();

	// Engine Information
	static const string ENGINE_NAME;
	static const string ENGINE_AUTHOR;

	void run();
};

#endif
