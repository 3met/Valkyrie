
#include "transposition_table.hpp"
#include "U8.hpp"

/* Clears Transposition Table */
void TranspositonTable::clear() {
	this->table.clear();
}

/* Adds entry to the table */
void TranspositonTable::add(const ChessState* cs, EvalScore score, U8 depth) {
	BoardHash bh(cs);
	this->add(&bh, score, depth);
}

void TranspositonTable::add(const BoardHash* bh, EvalScore score, U8 depth) {
	this->table[*bh] = HashScore(score, depth);
}

bool TranspositonTable::contains(const ChessState* cs) {
	BoardHash bh(cs);
	return this->contains(&bh);
}

bool TranspositonTable::contains(const BoardHash* bh) {
	if (this->table.find(*bh) != this->table.end()) {
		return true;
	}
	return false;
}

HashScore TranspositonTable::get(const ChessState* cs) {
	BoardHash bh(cs);
	return this->get(&bh);
}

HashScore TranspositonTable::get(const BoardHash* bh) {
	this->table.find(*bh);
	return this->table[*bh];
}
