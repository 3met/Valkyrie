
#include "bitboard.hpp"
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Check if a square is attacked "turn" represents the color of the attacker
bool ChessEngine::isPosAttacked(ChessState* cs, bool turn, U8 pos) {
	// Check if attacked by knight
	if ((NMoveDB[pos].board & cs->pieces[turn][KNIGHT].board) != 0) {
		return true;
	}
	// Check if attacked by king
	if ((KMoveDB[pos].board & cs->pieces[turn][KING].board) != 0) {
		return true;
	}

	// --- Bishop-like moves ---
	Bitboard occ(cs->pieces[0][6].board | cs->pieces[1][6].board);
	// Find all potential squares
	occ.board &= bishopMasks[pos].board;
	occ.board *= bishopMagics[pos].board;
	occ.board >>= bishopMagicShifts[pos];
	occ = bishopAttackTable[pos][occ.board];
	// Returns true if a diagonal move away from attacking bishop or queen
	if ((occ.board & cs->pieces[turn][BISHOP].board) != 0
		|| (occ.board & cs->pieces[turn][QUEEN].board) != 0) {

		return true;
	}

	// --- Rook-like moves ---
	occ.board = cs->pieces[0][6].board | cs->pieces[1][6].board;
	// Find all potential squares
	occ.board &= rookMasks[pos].board;
	occ.board *= rookMagics[pos].board;
	occ.board >>= rookMagicShifts[pos];
	occ = rookAttackTable[pos][occ.board];
	// Returns true if a diagonal move away from attacking bishop or queen
	if ((occ.board & cs->pieces[turn][ROOK].board) != 0
		|| (occ.board & cs->pieces[turn][QUEEN].board) != 0) {

		return true;
	}

	// Check pawn attacks
	if (turn == WHITE) {
		if (BOARD_RANK[pos] >= 2) {
			// Right and left pawn attacks
			if (BOARD_FILE[pos] >= 1 && cs->pieces[WHITE][PAWN].getPos(pos-9)) {
				return true;
			}
			if (BOARD_FILE[pos] <= 6 && cs->pieces[WHITE][PAWN].getPos(pos-7)) {
				return true;
			}
		}
	} else {	// Black's attacking
		if (BOARD_RANK[pos] <= 5) {
			// Right and left pawn attacks
			if (BOARD_FILE[pos] >= 1 && cs->pieces[BLACK][PAWN].getPos(pos+7)) {
				return true;
			}
			if (BOARD_FILE[pos] <= 6 && cs->pieces[BLACK][PAWN].getPos(pos+9)) {
				return true;
			}
		}
	}

	return false;
}
