
#include <vector>
#include "board_hash.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "opening_table.hpp"

using namespace std;

OpeningTable::OpeningTable() {};
OpeningTable::~OpeningTable() {};

// Clears table
void OpeningTable::clear() {
	this->table.clear();
}

// Returns number of table entries
size_t OpeningTable::size() {
	return this->table.size();
}

// Add a list of possible moves for a given BoardHash
void OpeningTable::add(const BoardHash* bh, vector<Move>* moves) {
	table[*bh] = *moves;
}

// Check if opening moves exist for a given BoardHash
bool OpeningTable::contains(const BoardHash* bh) {
	if (table.find(*bh) != table.end()) {
		return true;
	} else {
		return false;
	}
}

// Get opening moves for a given BoardHash
vector<Move> OpeningTable::get(const BoardHash* bh) {
	table.find(*bh);
	return table[*bh];
}
