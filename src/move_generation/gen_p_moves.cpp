
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal pawn moves
void ChessEngine::genWhitePMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	
	// Set en passant square
	bufferBoard = 0;
	if (cs->enPassantHistory[cs->moveNumber-1] != -1) {
		bufferBoard.setPosOn(cs->enPassantHistory[cs->moveNumber-1]);
	}

	bufferBoard.board |= cs->pieces[BLACK][ALL_PIECES].board;

	// ---- Right Kills ----
	killBoard = cs->pieces[WHITE][PAWN].board;
	// Remove all on 'h' file
	killBoard.board &= 9187201950435737471ULL;
	killBoard.board <<= 9;
	// Removes all but kill positions (en passant included)
	killBoard.board &= bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	U8 i(0);
	S8 killed;
	for ( ; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(BLACK, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed);
			++*moveCount;
		}
	}

	// ---- Left Kills ----
	killBoard.board = cs->pieces[WHITE][PAWN].board;
	// Remove all on 'a' file
	killBoard.board &= 18374403900871474942ULL;
	killBoard.board <<= 7;
	// Removes all but kill positions (en passant included)
	killBoard.board &=  bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(BLACK, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed);
			++*moveCount;
		}
	}

	// --- Generate non-kill moves ---
	bufferBoard.board = ~(cs->pieces[WHITE][ALL_PIECES].board | cs->pieces[BLACK][ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[WHITE][PAWN].board << 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	moveBoard.getPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]-8, posTargets[i], -1, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-8, posTargets[i], -1, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-8, posTargets[i], -1, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-8, posTargets[i], -1, KNIGHT);
			++*moveCount;
		} else {
			moves[*moveCount] = Move(PAWN, posTargets[i]-8, posTargets[i]);
			++*moveCount;
		}
	}

	// Remove all except potential double moves
	moveBoard.board &= (255 << 16);
	moveBoard.board <<= 8;
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	moveBoard.popPosArr(posTargets, &targetCount);
	// Add positions to array
	for (i=0; i<targetCount; ++i) {
		moves[*moveCount] = Move(PAWN, posTargets[i]-16, posTargets[i]);
		++*moveCount;
	}
}

// Generates all psudo-legal pawn moves
void ChessEngine::genBlackPMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	
	// Set en passant square
	bufferBoard = 0;
	if (cs->enPassantHistory[cs->moveNumber-1] != -1) {
		bufferBoard.setPosOn(cs->enPassantHistory[cs->moveNumber-1]);
	}

	bufferBoard.board |= cs->pieces[WHITE][ALL_PIECES].board;

	// ---- Right Kills ----
	killBoard = cs->pieces[BLACK][PAWN].board;
	// Remove all on 'h' file
	killBoard.board &= 9187201950435737471ULL;
	killBoard.board >>= 7;
	// Removes all but kill positions (en passant included)
	killBoard.board &= bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	U8 i(0);
	S8 killed;
	for ( ; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(WHITE, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed);
			++*moveCount;
		}
	}

	// ---- Left Kills ----
	killBoard.board = cs->pieces[BLACK][PAWN].board;
	// Remove all on 'a' file
	killBoard.board &= 18374403900871474942ULL;
	killBoard.board >>= 9;
	// Removes all but kill positions (en passant included)
	killBoard.board &= bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(WHITE, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed);
			++*moveCount;
		}
	}

	// --- Generate non-kill moves ---
	bufferBoard.board = ~(cs->pieces[WHITE][ALL_PIECES].board | cs->pieces[BLACK][ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[BLACK][PAWN].board >> 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	moveBoard.getPosArr(posTargets, &targetCount);

	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]+8, posTargets[i], -1, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+8, posTargets[i], -1, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+8, posTargets[i], -1, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+8, posTargets[i], -1, KNIGHT);
			++*moveCount;
		} else {
			moves[*moveCount] = Move(PAWN, posTargets[i]+8, posTargets[i]);
			++*moveCount;
		}
	}

	// Remove all except potential double moves
	moveBoard.board &= (255ULL << 40);
	moveBoard.board >>= 8;
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	moveBoard.popPosArr(posTargets, &targetCount);
	// Add positions to array
	for (i=0; i<targetCount; ++i) {
		moves[*moveCount] = Move(PAWN, posTargets[i]+16, posTargets[i]);
		++*moveCount;
	}
}

// Generates all psudo-legal pawn kill moves
void ChessEngine::genWhitePKillMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	
	// Set en passant square
	bufferBoard = 0;
	if (cs->enPassantHistory[cs->moveNumber-1] != -1) {
		bufferBoard.setPosOn(cs->enPassantHistory[cs->moveNumber-1]);
	}

	bufferBoard.board |= cs->pieces[BLACK][ALL_PIECES].board;

	// ---- Right Kills ----
	killBoard = cs->pieces[WHITE][PAWN].board;
	// Remove all on 'h' file
	killBoard.board &= 9187201950435737471ULL;
	killBoard.board <<= 9;
	// Removes all but kill positions (en passant included)
	killBoard.board &= bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	U8 i(0);
	S8 killed;
	for ( ; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(BLACK, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]-9, posTargets[i], killed);
			++*moveCount;
		}
	}

	// ---- Left Kills ----
	killBoard.board = cs->pieces[WHITE][PAWN].board;
	// Remove all on 'a' file
	killBoard.board &= 18374403900871474942ULL;
	killBoard.board <<= 7;
	// Removes all but kill positions (en passant included)
	killBoard.board &=  bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(BLACK, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]-7, posTargets[i], killed);
			++*moveCount;
		}
	}
}

// Generates all psudo-legal pawn moves
void ChessEngine::genBlackPKillMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	
	// Set en passant square
	bufferBoard = 0;
	if (cs->enPassantHistory[cs->moveNumber-1] != -1) {
		bufferBoard.setPosOn(cs->enPassantHistory[cs->moveNumber-1]);
	}

	bufferBoard.board |= cs->pieces[WHITE][ALL_PIECES].board;

	// ---- Right Kills ----
	killBoard = cs->pieces[BLACK][PAWN].board;
	// Remove all on 'h' file
	killBoard.board &= 9187201950435737471ULL;
	killBoard.board >>= 7;
	// Removes all but kill positions (en passant included)
	killBoard.board &= bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	U8 i(0);
	S8 killed;
	for ( ; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(WHITE, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]+7, posTargets[i], killed);
			++*moveCount;
		}
	}

	// ---- Left Kills ----
	killBoard.board = cs->pieces[BLACK][PAWN].board;
	// Remove all on 'a' file
	killBoard.board &= 18374403900871474942ULL;
	killBoard.board >>= 9;
	// Removes all but kill positions (en passant included)
	killBoard.board &= bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(WHITE, posTargets[i]);
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, QUEEN);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, ROOK);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, BISHOP);
			++*moveCount;
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed, KNIGHT);
			++*moveCount;
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = PAWN;
			}
			moves[*moveCount] = Move(PAWN, posTargets[i]+9, posTargets[i], killed);
			++*moveCount;
		}
	}
}