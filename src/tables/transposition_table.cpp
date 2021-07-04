
#include "transposition_table.hpp"
#include "U8.hpp"
#include "U64.hpp"

// ----- Transposition Table Entry/Row Methods -----

TTEntry::TTEntry() {}

TTEntry::TTEntry(const BoardHash* _bh, EvalScore _score, U8 _depth) {
	bh = *_bh;
	score = _score;
	depth = _depth;
}

TTEntry::~TTEntry() {}

void TTEntry::setNull() {
	bh.hash = 0;
}

void TTEntry::setData(const BoardHash* _bh, EvalScore _score, U8 _depth) {
	bh = *_bh;
	score = _score;
	depth = _depth;
}

// ----- Transposition Table Methods -----

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

// Resizes table and clears old entries
void TranspostionTable::resize(U64 memSize) {
	delete [] table;

	tableSize = U64(memSize / sizeof(TTEntry));
	table = new TTEntry[tableSize]();	
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
	this->table[bh->hash % tableSize].setData(bh, score, depth);
}

bool TranspostionTable::contains(const ChessState* cs) {
	return this->contains(&cs->bh);
}

bool TranspostionTable::contains(const BoardHash* bh) {
	return this->table[bh->hash % tableSize].bh == *bh;
}

TTEntry TranspostionTable::get(const ChessState* cs) {
	return this->get(&cs->bh);
}

TTEntry TranspostionTable::get(const BoardHash* bh) {
	return this->table[bh->hash % tableSize];
}
