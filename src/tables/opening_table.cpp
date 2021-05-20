
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
	BoardHash bh(cs);
	this->add(&bh, moves);
}

void OpeningTable::add(const BoardHash* bh, vector<Move>* moves) {
	table[*bh] = *moves;
}

bool OpeningTable::contains(const ChessState* cs) {
	BoardHash bh(cs);
	return this->contains(&bh);
}

bool OpeningTable::contains(const BoardHash* bh) {
	if (table.find(*bh) != table.end()) {
		return true;
	} else {
		return false;
	}
}

vector<Move> OpeningTable::get(const ChessState* cs) {
	BoardHash bh(cs);
	return this->get(&bh);
}

vector<Move> OpeningTable::get(const BoardHash* bh) {
	table.find(*bh);
	return table[*bh];
}
