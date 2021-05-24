#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

#include <chrono>
using namespace std::chrono;
extern high_resolution_clock::time_point moveStart, moveEnd;
extern long long int moveTotal;

void ChessEngine::genAllMoves(ChessState* cs, vector<Move>* moves) {
	/* Generates all moves in a givin ChessState */

	moveStart = high_resolution_clock::now();

	genPMoves(cs, moves);
	genNMoves(cs, moves);
	genBMoves(cs, moves);
	genRMoves(cs, moves);
	genQMoves(cs, moves);
	genKMoves(cs, moves);

	moveEnd = high_resolution_clock::now();
	moveTotal += duration_cast<microseconds>(moveEnd - moveStart).count();
}
