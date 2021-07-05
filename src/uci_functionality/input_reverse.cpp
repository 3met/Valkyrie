
#include "uci.hpp"

// Handles "reverse" command.
// Used for debugging.
void UCI::inputReverse() {
	if (cs.moveNumber > 1) {
		cs.reverseMove();
	}
}
