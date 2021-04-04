#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

void ChessEngine::genKMoves(ChessState* cs, vector<Move>* moves){
	/* Generates all legal king moves */

	U8 i, j;
	vector<Move> validMoves;
	vector<U8> start;
	Bitboard target_board;
	vector<U8> targets;
	short killed;
	
	// Start position of the king
	start = cs->pieces[cs->turn][cs->KING]->getPosVector(1);

	// For each king (there should only be one)
	for (i=0; i<start.size(); ++i) {

		// Get surrounding squares
		target_board = KMoveDB.find(start[i])->second;
		// Remove squares with same coloured pieces
		target_board.board = target_board.board & (~cs->pieces[cs->turn][cs->ALL_PIECES]->board);	

		// TODO: CHECK IF TARGET SQUARE IS UNDER ATTACK

		// Positions of all target squares
		targets = target_board.getPosVector();

		// Add moves to vector
		for (j=0; j<targets.size(); ++j) {
			// Check for killing a piece
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(targets[j])) {
				killed = cs->getPieceType(!cs->turn, targets[j]);
			} else {
				killed = -1;	// Default
			}

			// Add to list of valid moves
			moves->push_back(Move(cs->KING, start[i], targets[j], killed));
		}
	}
}
