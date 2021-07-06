
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal king moves
void ChessEngine::genKMoves(ChessState* cs, Move moves[218], U8* moveCount){
	
	// Start position of the king
	kingPos[0] = cs->pieces[cs->turn][cs->KING].getFirstPos();
	// Get surrounding squares
	moveBoard = (KMoveDB[kingPos[0]].board);
	// Remove squares with same coloured pieces
	moveBoard.board &= ~(cs->pieces[cs->turn][cs->ALL_PIECES].board);
	// Positions of all target squares
	moveBoard.popPosArr(posTargets, &targetCount);

	S8 killed;
	// Add moves to vector
	for (U8 i(0); i<targetCount; ++i) {
		// Check for killing a piece
		if (cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(posTargets[i])) {
			killed = cs->getPieceType(!cs->turn, posTargets[i]);
		} else {
			killed = -1;	// Default
		}

		// Add to list of valid moves
		moves[*moveCount] = Move(cs->KING, kingPos[0], posTargets[i], killed);
		++*moveCount;
	}

	// Castling Moves
	if (cs->castlePerms[cs->turn][cs->KING_SIDE]
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(kingPos[0]+1)	// Ensure no pieces are in the way
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(kingPos[0]+2)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(kingPos[0]+1)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(kingPos[0]+2)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0])	// Cannot castle out of check
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]+1)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]+2)) {

		moves[*moveCount] = Move(cs->KING, kingPos[0], kingPos[0]+2);
		++*moveCount;
	}
	if (cs->castlePerms[cs->turn][cs->QUEEN_SIDE]
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(kingPos[0]-1)	// Ensure no pieces are in the way
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(kingPos[0]-2)
		&& !cs->pieces[cs->turn][cs->ALL_PIECES].getPos(kingPos[0]-3)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(kingPos[0]-1)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(kingPos[0]-2)
		&& !cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(kingPos[0]-3)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0])	// Cannot castle out of check
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]-1)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]-2)) {

		moves[*moveCount] = Move(cs->KING, kingPos[0], kingPos[0]-2);
		++*moveCount;
	}
}
