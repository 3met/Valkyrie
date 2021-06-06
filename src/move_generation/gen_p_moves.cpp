
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "S8.hpp"
#include "U64.hpp"
#include "U8.hpp"

/* Generates all legal pawn moves */
void ChessEngine::genPMoves(ChessState* cs, vector<Move>* moves) {
	U8 i;
	Bitboard pos_board;
	vector<U8> pos_targets;
	S8 killed;
	Bitboard buffer;
	
	// Set en passant square
	if (cs->enPassant != -1) {
		buffer.setPos(cs->enPassant, true);
	}

	buffer.board |= cs->pieces[!cs->turn][cs->ALL_PIECES].board;

	if (cs->turn == cs->WHITE) {
		// ---- Right Kills ----
		pos_board.board = cs->pieces[cs->turn][cs->PAWN].board;
		// Remove all on 'h' file
		pos_board.board &= 9187201950435737471ULL;
		pos_board.board <<= 9;
		// Removes all but kill positions (en passant included)
		pos_board.board &= buffer.board;
		// Add moves to vector
		pos_targets = pos_board.getPosVector();
		for (i=0; i<pos_targets.size(); ++i) {
			// Type of piece killed
			killed = cs->getPieceType(!cs->turn, pos_targets[i]);
			// Check pawn promotion
			if (Bitboard::RANK[pos_targets[i]] == 7) {
				// Add a moves for each possible promotion type
				moves->push_back(Move(cs->PAWN, pos_targets[i]-9, pos_targets[i], killed, cs->QUEEN));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-9, pos_targets[i], killed, cs->ROOK));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-9, pos_targets[i], killed, cs->BISHOP));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-9, pos_targets[i], killed, cs->KNIGHT));
			} else {
				// Account for en passant
				if (killed == -1) {
					killed = cs->PAWN;
				}
				moves->push_back(Move(cs->PAWN, pos_targets[i]-9, pos_targets[i], killed));
			}
		}

		// ---- Left Kills ----
		pos_board.board = cs->pieces[cs->turn][cs->PAWN].board;
		// Remove all on 'a' file
		pos_board.board &= 18374403900871474942ULL;
		pos_board.board <<= 7;
		// Removes all but kill positions (en passant included)
		pos_board.board &=  buffer.board;
		// Add moves to vector
		pos_targets = pos_board.getPosVector();
		for (i=0; i<pos_targets.size(); ++i) {
			// Type of piece killed
			killed = cs->getPieceType(!cs->turn, pos_targets[i]);
			// Check pawn promotion
			if (Bitboard::RANK[pos_targets[i]] == 7) {
				// Add a moves for each possible promotion type
				moves->push_back(Move(cs->PAWN, pos_targets[i]-7, pos_targets[i], killed, cs->QUEEN));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-7, pos_targets[i], killed, cs->ROOK));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-7, pos_targets[i], killed, cs->BISHOP));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-7, pos_targets[i], killed, cs->KNIGHT));
			} else {
				// Account for en passant
				if (killed == -1) {
					killed = cs->PAWN;
				}
				moves->push_back(Move(cs->PAWN, pos_targets[i]-7, pos_targets[i], killed));
			}
		}

	} else {	// If black's turn
		// ---- Right Kills ----
		pos_board.board = cs->pieces[cs->turn][cs->PAWN].board;
		// Remove all on 'h' file
		pos_board.board &= 9187201950435737471ULL;
		pos_board.board >>= 7;
		// Removes all but kill positions (en passant included)
		pos_board.board &= buffer.board;
		// Add moves to vector
		pos_targets = pos_board.getPosVector();
		for (i=0; i<pos_targets.size(); ++i) {
			// Type of piece killed
			killed = cs->getPieceType(!cs->turn, pos_targets[i]);
			// Check pawn promotion
			if (Bitboard::RANK[pos_targets[i]] == 0) {
				// Add a moves for each possible promotion type
				moves->push_back(Move(cs->PAWN, pos_targets[i]+7, pos_targets[i], killed, cs->QUEEN));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+7, pos_targets[i], killed, cs->ROOK));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+7, pos_targets[i], killed, cs->BISHOP));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+7, pos_targets[i], killed, cs->KNIGHT));
			} else {
				// Account for en passant
				if (killed == -1) {
					killed = cs->PAWN;
				}
				moves->push_back(Move(cs->PAWN, pos_targets[i]+7, pos_targets[i], killed));
			}
		}

		// ---- Left Kills ----
		pos_board.board = cs->pieces[cs->turn][cs->PAWN].board;
		// Remove all on 'a' file
		pos_board.board &= 18374403900871474942ULL;
		pos_board.board >>= 9;
		// Removes all but kill positions (en passant included)
		pos_board.board &= buffer.board;
		// Add moves to vector
		pos_targets = pos_board.getPosVector();
		for (i=0; i<pos_targets.size(); ++i) {
			// Type of piece killed
			killed = cs->getPieceType(!cs->turn, pos_targets[i]);
			// Check pawn promotion
			if (Bitboard::RANK[pos_targets[i]] == 0) {
				// Add a moves for each possible promotion type
				moves->push_back(Move(cs->PAWN, pos_targets[i]+9, pos_targets[i], killed, cs->QUEEN));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+9, pos_targets[i], killed, cs->ROOK));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+9, pos_targets[i], killed, cs->BISHOP));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+9, pos_targets[i], killed, cs->KNIGHT));
			} else {
				// Account for en passant
				if (killed == -1) {
					killed = cs->PAWN;
				}
				moves->push_back(Move(cs->PAWN, pos_targets[i]+9, pos_targets[i], killed));
			}
		}
	}

	// --- Generate non-kill moves ---
	buffer.board = ~(cs->pieces[cs->WHITE][cs->ALL_PIECES].board | cs->pieces[cs->BLACK][cs->ALL_PIECES].board);

	if (cs->turn == cs->WHITE) {
		// Get potential single move squares
		pos_board.board = (cs->pieces[cs->turn][cs->PAWN].board << 8);
		// Remove occupied squares
		pos_board.board &= buffer.board;
		// Get end positions
		pos_targets = pos_board.getPosVector();
		for (i=0; i<pos_targets.size(); ++i) {
			// Check pawn promotion
			if (Bitboard::RANK[pos_targets[i]] == 7) {
				// Add a moves for each possible promotion type
				moves->push_back(Move(cs->PAWN, pos_targets[i]-8, pos_targets[i], -1, cs->QUEEN));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-8, pos_targets[i], -1, cs->ROOK));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-8, pos_targets[i], -1, cs->BISHOP));
				moves->push_back(Move(cs->PAWN, pos_targets[i]-8, pos_targets[i], -1, cs->KNIGHT));
			} else {
				moves->push_back(Move(cs->PAWN, pos_targets[i]-8, pos_targets[i]));
			}
		}

		// Remove all except potential double moves
		pos_board.board &= (255 << 16);
		pos_board.board <<= 8;
		// Remove occupied squares
		pos_board.board &= buffer.board;
		// Get end positions
		pos_targets = pos_board.getPosVector();
		// Add positions to vector
		for (i=0; i<pos_targets.size(); ++i) {
			moves->push_back(Move(cs->PAWN, pos_targets[i]-16, pos_targets[i]));
		}
	} else {	// Black's turn
		// Get potential single move squares
		pos_board.board = (cs->pieces[cs->turn][cs->PAWN].board >> 8);
		// Remove occupied squares
		pos_board.board &= buffer.board;
		// Get end positions
		pos_targets = pos_board.getPosVector();

		for (i=0; i<pos_targets.size(); ++i) {
			// Check pawn promotion
			if (Bitboard::RANK[pos_targets[i]] == 0) {
				// Add a moves for each possible promotion type
				moves->push_back(Move(cs->PAWN, pos_targets[i]+8, pos_targets[i], -1, cs->QUEEN));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+8, pos_targets[i], -1, cs->ROOK));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+8, pos_targets[i], -1, cs->BISHOP));
				moves->push_back(Move(cs->PAWN, pos_targets[i]+8, pos_targets[i], -1, cs->KNIGHT));
			} else {
				moves->push_back(Move(cs->PAWN, pos_targets[i]+8, pos_targets[i]));
			}
		}

		// Remove all except potential double moves
		pos_board.board &= (255ULL << 40);
		pos_board.board >>= 8;
		// Remove occupied squares
		pos_board.board &= buffer.board;
		// Get end positions
		pos_targets = pos_board.getPosVector();
		// Add positions to vector
		for (i=0; i<pos_targets.size(); ++i) {
			moves->push_back(Move(cs->PAWN, pos_targets[i]+16, pos_targets[i]));
		}
	}
}
