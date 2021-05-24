
#include "eval_score.hpp"

using namespace std;

EvalScore::EvalScore() {}

EvalScore::EvalScore(short e) {
	eval = e;
	foundMate = false;
}

EvalScore::EvalScore(short e, bool fM) {
	eval = e;
	foundMate = fM;
	movesToMate = 0;
}

EvalScore::EvalScore(short e, bool fM, U8 mTM) {
	eval = e;
	foundMate = fM;
	movesToMate = mTM;
}

EvalScore::~EvalScore() {}

ostream& operator<< (ostream &out, EvalScore const& obj) {
	if (obj.eval > 0) {
		out << '+';
	}

	if (obj.foundMate && obj.eval != 0) {
		if (obj.eval < 0) {
			out << '-';
		}
		out << 'M' << short(obj.movesToMate);
	} else {
		out << obj.eval;
	}
    
    return out;
}
