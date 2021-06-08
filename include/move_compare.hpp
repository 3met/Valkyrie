
#pragma once
#ifndef MOVE_COMPARE_HPP
#define MOVE_COMPARE_HPP

#include "chess_engine.hpp"
#include "move.hpp"
#include "transposition_table.hpp"
#include "U8.hpp"

/* Class to compare moves for moving ording prior to
   alpha-beta pruning search. */
class MoveCompare {
private:
	ChessEngine* engine;
	U8 depth;

	static const U8 centerBias[64];

public:
	MoveCompare(ChessEngine* _engine, U8 _depth);

	bool operator()(const Move& a, const Move& b) const;
};

#endif
