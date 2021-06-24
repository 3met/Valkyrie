
#include <algorithm>
#include "chess_engine.hpp"
#include "move.hpp"
#include "move_compare.hpp"

using namespace std;

void ChessEngine::sortMoves(vector<Move>* moves, U8 depth) {
	sort(moves->begin(), moves->end(), MoveCompare(this, depth));
}
