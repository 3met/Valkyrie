
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal bishop moves
void ChessEngine::genBMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	// Get piece locations
	cs->pieces[cs->turn][cs->BISHOP].getPosArr(bishopPosArr[0], &pieceCount[0][0]);

	U8 j;
	for (U8 i(0); i<pieceCount[0][0]; ++i) {
		// Find all potential squares
		moveBoard.board = cs->pieces[0][6].board | cs->pieces[1][6].board;
		moveBoard.board &= bishopMasks[bishopPosArr[0][i]].board;
		moveBoard.board *= bishopMagics[bishopPosArr[0][i]].board;
		moveBoard.board >>= bishopMagicShifts[bishopPosArr[0][i]];
		moveBoard = bishopAttackTable[bishopPosArr[0][i]][moveBoard.board];
		// Remove friendly targets
		moveBoard.board &= ~(cs->pieces[cs->turn][6].board);
		// Seperate enemy move board
		killBoard.board = moveBoard.board & cs->pieces[!cs->turn][6].board;
		// Remove enemies from moveBoard
		moveBoard.board &= ~(cs->pieces[!cs->turn][6].board);

		// Add kill moves
		if (killBoard.board != 0) {
			killBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(cs->BISHOP,
					bishopPosArr[0][i],
					posTargets[j],
					cs->getPieceType(!cs->turn, posTargets[j]));
				++*moveCount;
			}
		}

		// Add non-kill moves
		if (moveBoard.board != 0) {
			moveBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(cs->BISHOP, bishopPosArr[0][i], posTargets[j]);
				++*moveCount;
			}
		}
	}
}
