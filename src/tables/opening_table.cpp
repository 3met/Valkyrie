
#include <iostream>
#include <vector>
#include "board_hash.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "opening_table.hpp"

using namespace std;

OpeningTable::OpeningTable() {};
OpeningTable::~OpeningTable() {};

void OpeningTable::add(const ChessState* cs, vector<Move>* moves) {
	this->add(cs->bh, moves);
}

void OpeningTable::add(const BoardHash bh, vector<Move>* moves) {
	table[bh] = *moves;
}

bool OpeningTable::contains(const ChessState* cs) {
	return this->contains(cs->bh);
}

bool OpeningTable::contains(const BoardHash bh) {
	if (table.find(bh) != table.end()) {
		return true;
	} else {
		return false;
	}
}

vector<Move> OpeningTable::get(const ChessState* cs) {
	return this->get(cs->bh);
}

vector<Move> OpeningTable::get(const BoardHash bh) {
	table.find(bh);
	return table[bh];
}
