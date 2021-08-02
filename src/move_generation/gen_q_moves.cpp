
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal queen moves
void ChessEngine::genQMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	// Get piece locations
	cs->pieces[cs->turn][QUEEN].getPosArr(queenPosArr[0], &pieceCount[0][0]);

	U8 j;
	for (U8 i(0); i<pieceCount[0][0]; ++i) {
		bufferBoard.board = cs->pieces[0][6].board | cs->pieces[1][6].board;
		// --- Bishop-like moves ---
		// Find all potential squares
		moveBoard.board = bufferBoard.board;
		moveBoard.board &= bishopMasks[queenPosArr[0][i]].board;
		moveBoard.board *= bishopMagics[queenPosArr[0][i]].board;
		moveBoard.board >>= bishopMagicShifts[queenPosArr[0][i]];
		moveBoard = bishopAttackTable[queenPosArr[0][i]][moveBoard.board];
		// Remove friendly targets
		moveBoard.board &= ~(cs->pieces[cs->turn][6].board);
		// Seperate enemy targets
		killBoard.board = moveBoard.board & cs->pieces[!cs->turn][6].board;
		// Remove enemies from open
		moveBoard.board &= ~(cs->pieces[!cs->turn][6].board);

		// Add kill moves
		if (killBoard.board != 0) {
			killBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(QUEEN,
					queenPosArr[0][i],
					posTargets[j],
					cs->getPieceType(!cs->turn, posTargets[j]));
				++*moveCount;
			}
		}
		// Add non-kill moves
		if (moveBoard.board != 0) {
			moveBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(QUEEN, queenPosArr[0][i], posTargets[j]);
				++*moveCount;
			}
		}

		// --- Rook-like moves ---
		// Find all potential squares
		moveBoard.board = bufferBoard.board;
		moveBoard.board &= rookMasks[queenPosArr[0][i]].board;
		moveBoard.board *= rookMagics[queenPosArr[0][i]].board;
		moveBoard.board >>= rookMagicShifts[queenPosArr[0][i]];
		moveBoard = rookAttackTable[queenPosArr[0][i]][moveBoard.board];
		// Remove friendly targets
		moveBoard.board &= ~(cs->pieces[cs->turn][6].board);
		// Seperate enemy targets
		killBoard.board = moveBoard.board & cs->pieces[!cs->turn][6].board;
		// Remove enemies from open
		moveBoard.board &= ~(cs->pieces[!cs->turn][6].board);

		// Add kill moves
		if (killBoard.board != 0) {
			killBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(QUEEN,
					queenPosArr[0][i],
					posTargets[j],
					cs->getPieceType(!cs->turn, posTargets[j]));
				++*moveCount;
			}
		}
		// Add non-kill moves
		if (moveBoard.board != 0) {
			moveBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(QUEEN, queenPosArr[0][i], posTargets[j]);
				++*moveCount;
			}
		}
	}
}

// Generates all psudo-legal queen kill moves
void ChessEngine::genQKillMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	// Get piece locations
	cs->pieces[cs->turn][QUEEN].getPosArr(queenPosArr[0], &pieceCount[0][0]);

	U8 j;
	for (U8 i(0); i<pieceCount[0][0]; ++i) {
		bufferBoard.board = cs->pieces[0][6].board | cs->pieces[1][6].board;
		// --- Bishop-like moves ---
		// Find all potential squares
		killBoard.board = bufferBoard.board;
		killBoard.board &= bishopMasks[queenPosArr[0][i]].board;
		killBoard.board *= bishopMagics[queenPosArr[0][i]].board;
		killBoard.board >>= bishopMagicShifts[queenPosArr[0][i]];
		killBoard = bishopAttackTable[queenPosArr[0][i]][killBoard.board];
		killBoard.board &= cs->pieces[!cs->turn][6].board;

		// Add kill moves
		if (killBoard.board != 0) {
			killBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(QUEEN,
					queenPosArr[0][i],
					posTargets[j],
					cs->getPieceType(!cs->turn, posTargets[j]));
				++*moveCount;
			}
		}

		// --- Rook-like moves ---
		// Find all potential squares
		killBoard.board = bufferBoard.board;
		killBoard.board &= rookMasks[queenPosArr[0][i]].board;
		killBoard.board *= rookMagics[queenPosArr[0][i]].board;
		killBoard.board >>= rookMagicShifts[queenPosArr[0][i]];
		killBoard = rookAttackTable[queenPosArr[0][i]][killBoard.board];
		killBoard.board &= cs->pieces[!cs->turn][6].board;

		// Add kill moves
		if (killBoard.board != 0) {
			killBoard.popPosArr(posTargets, &targetCount);
			for (j=0; j<targetCount; ++j) {
				moves[*moveCount] = Move(QUEEN,
					queenPosArr[0][i],
					posTargets[j],
					cs->getPieceType(!cs->turn, posTargets[j]));
				++*moveCount;
			}
		}
	}
}
