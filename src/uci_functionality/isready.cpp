
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
		outputMutex.lock();
		printf("readyok\n");
		outputMutex.unlock();
	} else {
		outputMutex.lock();
		printf("notready\n");
		outputMutex.unlock();
	}
	
}
