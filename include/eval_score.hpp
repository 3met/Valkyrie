
#pragma once
#ifndef EVAL_SCORE_HPP
#define EVAL_SCORE_HPP

#include <iostream>
#include "U8.hpp"

using namespace std;

// Stores chess state evaluation score.
// Takes into account checkmate and mate in N moves.
class EvalScore {
public:
	EvalScore();
	EvalScore(short eval);
	EvalScore(short eval, bool foundMate);
	EvalScore(short eval, bool foundMate, U8 movesToMate);
	~EvalScore();

	short eval;	// Board evaluation if foundMate is true,
	           	// +1 for white mate, -1 for black mate
	bool foundMate;	// Whether checkmate has beem found
	U8 movesToMate;	// Moves to checkmate

	// Compares Move Ratings
	// The higher rating is better for white and the lower for black.
	// Takes into account checkmate.
	// If both positions are checkmate, the comparison returns
	// the fastest checkmate for black or slowest for white.
	// Overall returns true if a is the better move for black.
	friend bool operator<(const EvalScore& a, const EvalScore& b) {
		// If neither rating is checkmate
		if (!a.foundMate && !b.foundMate) {
			return a.eval < b.eval;

		// If only a rating is checkmate
		} else if (a.foundMate && !b.foundMate) {
			return a.eval == -1;

		// If only b rating is checkmate
		} else if (!a.foundMate && b.foundMate) {	
			return b.eval == 1;

		// If both ratings are checkmate
		} else {
			// If found mates are for the same color
			if (a.eval == b.eval) {
				// If found mate is for white
				if (a.eval == 1) {
					return a.movesToMate > b.movesToMate;
				} else {
					return a.movesToMate < b.movesToMate;
				}
			// If mates are for different colors
			} else {
				if (a.eval == 1) {
					return false;
				} else {
					return true;
				}
			}
		}
	};

	friend bool operator>(const EvalScore& a, const EvalScore& b) {
		return b < a;
	};

	friend bool operator>=(const EvalScore& a, const EvalScore& b) {
	    return !(a < b);
	}

	friend bool operator<=(const EvalScore& a, const EvalScore& b) {
	    return !(b < a);
	}

	friend bool operator==(const EvalScore& a, const EvalScore& b) {
	    return !(b < a) && !(a < b);
	}

	// Returns the score as though the colors have been switched
	friend EvalScore operator-(const EvalScore& obj) {
		return EvalScore(-obj.eval, obj.foundMate, obj.movesToMate);
	};

	friend ostream& operator<<(ostream& out, const EvalScore& obj); 
};

#endif
