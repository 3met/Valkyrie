
#include "uci.hpp"

// Handles "reverse" command.
// Used for debugging.
void UCI::inputReverse() {
	if (cs.moveList.size() > 0) {
		cs.reverseMove();
	}
}
