
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal knight moves
void ChessEngine::genNMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	U8 j;
	
	// Get all knight locations
	pieceCount[0][0] = cs->pieces[cs->turn][KNIGHT].getPosArr(knightPosArr[0]);
	
	// Loop through pieces
	for (U8 i(0); i<pieceCount[0][0]; ++i) {
		// Get potential squares
		moveBoard = NMoveDB[knightPosArr[0][i]];
		// Remove squares with same colored pieces
		moveBoard.board &= ~(cs->pieces[cs->turn][ALL_PIECES].board);	
		// Positions of all targets
		targetCount = moveBoard.popPosArr(posTargets);

		for (j=0; j<targetCount; ++j) {
			// Check for killing a piece
			if (cs->pieces[!cs->turn][ALL_PIECES].getPos(posTargets[j])) {
				// Create move
				moves[*moveCount] = Move(knightPosArr[0][i], posTargets[j], Move::CAPTURE, KNIGHT);
				++*moveCount;
			} else {
				// Create move
				moves[*moveCount] = Move(knightPosArr[0][i], posTargets[j], Move::QUIET, KNIGHT);
				++*moveCount;
			}
		}
	}
}


// Generates all psudo-legal knight kill moves
void ChessEngine::genNKillMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	U8 j;
	
	// Get all knight locations
	pieceCount[0][0] = cs->pieces[cs->turn][KNIGHT].getPosArr(knightPosArr[0]);
	
	// Loop through pieces
	for (U8 i(0); i<pieceCount[0][0]; ++i) {
		// Get potential squares
		killBoard = NMoveDB[knightPosArr[0][i]];
		// Positions of all target squares
		killBoard.board &= cs->pieces[!cs->turn][6].board;
		// Positions of all targets
		targetCount = killBoard.popPosArr(posTargets);

		for (j=0; j<targetCount; ++j) {
			// Create move
			moves[*moveCount] = Move(knightPosArr[0][i], posTargets[j], Move::CAPTURE, KNIGHT);
			++*moveCount;
		}
	}
}
