
#include <algorithm>
#include "chess_engine.hpp"
#include "move.hpp"
#include "move_compare.hpp"

using namespace std;

// Call std::sort on vector of moves
void ChessEngine::sortMoves(Move moves[218], U8* moveCount, U8 depth) {
	sort(moves, moves+(*moveCount), MoveCompare(this, depth));
}
