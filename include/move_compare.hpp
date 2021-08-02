
#pragma once
#ifndef MOVE_COMPARE_HPP
#define MOVE_COMPARE_HPP

#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "size_defs.hpp"

// Compares moves for moving ording prior to alpha-beta pruning search.
class MoveCompare {
private:
	ChessEngine* engine;
	ChessState* cs;
	U8 depth;
	const BaseMove* hashMove;

	// Center bias ranks square based on distance from the center
	static const U8 centerBias[64];

public:
	MoveCompare(ChessEngine* _engine, ChessState* _cs, U8 _depth, const BaseMove* _hashMove);

	bool operator()(Move& a, Move& b) const;
};

#endif
