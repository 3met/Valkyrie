
#include "uci.hpp"

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
