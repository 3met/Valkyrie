
#include <cmath>
#include <iostream>
#include "eval_score.hpp"

using namespace std;

EvalScore::EvalScore() {}

EvalScore::EvalScore(short _eval) {
	eval = _eval;
}

EvalScore::~EvalScore() {}

const EvalScore EvalScore::DEFAULT = EvalScore(0);
const EvalScore EvalScore::DRAW = EvalScore(0);
const EvalScore EvalScore::INFINITE = EvalScore(32700);
const EvalScore EvalScore::NULL_SCORE = EvalScore(-32701);
const EvalScore EvalScore::MATE_IN_0 = EvalScore(32699);
const EvalScore EvalScore::MAX_PLAIN_EVAL = EvalScore(32000);	// Max non-checkmate rating

// Prints the evaluation
ostream& operator<<(ostream &out, EvalScore const& obj) {
	if (abs(obj.eval) == EvalScore::INFINITE) {
		if (obj.eval > 0) {
			out << "+INF";
		} else {
			out << "-INF";
		}
	} else if (obj.eval == EvalScore::NULL_SCORE) {
		out << "NullScore";
	} else if (obj.hasMate()) {
		if (obj.eval > 0) {
			out << "+M";
		} else {
			out << "-M";
		}

		out << (EvalScore::MATE_IN_0.eval - abs(obj.eval));

	} else {
		if (obj.eval > 0) {
			out << '+';
		}

		out << obj.eval;
	}

	return out;
}

// Returns whether the score is a checkmate
bool EvalScore::hasMate() const {
	return abs(eval) > MAX_PLAIN_EVAL.eval && abs(eval) < INFINITE.eval;
}

// Returns the number of halfmoves until checkmate
short EvalScore::halfMovesToMate() const {
	return MATE_IN_0.eval - abs(eval);
}

// Increments the number of halfmoves until checkmate
void EvalScore::addHalfMoveToMate() {
	if (eval > 0) {
		eval -= 1;
	} else {
		eval += 1;
	}
}
