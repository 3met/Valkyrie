
#include <iostream>
#include <thread>
#include "move.hpp"
#include "uci.hpp"

void UCI::inputGo(string input) {

	this->engine.canSearch = true;

	Move m = this->engine.searchOnTimer(this->cs, 10000, 1000);

	cout << "bestmove " << m << endl;
}
