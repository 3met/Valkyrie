
#pragma once
#ifndef EVAL_SCORE_HPP
#define EVAL_SCORE_HPP

#include <iostream>
#include "size_defs.hpp"

using namespace std;

// Stores chess state evaluation score.
// Takes into account checkmate and mate in N moves.
class EvalScore {
public:
	EvalScore();
	EvalScore(short _eval);
	~EvalScore();

	// The score in centipawns
	short eval;

	static const EvalScore DEFAULT;
	static const EvalScore DRAW;
	static const EvalScore INFINITE;
	static const EvalScore MATE_IN_0;
	static const EvalScore MAX_PLAIN_EVAL;

	// Compares Move Ratings
	// The higher rating is better for white and the lower for black.
	// Takes into account checkmate.
	// If both positions are checkmate, the comparison returns
	// the fastest checkmate for black or slowest for white.
	// Overall returns true if a is the better move for black.
	friend bool operator<(const EvalScore& a, const EvalScore& b) {
		return a.eval < b.eval;
	};

	friend bool operator>(const EvalScore& a, const EvalScore& b) {
		return a.eval > b.eval;
	};

	friend bool operator>=(const EvalScore& a, const EvalScore& b) {
		return a.eval >= b.eval;
	}

	friend bool operator<=(const EvalScore& a, const EvalScore& b) {
		return a.eval <= b.eval;
	}

	friend bool operator==(const EvalScore& a, const EvalScore& b) {
		return a.eval == b.eval;
	}

	// Returns the score as though the colors have been switched
	friend EvalScore operator-(const EvalScore& obj) {
		return EvalScore(-obj.eval);
	};

	friend ostream& operator<<(ostream& out, const EvalScore& obj);

	bool hasMate() const;
	short halfMovesToMate() const;
	void addHalfMoveToMate();
};

#endif
