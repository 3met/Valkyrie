
#pragma once
#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include <map>
#include "board_hash.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "U8.hpp"

class HashScore {
public:
	HashScore() {};
	HashScore(EvalScore s, U8 d) { score=s; depth=d; };
	~HashScore() {};

	EvalScore score;
	U8 depth;
};

class TranspositonTable {
private:
	map<BoardHash, HashScore> table;
public:
	
	void clear();	// Clears table
	size_t size();

	void add(const ChessState* cs, EvalScore score, U8 depth);
	void add(const BoardHash* bh, EvalScore score, U8 depth);
	
	bool contains(const ChessState* cs);
	bool contains(const BoardHash* bh);

	HashScore get(const ChessState* cs);
	HashScore get(const BoardHash* bh);
};

#endif
