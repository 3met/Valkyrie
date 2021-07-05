
#include "chess_engine.hpp"
#include "move.hpp"
#include "U8.hpp"

// Add killer heuristic move
void ChessEngine::addKillerMove(Move* m, U8* depth) {
	killerHeuristic[*depth][killerHeuristicNext[*depth]] = *m;
	killerHeuristicNext[*depth] = !killerHeuristicNext[*depth];
}
