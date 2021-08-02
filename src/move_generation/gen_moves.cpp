
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Generates all psudo-legal moves
void ChessEngine::genAllMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	*moveCount = 0;
	if (cs->pieces[cs->turn][PAWN].board) {
		if (cs->turn == WHITE) {
			genWhitePMoves(cs, moves, moveCount);
		} else {
			genBlackPMoves(cs, moves, moveCount);
		}
	}
	if (cs->pieces[cs->turn][KNIGHT].board) {
		genNMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][BISHOP].board) {
		genBMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][ROOK].board) {
		genRMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][QUEEN].board) {
		genQMoves(cs, moves, moveCount);
	}
	genKMoves(cs, moves, moveCount);
}

// Generates all psudo-legal moves
void ChessEngine::genAllKillMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	*moveCount = 0;
	if (cs->pieces[cs->turn][PAWN].board) {
		if (cs->turn == WHITE) {
			genWhitePKillMoves(cs, moves, moveCount);
		} else {
			genBlackPKillMoves(cs, moves, moveCount);
		}
	}
	if (cs->pieces[cs->turn][KNIGHT].board) {
		genNKillMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][BISHOP].board) {
		genBKillMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][ROOK].board) {
		genRKillMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][QUEEN].board) {
		genQKillMoves(cs, moves, moveCount);
	}
	genKKillMoves(cs, moves, moveCount);
}

