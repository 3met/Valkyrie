
#include "uci.hpp"

/* Prepare the chess engine for a new game */
void UCI::inputUcinewgame() {

	cs.reset();

	this->engine.canSearch = false;

	this->engine.transTable.clear();
}
