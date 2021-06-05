
#include "chess_engine.hpp"
#include "chess_state.hpp"

void ChessEngine::genAllMoves(ChessState* cs, vector<Move>* moves) {
	/* Generates all moves in a givin ChessState */

	if (cs->pieces[cs->turn][cs->PAWN].board) {
		genPMoves(cs, moves);
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
