
#pragma once
#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "board_hash.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "size_defs.hpp"

// TTEntry stores info which the transposition table maps to.
class TTEntry {
public:
	enum ScoreType {
		EXACT_SCORE = 0,
		ALPHA_SCORE,
		BETA_SCORE,
		UNKNOWN_SCORE,
	};

	BoardHash bh;
	U8 depth;
	EvalScore score;
	U8 scoreType;
	Move bestMove;

	TTEntry();
	~TTEntry();

	void setNull();
	void setScoreData(const BoardHash* bh, U8 depth, EvalScore score, U8 scoreType) ;
	void setMoveData(const BoardHash* bh, U8 depth, EvalScore score, U8 scoreType, Move bestMove) ;
};

// Transposition table is used to store previous enteries
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

	bool contains(const BoardHash* bh);
	TTEntry get(const BoardHash* bh);
	EvalScore getScore(const BoardHash* bh);
	Move getMove(const BoardHash* bh);
	TTEntry* getEntryPointer(const BoardHash* bh);
};

#endif
