
#include "chess_engine.hpp"
#include "chess_state.hpp"

// Generates all psudo-legal moves
void ChessEngine::genAllMoves(ChessState* cs, vector<Move>* moves) {
	if (cs->pieces[cs->turn][cs->PAWN].board) {
		if (cs->turn == cs->WHITE) {
			genWhitePMoves(cs, moves);
		} else {
			genBlackPMoves(cs, moves);
		}
	}
	if (cs->pieces[cs->turn][cs->KNIGHT].board) {
		genNMoves(cs, moves);
	}
	if (cs->pieces[cs->turn][cs->BISHOP].board) {
		genBMoves(cs, moves);
	}
	if (cs->pieces[cs->turn][cs->ROOK].board) {
		genRMoves(cs, moves);
	}
	if (cs->pieces[cs->turn][cs->QUEEN].board) {
		genQMoves(cs, moves);
	}
	genKMoves(cs, moves);
}
