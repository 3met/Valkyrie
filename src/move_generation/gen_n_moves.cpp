
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal knight moves
void ChessEngine::genNMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	U8 j;
	S8 killed;
	
	// Get all knight locations
	cs->pieces[cs->turn][cs->KNIGHT].getPosArr(knightPosArr[0], &pieceCount[0][0]);
	
	// Loop through pieces
	for (U8 i(0); i<pieceCount[0][0]; ++i) {
		// Get potential squares
		moveBoard = NMoveDB[knightPosArr[0][i]];
		// Remove squares with same colored pieces
		moveBoard.board &= ~(cs->pieces[cs->turn][cs->ALL_PIECES].board);	
		// Positions of all targets
		moveBoard.popPosArr(posTargets, &targetCount);

		for (j=0; j<targetCount; ++j) {
			// Check for killing a piece
			if (cs->pieces[!cs->turn][cs->ALL_PIECES].getPos(posTargets[j])) {
				killed = cs->getPieceType(!cs->turn, posTargets[j]);
			} else {
				killed = -1;	// Default
			}

			// Create move
			moves[*moveCount] = Move(cs->KNIGHT, knightPosArr[0][i], posTargets[j], killed);
			++*moveCount;
		}
	}
}


// Generates all psudo-legal knight kill moves
void ChessEngine::genNKillMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	U8 j;
	S8 killed;
	
	// Get all knight locations
	cs->pieces[cs->turn][cs->KNIGHT].getPosArr(knightPosArr[0], &pieceCount[0][0]);
	
	// Loop through pieces
	for (U8 i(0); i<pieceCount[0][0]; ++i) {
		// Get potential squares
		killBoard = NMoveDB[knightPosArr[0][i]];
		// Positions of all target squares
		killBoard.board &= cs->pieces[!cs->turn][6].board;
		// Positions of all targets
		killBoard.popPosArr(posTargets, &targetCount);

		for (j=0; j<targetCount; ++j) {
			killed = cs->getPieceType(!cs->turn, posTargets[j]);

			// Create move
			moves[*moveCount] = Move(cs->KNIGHT, knightPosArr[0][i], posTargets[j], killed);
			++*moveCount;
		}
	}
}
