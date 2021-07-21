
#include <iostream>
#include <stdio.h>
#include "uci.hpp"

// Handles "uci" command.
void UCI::inputUCI() {
	cout << "id name " << this->ENGINE_NAME << '\n';
	cout << "id author " << this->ENGINE_AUTHOR << '\n';

	// Print UCI options
	if (options.size() > 0) {
		printf("\n");

		for (short i(0); i<options.size(); ++i) {
			cout << options[i]->to_str() << '\n';
		}
	}

	if (!engine.isLoaded) {
		engine.load();
	}
	printf("uciok\n");
}
