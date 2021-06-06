
#include <algorithm>
#include "chess_engine.hpp"
#include "move.hpp"
#include "move_compare.hpp"

using namespace std;

/*	Sorts moves for alpha beta pruning
	Order:
		Captures ordered by decreasing value of piece killed
		Pawn promotions
		Leftover moves
*/

void ChessEngine::sortMoves(vector<Move>* moves, U8 depth) {
	sort(moves->begin(), moves->end(), MoveCompare(this, depth));
}
