
#pragma once
#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "board_hash.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "size_defs.hpp"

// TTEntry stores info which the transposition table maps to.
class TTEntry {
public:
	BoardHash bh;
	EvalScore score;
	U8 depth;

	TTEntry();
	TTEntry(const BoardHash* _bh, EvalScore _score, U8 _depth);
	~TTEntry();

	void setNull();
	void setData(const BoardHash* _bh, EvalScore _score, U8 _depth) ;
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
	void resize(U64 memSize);
	size_t size();

	void add(const ChessState* cs, EvalScore score, U8 depth);
	void add(const BoardHash* bh, EvalScore score, U8 depth);
	bool contains(const ChessState* cs);
	bool contains(const BoardHash* bh);
	TTEntry get(const ChessState* cs);
	TTEntry get(const BoardHash* bh);
};

#endif
