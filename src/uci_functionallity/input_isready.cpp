
#include "uci.hpp"

void UCI::inputIsready() {
	if (!engine.isLoaded) {
		engine.load();
	}
	cout << "readyok" << endl;
}
