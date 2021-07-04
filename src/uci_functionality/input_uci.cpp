
#include "uci.hpp"

// Handles "uci" command.
void UCI::inputUCI() {
	cout << "id name " << this->ENGINE_NAME << endl;
	cout << "id author " << this->ENGINE_AUTHOR << endl;

	// Print UCI options
	if (options.size() > 0) {
		cout << endl;

		for (short i(0); i<options.size(); ++i) {
			cout << options[i]->to_str() << endl;
		}
	}

	if (!engine.isLoaded) {
		engine.load();
	}
	cout << "uciok" << endl;
}
