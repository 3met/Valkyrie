
#ifndef STATE_TREE_HPP
#define STATE_TREE_HPP

#include <map>
#include "move.hpp"
#include "chess_state.hpp"

using namespace std;

class StateTree {
private:
	map<ChessState, Move> states;

public:
	StateTree() {};
	~StateTree() {};
	
	bool has(ChessState cs);
	Move get(ChessState cs);
	void add(ChessState cs, Move m);
};

#endif
