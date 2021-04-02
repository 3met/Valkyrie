
#include "state_tree.hpp"

bool StateTree::has(ChessState cs) {
	return (states.count(cs) != 0);
}

Move StateTree::get(ChessState cs) {
	return states[cs];
}

void StateTree::add(ChessState cs, Move m) {
	states[cs] = m;
}
