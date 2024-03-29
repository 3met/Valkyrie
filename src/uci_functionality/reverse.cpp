
#include <iostream>
#include "uci.hpp"

// Handles "reverse" command.
// Used for debugging.
void UCI::inputReverse() {
	if (cs.moveNumber > 1) {
		cs.reverseMove();
	} else {
		outputMutex.lock();
		printf("ERROR: Nothing to reverse.\n");
		outputMutex.unlock();
	}
}
