
#include "uci.hpp"

// Handles "isready" input command.
// Attempts to ready the engine if the engine is not already ready.
void UCI::inputIsready() {
	if (!engine.isLoaded) {
		engine.load();
	}
	
	if (engine.isLoaded && !this->isSearching) {
		cout << "readyok" << endl;
	} else {
		cout << "notready" << endl;
	}
}
