
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal king moves
void ChessEngine::genKMoves(ChessState* cs, Move moves[218], U8* moveCount){
	
	// Start position of the king
	kingPos[0] = cs->pieces[cs->turn][KING].getFirstPos();
	// Get surrounding squares
	moveBoard = (KMoveDB[kingPos[0]].board);
	// Remove squares with same coloured pieces
	moveBoard.board &= ~(cs->pieces[cs->turn][ALL_PIECES].board);
	// Positions of all target squares
	targetCount = moveBoard.popPosArr(posTargets);

	// Add moves to array
	for (U8 i(0); i<targetCount; ++i) {
		// Add to list of valid moves
		if (cs->pieces[!cs->turn][ALL_PIECES].getPos(posTargets[i])) {
			moves[*moveCount] = Move(kingPos[0], posTargets[i], Move::CAPTURE, KING);
			++*moveCount;
		} else {
			// Add to list of valid moves
			moves[*moveCount] = Move(kingPos[0], posTargets[i], Move::QUIET, KING);
			++*moveCount;
		}
	}

	// Castling Moves
	if (cs->castlePerms[cs->turn][KING_SIDE]
		&& !cs->pieces[cs->turn][ALL_PIECES].getPos(kingPos[0]+1)	// Ensure no pieces are in the way
		&& !cs->pieces[cs->turn][ALL_PIECES].getPos(kingPos[0]+2)
		&& !cs->pieces[!cs->turn][ALL_PIECES].getPos(kingPos[0]+1)
		&& !cs->pieces[!cs->turn][ALL_PIECES].getPos(kingPos[0]+2)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0])	// Cannot castle out of check
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]+1)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]+2)) {

		moves[*moveCount] = Move(kingPos[0], kingPos[0]+2, Move::KING_CASTLE, KING);
		++*moveCount;
	}
	if (cs->castlePerms[cs->turn][QUEEN_SIDE]
		&& !cs->pieces[cs->turn][ALL_PIECES].getPos(kingPos[0]-1)	// Ensure no pieces are in the way
		&& !cs->pieces[cs->turn][ALL_PIECES].getPos(kingPos[0]-2)
		&& !cs->pieces[cs->turn][ALL_PIECES].getPos(kingPos[0]-3)
		&& !cs->pieces[!cs->turn][ALL_PIECES].getPos(kingPos[0]-1)
		&& !cs->pieces[!cs->turn][ALL_PIECES].getPos(kingPos[0]-2)
		&& !cs->pieces[!cs->turn][ALL_PIECES].getPos(kingPos[0]-3)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0])	// Cannot castle out of check
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]-1)
		&& !isPosAttacked(cs, !cs->turn, kingPos[0]-2)) {

		moves[*moveCount] = Move(kingPos[0], kingPos[0]-2, Move::QUEEN_CASTLE, KING);
		++*moveCount;
	}
}

// Generates all psudo-legal king killing moves
void ChessEngine::genKKillMoves(ChessState* cs, Move moves[218], U8* moveCount){
	
	// Start position of the king
	kingPos[0] = cs->pieces[cs->turn][KING].getFirstPos();
	// Get surrounding squares
	killBoard.board = (KMoveDB[kingPos[0]].board);
	// Get attacking squares
	killBoard.board &= cs->pieces[!cs->turn][6].board;
	// Positions of all target squares
	targetCount = moveBoard.popPosArr(posTargets);

	// Add moves to array
	for (U8 i(0); i<targetCount; ++i) {
		// Add to list of valid moves
		moves[*moveCount] = Move(kingPos[0], posTargets[i], Move::CAPTURE, KING);
		++*moveCount;
	}
}
