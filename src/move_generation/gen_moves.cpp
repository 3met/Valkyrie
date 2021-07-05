
#include "chess_engine.hpp"
#include "chess_state.hpp"

// Generates all psudo-legal moves
void ChessEngine::genAllMoves(ChessState* cs, Move moves[218], U8* moveCount) {
	*moveCount = 0;
	if (cs->pieces[cs->turn][cs->PAWN].board) {
		if (cs->turn == cs->WHITE) {
			genWhitePMoves(cs, moves, moveCount);
		} else {
			genBlackPMoves(cs, moves, moveCount);
		}
	}
	if (cs->pieces[cs->turn][cs->KNIGHT].board) {
		genNMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][cs->BISHOP].board) {
		genBMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][cs->ROOK].board) {
		genRMoves(cs, moves, moveCount);
	}
	if (cs->pieces[cs->turn][cs->QUEEN].board) {
		genQMoves(cs, moves, moveCount);
	}
	genKMoves(cs, moves, moveCount);
}
