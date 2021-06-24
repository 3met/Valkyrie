
#include "uci.hpp"

// Prepare the chess engine for a new game.
// Resets chess state and engine variables
void UCI::inputUcinewgame() {

	this->engine.canSearch = false;		// Stop engine from searching

	this->cs.reset();
	this->engine.clear();
}
