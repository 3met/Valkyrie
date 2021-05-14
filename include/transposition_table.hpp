
#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "board_hash.hpp"
#include "eval_score.hpp"
#include "U8.hpp"

struct HashScore {
	Evalscore score;
	U8 depth;
};

class TranspositonTable {
private:
	map<BoardHash, HashScore> table;
public:
	// void add(BoardHash bh, Evalscore score);
	
	// FIND METHOD
};

#endif
