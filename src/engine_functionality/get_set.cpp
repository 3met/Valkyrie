
#include "chess_engine.hpp"
#include "move.hpp"
#include "U8.hpp"

/* Add killer heuristic move */
void ChessEngine::addKillerMove(Move* m, U8* depth) {
	if (killerHeuristic[*depth].size() == KILL_QUEUE_MAX_SIZE) {
		killerHeuristic[*depth].pop_front();
		killerHeuristic[*depth].push_back(*m);
	} else {
		killerHeuristic[*depth].push_back(*m);
	}
}
