
#include "uci.hpp"

/* Prepare the chess engine for a new game */
void UCI::inputUcinewgame() {
	this->engine->transTable.clear();
}
