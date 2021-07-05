
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "S8.hpp"
#include "U8.hpp"
#include "U64.hpp"

// Generates all psudo-legal pawn moves
void ChessEngine::genWhitePMoves(ChessState* cs, vector<Move>* moves) {
	
	// Set en passant square
	bufferBoard = 0;
	if (cs->enPassant != -1) {
		bufferBoard.setPosOn(cs->enPassant);
	}

	bufferBoard.board |= cs->pieces[cs->BLACK][cs->ALL_PIECES].board;

	// ---- Right Kills ----
	killBoard = cs->pieces[cs->WHITE][cs->PAWN].board;
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
		killed = cs->getPieceType(cs->BLACK, posTargets[i]);
		// Check pawn promotion
		if (Bitboard::RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves->push_back(Move(cs->PAWN, posTargets[i]-9, posTargets[i], killed, cs->QUEEN));
			moves->push_back(Move(cs->PAWN, posTargets[i]-9, posTargets[i], killed, cs->ROOK));
			moves->push_back(Move(cs->PAWN, posTargets[i]-9, posTargets[i], killed, cs->BISHOP));
			moves->push_back(Move(cs->PAWN, posTargets[i]-9, posTargets[i], killed, cs->KNIGHT));
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = cs->PAWN;
			}
			moves->push_back(Move(cs->PAWN, posTargets[i]-9, posTargets[i], killed));
		}
	}

	// ---- Left Kills ----
	killBoard.board = cs->pieces[cs->WHITE][cs->PAWN].board;
	// Remove all on 'a' file
	killBoard.board &= 18374403900871474942ULL;
	killBoard.board <<= 7;
	// Removes all but kill positions (en passant included)
	killBoard.board &=  bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(cs->BLACK, posTargets[i]);
		// Check pawn promotion
		if (Bitboard::RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves->push_back(Move(cs->PAWN, posTargets[i]-7, posTargets[i], killed, cs->QUEEN));
			moves->push_back(Move(cs->PAWN, posTargets[i]-7, posTargets[i], killed, cs->ROOK));
			moves->push_back(Move(cs->PAWN, posTargets[i]-7, posTargets[i], killed, cs->BISHOP));
			moves->push_back(Move(cs->PAWN, posTargets[i]-7, posTargets[i], killed, cs->KNIGHT));
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = cs->PAWN;
			}
			moves->push_back(Move(cs->PAWN, posTargets[i]-7, posTargets[i], killed));
		}
	}

	// --- Generate non-kill moves ---
	bufferBoard.board = ~(cs->pieces[cs->WHITE][cs->ALL_PIECES].board | cs->pieces[cs->BLACK][cs->ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[cs->WHITE][cs->PAWN].board << 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	moveBoard.getPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (Bitboard::RANK[posTargets[i]] == 7) {
			// Add a moves for each possible promotion type
			moves->push_back(Move(cs->PAWN, posTargets[i]-8, posTargets[i], -1, cs->QUEEN));
			moves->push_back(Move(cs->PAWN, posTargets[i]-8, posTargets[i], -1, cs->ROOK));
			moves->push_back(Move(cs->PAWN, posTargets[i]-8, posTargets[i], -1, cs->BISHOP));
			moves->push_back(Move(cs->PAWN, posTargets[i]-8, posTargets[i], -1, cs->KNIGHT));
		} else {
			moves->push_back(Move(cs->PAWN, posTargets[i]-8, posTargets[i]));
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
		moves->push_back(Move(cs->PAWN, posTargets[i]-16, posTargets[i]));
	}
}

// Generates all psudo-legal pawn moves
void ChessEngine::genBlackPMoves(ChessState* cs, vector<Move>* moves) {
	
	// Set en passant square
	bufferBoard = 0;
	if (cs->enPassant != -1) {
		bufferBoard.setPosOn(cs->enPassant);
	}

	bufferBoard.board |= cs->pieces[cs->WHITE][cs->ALL_PIECES].board;

	// ---- Right Kills ----
	killBoard = cs->pieces[cs->BLACK][cs->PAWN].board;
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
		killed = cs->getPieceType(cs->WHITE, posTargets[i]);
		// Check pawn promotion
		if (Bitboard::RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves->push_back(Move(cs->PAWN, posTargets[i]+7, posTargets[i], killed, cs->QUEEN));
			moves->push_back(Move(cs->PAWN, posTargets[i]+7, posTargets[i], killed, cs->ROOK));
			moves->push_back(Move(cs->PAWN, posTargets[i]+7, posTargets[i], killed, cs->BISHOP));
			moves->push_back(Move(cs->PAWN, posTargets[i]+7, posTargets[i], killed, cs->KNIGHT));
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = cs->PAWN;
			}
			moves->push_back(Move(cs->PAWN, posTargets[i]+7, posTargets[i], killed));
		}
	}

	// ---- Left Kills ----
	killBoard.board = cs->pieces[cs->BLACK][cs->PAWN].board;
	// Remove all on 'a' file
	killBoard.board &= 18374403900871474942ULL;
	killBoard.board >>= 9;
	// Removes all but kill positions (en passant included)
	killBoard.board &= bufferBoard.board;
	// Add moves to array
	killBoard.popPosArr(posTargets, &targetCount);
	for (i=0; i<targetCount; ++i) {
		// Type of piece killed
		killed = cs->getPieceType(cs->WHITE, posTargets[i]);
		// Check pawn promotion
		if (Bitboard::RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves->push_back(Move(cs->PAWN, posTargets[i]+9, posTargets[i], killed, cs->QUEEN));
			moves->push_back(Move(cs->PAWN, posTargets[i]+9, posTargets[i], killed, cs->ROOK));
			moves->push_back(Move(cs->PAWN, posTargets[i]+9, posTargets[i], killed, cs->BISHOP));
			moves->push_back(Move(cs->PAWN, posTargets[i]+9, posTargets[i], killed, cs->KNIGHT));
		} else {
			// Account for en passant
			if (killed == -1) {
				killed = cs->PAWN;
			}
			moves->push_back(Move(cs->PAWN, posTargets[i]+9, posTargets[i], killed));
		}
	}

	// --- Generate non-kill moves ---
	bufferBoard.board = ~(cs->pieces[cs->WHITE][cs->ALL_PIECES].board | cs->pieces[cs->BLACK][cs->ALL_PIECES].board);

	// Get potential single move squares
	moveBoard.board = (cs->pieces[cs->BLACK][cs->PAWN].board >> 8);
	// Remove occupied squares
	moveBoard.board &= bufferBoard.board;
	// Get end positions
	moveBoard.getPosArr(posTargets, &targetCount);

	for (i=0; i<targetCount; ++i) {
		// Check pawn promotion
		if (Bitboard::RANK[posTargets[i]] == 0) {
			// Add a moves for each possible promotion type
			moves->push_back(Move(cs->PAWN, posTargets[i]+8, posTargets[i], -1, cs->QUEEN));
			moves->push_back(Move(cs->PAWN, posTargets[i]+8, posTargets[i], -1, cs->ROOK));
			moves->push_back(Move(cs->PAWN, posTargets[i]+8, posTargets[i], -1, cs->BISHOP));
			moves->push_back(Move(cs->PAWN, posTargets[i]+8, posTargets[i], -1, cs->KNIGHT));
		} else {
			moves->push_back(Move(cs->PAWN, posTargets[i]+8, posTargets[i]));
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
		moves->push_back(Move(cs->PAWN, posTargets[i]+16, posTargets[i]));
	}
}

