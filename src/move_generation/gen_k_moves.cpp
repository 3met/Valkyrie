
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "S8.hpp"
#include "U8.hpp"

void ChessEngine::genKMoves(ChessState* cs, vector<Move>* moves){
	/* Generates all legal king moves */
	
	// Start position of the king
	U8 start = cs->pieces[cs->turn][cs->KING].getFirstPos();
	// Get surrounding squares
	Bitboard target_board = KMoveDB[start];
	// Remove squares with same coloured pieces
	target_board.board &= ~(cs->pieces[cs->turn][cs->ALL_PIECES].board);
	// Positions of all target squares
	vector<U8> targets = target_board.getPosVec();

	S8 killed;
	// Add moves to vector
	for (U8 i=0; i<targets.size(); ++i) {
		// Check for killing a piece
		if (cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(targets[i])) {
			killed = cs->getPieceType(!cs->turn, targets[i]);
		} else {
			killed = -1;	// Default
		}

		// Add to list of valid moves
		moves->push_back(Move(cs->KING, start, targets[i], killed));
	}

	// Castling Moves
	if (cs->castlePerms[cs->turn][cs->KING_SIDE]
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(start+1)	// Ensure no pieces are in the way
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(start+2)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(start+1)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(start+2)
		&& !isPosAttacked(cs, !cs->turn, start)	// Cannot castle out of check
		&& !isPosAttacked(cs, !cs->turn, start+1)
		&& !isPosAttacked(cs, !cs->turn, start+2)) {

		moves->push_back(Move(cs->KING, start, start+2));
	}
	if (cs->castlePerms[cs->turn][cs->QUEEN_SIDE]
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(start-1)	// Ensure no pieces are in the way
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(start-2)
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(start-3)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(start-1)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(start-2)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(start-3)
		&& !isPosAttacked(cs, !cs->turn, start)	// Cannot castle out of check
		&& !isPosAttacked(cs, !cs->turn, start-1)
		&& !isPosAttacked(cs, !cs->turn, start-2)) {

		moves->push_back(Move(cs->KING, start, start-2));
	}
}
