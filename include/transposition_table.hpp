
#pragma once
#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "board_hash.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "U8.hpp"
#include "U64.hpp"

// TTEntry stores info which the transposition table maps to.
class TTEntry {
public:
	bool isNull;
	EvalScore score;
	U8 depth;

	TTEntry() { isNull=true; };
	TTEntry(EvalScore s, U8 d) { score=s; depth=d; isNull=false; };
	~TTEntry() {};

	void setNull() { isNull=true; };
	void setData(EvalScore s, U8 d) { score=s; depth=d; };
};

// Wrapper for a map between BoardHashes and their matching TTEntrys.
class TranspostionTable {
private:
	TTEntry* table;
	U64 tableSize;

public:
	TranspostionTable();
	TranspostionTable(U64 memSize);
	~TranspostionTable();

	void clear();
	size_t size();

	void add(const ChessState* cs, EvalScore score, U8 depth);
	void add(const BoardHash* bh, EvalScore score, U8 depth);
	bool contains(const ChessState* cs);
	bool contains(const BoardHash* bh);
	TTEntry get(const ChessState* cs);
	TTEntry get(const BoardHash* bh);
};

#endif
