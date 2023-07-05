
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all pseudo-legal pawn moves
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
	targetCount = killBoard.popPosArr(posTargets);
	U8 i(0);
	for ( ; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 5
				&& cs->getPieceType(BLACK, posTargets[i]) == NULL_PIECE) {
				
				moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
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
	targetCount = killBoard.popPosArr(posTargets);
	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 5
				&& cs->getPieceType(BLACK, posTargets[i]) == NULL_PIECE) {
				
				moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
		}
	}

	// --- Generate non-kill moves ---
	bufferBoard.board = ~(cs->pieces[WHITE][ALL_PIECES].board | cs->pieces[BLACK][ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[WHITE][PAWN].board << 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	targetCount = moveBoard.getPosArr(posTargets);
	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::QUEEN_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::KNIGHT_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::ROOK_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::BISHOP_PROMO, PAWN);
			++*moveCount;
		} else {
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::QUIET, PAWN);
			++*moveCount;
		}
	}

	// Remove all except potential double moves
	moveBoard.board &= (255 << 16);
	moveBoard.board <<= 8;
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	targetCount = moveBoard.popPosArr(posTargets);
	// Add positions to array
	for (i=0; i<targetCount; ++i) {
		moves[*moveCount] = Move(posTargets[i]-16, posTargets[i], Move::QUIET, PAWN);
		++*moveCount;
	}
}

// Generates all pseudo-legal pawn moves
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
	targetCount = killBoard.popPosArr(posTargets);
	U8 i(0);
	for ( ; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 2
				&& cs->getPieceType(WHITE, posTargets[i]) == NULL_PIECE) {
				
				moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
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
	targetCount = killBoard.popPosArr(posTargets);
	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 2
				&& cs->getPieceType(WHITE, posTargets[i]) == NULL_PIECE) {

				moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
		}
	}

	// --- Generate non-kill moves ---
	bufferBoard.board = ~(cs->pieces[WHITE][ALL_PIECES].board | cs->pieces[BLACK][ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[BLACK][PAWN].board >> 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	targetCount = moveBoard.getPosArr(posTargets);

	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::QUEEN_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::KNIGHT_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::ROOK_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::BISHOP_PROMO, PAWN);
			++*moveCount;
		} else {
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::QUIET, PAWN);
			++*moveCount;
		}
	}

	// Remove all except potential double moves
	moveBoard.board &= (255ULL << 40);
	moveBoard.board >>= 8;
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	targetCount = moveBoard.popPosArr(posTargets);
	// Add positions to array
	for (i=0; i<targetCount; ++i) {
		moves[*moveCount] = Move(posTargets[i]+16, posTargets[i], Move::QUIET, PAWN);
		++*moveCount;
	}
}

// Generates all pseudo-legal pawn kill moves
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
	targetCount = killBoard.popPosArr(posTargets);
	U8 i(0);
	for ( ; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 5
				&& cs->getPieceType(BLACK, posTargets[i]) == NULL_PIECE) {
				
				moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]-9, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
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
	targetCount = killBoard.popPosArr(posTargets);
	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 5
				&& cs->getPieceType(BLACK, posTargets[i]) == NULL_PIECE) {
				
				moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]-7, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
		}
	}
}

// Generates all pseudo-legal pawn moves
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
	targetCount = killBoard.popPosArr(posTargets);
	U8 i(0);
	for ( ; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 2
				&& cs->getPieceType(WHITE, posTargets[i]) == NULL_PIECE) {
				
				moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]+7, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
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
	targetCount = killBoard.popPosArr(posTargets);
	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::QUEEN_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::KNIGHT_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::ROOK_PROMO_CAPTURE, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::BISHOP_PROMO_CAPTURE, PAWN);
			++*moveCount;
		} else {
			// Account for en passant
			if (BOARD_RANK[posTargets[i]] == 2
				&& cs->getPieceType(WHITE, posTargets[i]) == NULL_PIECE) {

				moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::EP_CAPTURE, PAWN);
				++*moveCount;
			} else {
				moves[*moveCount] = Move(posTargets[i]+9, posTargets[i], Move::CAPTURE, PAWN);
				++*moveCount;
			}
		}
	}
}

// Generates all pseudo-legal non-capture promotions for white
void ChessEngine::genWhiteMovePromotion(ChessState* cs, Move moves[218], U8* moveCount) {

	bufferBoard.board = ~(cs->pieces[WHITE][ALL_PIECES].board | cs->pieces[BLACK][ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[WHITE][PAWN].board << 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	targetCount = moveBoard.getPosArr(posTargets);
	for (U8 i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::QUEEN_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::KNIGHT_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::ROOK_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]-8, posTargets[i], Move::BISHOP_PROMO, PAWN);
			++*moveCount;
		}
	}
}

// Generates all pseudo-legal non-capture promotions for white
void ChessEngine::genBlackMovePromotion(ChessState* cs, Move moves[218], U8* moveCount) {
	// --- Generate non-kill moves ---
	bufferBoard.board = ~(cs->pieces[WHITE][ALL_PIECES].board | cs->pieces[BLACK][ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[BLACK][PAWN].board >> 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	targetCount = moveBoard.getPosArr(posTargets);

	for (U8 i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (BOARD_RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::QUEEN_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::KNIGHT_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::ROOK_PROMO, PAWN);
			++*moveCount;
			moves[*moveCount] = Move(posTargets[i]+8, posTargets[i], Move::BISHOP_PROMO, PAWN);
			++*moveCount;
		}
	}
}
