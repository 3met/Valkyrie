
#include <iostream>
#include "uci.hpp"

// Handles "reverse" command.
// Used for debugging.
void UCI::inputReverse() {
	if (cs.moveNumber > 0) {
		cs.reverseMove();
	} else {
		cout << "Cannot Reverse" << endl;
	}
}
