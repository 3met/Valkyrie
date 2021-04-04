#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

void ChessEngine::genAllMoves(ChessState* cs, vector<Move>* moves) {
	/* Generates all moves in a givin ChessState */

	genPMoves(cs, moves);
	genNMoves(cs, moves);
	genBMoves(cs, moves);
	genRMoves(cs, moves);
	genQMoves(cs, moves);
	genKMoves(cs, moves);
}
