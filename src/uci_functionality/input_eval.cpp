
#include "eval_score.hpp"
#include "uci.hpp"

/* Prints Current Evaluation */
void UCI::inputEval() {
	cout << EvalScore(engine.evalBoard(&cs, cs.WHITE)) << endl;
}
