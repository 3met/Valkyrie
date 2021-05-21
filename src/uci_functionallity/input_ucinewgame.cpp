
#include "uci.hpp"

/* Prepare the chess engine for a new game */
void UCI::inputUcinewgame() {
	#ifdef USE_TRANS_TABLE
	this->engine.transTable.clear();
	#endif
}
