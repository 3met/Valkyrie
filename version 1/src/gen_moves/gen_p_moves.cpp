#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

void ChessEngine::genPMoves(ChessState* cs, vector<Move>* moves) {
	/* Generates all legal pawn moves */

	U8 i, j;
	vector<U8> start;
	Bitboard move_board;
	Bitboard kill_board;
	vector<U8> move_targets;
	vector<U8> kill_targets;
	short killed;
	
	// Get all pawn locations
	start = cs->pieces[cs->turn][cs->PAWN]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		// Get potential squares
		move_board.board = 0;
		kill_board.board = 0;

		// Add square in front of pawn
		if (cs->turn == cs->WHITE)
			move_board.setPos(start[i]+8, true);
		else
			move_board.setPos(start[i]-8, true);

		// Remove if blocked by enemy piece
		move_board.board &= ~(cs->pieces[!cs->turn][cs->ALL_PIECES]->board);
		
		// Remove if blocked by friendly piece
		if (move_board.board != 0) {
			move_board.board &= ~(cs->pieces[cs->turn][cs->ALL_PIECES]->board);
		}

		if (cs->turn == cs->WHITE) {
			// Try two squares up if can move up one and on home row
			if (move_board.board != 0 && start[i] >= 8 && start[i] <= 15) {
				move_board.setPos(start[i]+16, true);
				// Remove square if occupied
				move_board.board &= ~(cs->pieces[cs->WHITE][cs->ALL_PIECES]->board);
				move_board.board &= ~(cs->pieces[cs->BLACK][cs->ALL_PIECES]->board);
			}
			// Add left kill position if not on a column
			if (start[i] % 8 != 0) {
				kill_board.setPos(start[i]+7, true);
			}
			// Add right kill position if not on h column
			if (start[i] % 8 != 7) {
				kill_board.setPos(start[i]+9, true);
			}
		} else {	// Black's turn
			// Try two squares up if can move up one and on home row
			if (move_board.board != 0 && start[i] >= 48 && start[i] <= 55) {
				move_board.setPos(start[i]-16, true);
				// Remove square if occupied
				move_board.board &= ~(cs->pieces[cs->WHITE][cs->ALL_PIECES]->board);
				move_board.board &= ~(cs->pieces[cs->BLACK][cs->ALL_PIECES]->board);
			}
			// Add left kill position if not on a column
			if (start[i] % 8 != 0) {
				kill_board.setPos(start[i]-9, true);
			}
			// Add right kill position if not on h column
			if (start[i] % 8 != 7) {
				kill_board.setPos(start[i]-7, true);
			}
		}

		// Remove any kill positions without enemy pieces
		kill_board.board &= cs->pieces[!cs->turn][cs->ALL_PIECES]->board;

		// All squares that can be killed/moved to
		move_targets = move_board.getPosVector();
		kill_targets = kill_board.getPosVector();

		// TODO: Make sure king is safe after move

		S8 promotion;
		// All possible non-kill moves
		for (j=0; j<move_targets.size(); ++j) {
			// Check pawn promotion
			if (move_targets[j] >= 56 || move_targets[j] <= 7) {
				promotion = 4;	// Auto queen
			} else {
				promotion = -1;
			}
			moves->push_back(Move(cs->PAWN, start[i], move_targets[j], -1, promotion));
		}

		// All possible kill moves
		for (j=0; j<kill_targets.size(); ++j) {
			// Check pawn promotion
			if (kill_targets[j] >= 56 || kill_targets[j] <= 7) {
				promotion = 4;	// Auto queen
			} else {
				promotion = -1;
			}
			// Check for killing a piece
			killed = cs->getPieceType(!cs->turn, kill_targets[j]);
			moves->push_back(Move(cs->PAWN, start[i], kill_targets[j], killed, promotion));
		}
	}


}
