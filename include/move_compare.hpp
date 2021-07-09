
#pragma once
#ifndef MOVE_COMPARE_HPP
#define MOVE_COMPARE_HPP

#include "chess_engine.hpp"
#include "move.hpp"
#include "size_defs.hpp"

// Compares moves for moving ording prior to alpha-beta pruning search.
class MoveCompare {
private:
	ChessEngine* engine;
	U8 depth;

	// Center bias ranks square based on distance from the center
	static const U8 centerBias[64];

public:
	MoveCompare(ChessEngine* _engine, U8 _depth);

	bool operator()(const Move& a, const Move& b) const;
};

#endif
