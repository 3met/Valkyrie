
#include <stdio.h>
#include "uci.hpp"

// Handles "isready" input command.
// Attempts to ready the engine if the engine is not already ready.
void UCI::inputIsready() {
	if (!engine.isLoaded) {
		engine.load();
	}
	
	// Ensure engine is loaded
	if (engine.isLoaded && this->runPerm) {
		printf("readyok\n");
	} else {
		printf("notready\n");
	}
}
