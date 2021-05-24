
#include "uci.hpp"

void UCI::inputReverse() {
	if (cs.moveList.size() > 0) {
		cs.reverseMove();
	}
}
