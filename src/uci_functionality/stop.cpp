
#include "uci.hpp"

// Handles "stop" input command.
// Attempts to stop chess engine from searching.
void UCI::inputStop() {
	engine.canSearch = false;
}
