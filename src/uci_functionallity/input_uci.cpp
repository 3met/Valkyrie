
#include "uci.hpp"

void UCI::inputUCI() {
	cout << "id name " << this->ENGINE_NAME << endl;
	cout << "id author " << this->ENGINE_AUTHOR << endl;
	if (!engine.isLoaded) {
		engine.load();
	}
	cout << "uciok" << endl;
}
