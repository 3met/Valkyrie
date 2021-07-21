
#include "transposition_table.hpp"
#include "size_defs.hpp"

// ----- Transposition Table Entry/Row Methods -----

TTEntry::TTEntry() {}

TTEntry::~TTEntry() {}

// Clears the entry
void TTEntry::setNull() {
	bh.hash = 0;
	depth = 0;
	score = EvalScore::NULL_SCORE;
	scoreType = UNKNOWN_SCORE;
	bestMove = Move::NULL_MOVE;
}

// Stores data for a score
void TTEntry::setScoreData(const BoardHash* _bh, U8 _depth, EvalScore _score, U8 _scoreType) {
	bh = *_bh;
	depth = _depth;
	score = _score;
	scoreType = _scoreType;
	bestMove = Move::NULL_MOVE;
}

// Stores data for a move
void TTEntry::setMoveData(const BoardHash* _bh, U8 _depth, EvalScore _score, U8 _scoreType, Move _bestMove) {
	bh = *_bh;
	depth = _depth;
	score = _score;
	scoreType = _scoreType;
	bestMove = _bestMove;
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

// Returns whether a given key is in the table
bool TranspostionTable::contains(const BoardHash* bh) {
	return this->table[bh->hash % tableSize].bh == *bh;
}

TTEntry TranspostionTable::get(const BoardHash* bh) {
	return this->table[bh->hash % tableSize];
}

EvalScore TranspostionTable::getScore(const BoardHash* bh) {
	return this->table[bh->hash % tableSize].score;
}

Move TranspostionTable::getMove(const BoardHash* bh) {
	return this->table[bh->hash % tableSize].bestMove;
}

// Returns a pointer to the entry matching the passed BoardHash
TTEntry* TranspostionTable::getEntryPointer(const BoardHash* bh) {
	return &this->table[bh->hash % tableSize];
}
