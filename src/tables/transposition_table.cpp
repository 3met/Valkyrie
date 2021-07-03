
#include "transposition_table.hpp"
#include "U8.hpp"
#include "U64.hpp"

// Constructor to allocate memory
TranspostionTable::TranspostionTable() {}

// Constructor to allocate memory
TranspostionTable::TranspostionTable(U64 memSize) {
	tableSize = U64(memSize / sizeof(TTEntry));
	table = new TTEntry[tableSize]();	
}

// Destructor to free memory
TranspostionTable::~TranspostionTable() {
	delete [] table;
}

// Clears Transposition Table
void TranspostionTable::clear() {
	for (U64 i(0); i<tableSize; ++i) {
		table[i].setNull();
	}
}

// Returns number of table entries
size_t TranspostionTable::size() {
	return tableSize;
}

// Adds entry to the table
void TranspostionTable::add(const ChessState* cs, EvalScore score, U8 depth) {
	this->add(&cs->bh, score, depth);
}

void TranspostionTable::add(const BoardHash* bh, EvalScore score, U8 depth) {
	this->table[bh->hash % tableSize].setData(score, depth);
}

bool TranspostionTable::contains(const ChessState* cs) {
	return this->contains(&cs->bh);
}

bool TranspostionTable::contains(const BoardHash* bh) {
	return !this->table[bh->hash % tableSize].isNull;
}

TTEntry TranspostionTable::get(const ChessState* cs) {
	return this->get(&cs->bh);
}

TTEntry TranspostionTable::get(const BoardHash* bh) {
	return this->table[bh->hash % tableSize];
}
