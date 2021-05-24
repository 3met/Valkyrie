
#pragma once
#ifndef OPENING_TABLE_HPP
#define OPENING_TABLE_HPP

#include <map>
#include <vector>
#include "board_hash.hpp"
#include "chess_state.hpp"
#include "move.hpp"

using namespace std;

class OpeningTable {
private:
	map<BoardHash, vector<Move>> table;

public:
	OpeningTable();
	~OpeningTable();

	void clear();
	size_t size();

	void add(const ChessState* cs, vector<Move>* moves);
	void add(const BoardHash* bh, vector<Move>* moves);
	bool contains(const ChessState* cs);
	bool contains(const BoardHash* bh);
	vector<Move> get(const ChessState* cs);
	vector<Move> get(const BoardHash* bh);
};

#endif
