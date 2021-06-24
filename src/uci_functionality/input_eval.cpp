
#include "eval_score.hpp"
#include "uci.hpp"

// Prints static evaluation of current board
void UCI::inputEval() {
	cout << EvalScore(engine.evalBoard(&cs, cs.WHITE)) << endl;
}
