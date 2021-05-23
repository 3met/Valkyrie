
#include "uci.hpp"

/* Prepare the chess engine for a new game */
void UCI::inputUcinewgame() {

	cs.reset();

	this->engine.canSearch = false;

	#ifdef USE_TRANS_TABLE
	this->engine.transTable.clear();
	#endif
}
