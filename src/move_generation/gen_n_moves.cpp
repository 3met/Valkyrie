
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "S8.hpp"
#include "U8.hpp"

/* Generates all legal knight moves */
void ChessEngine::genNMoves(ChessState* cs, vector<Move>* moves) {
	U8 i, j;
	vector<U8> start;
	Bitboard target_board;
	vector<U8> targets;
	S8 killed;
	
	// Get all knight locations
	cs->pieces[cs->turn][cs->KNIGHT].getPosVector(&start);

	for (i=0; i<start.size(); ++i) {
		// Get potential squares
		target_board = NMoveDB[start[i]];
		// Remove squares with same coloured pieces
		target_board.board &= ~(cs->pieces[cs->turn][cs->ALL_PIECES].board);	
		// Positions of all targets
		targets = target_board.getPosVector();

		// Add moves to vector
		for (j=0; j<targets.size(); ++j) {
			// Check for killing a piece
			if (cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(targets[j])) {
				killed = cs->getPieceType(!cs->turn, targets[j]);
			} else {
				killed = -1;	// Default
			}

			// Store as Move object
			moves->push_back(Move(cs->KNIGHT, start[i], targets[j], killed));
		}
	}
}
