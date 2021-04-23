
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

#include <iostream>	// TEMP
using namespace std;	// TEMP

void ChessEngine::genKMoves(ChessState* cs, vector<Move>* moves){
	/* Generates all legal king moves */

	U8 i;
	vector<Move> validMoves;
	U8 start;
	Bitboard target_board;
	vector<U8> targets;
	short killed;
	
	// Start position of the king
	// cout << "START" << endl;
	start = cs->pieces[cs->turn][cs->KING]->getFirstPos();
	// cout << "END" << endl;

	// Get surrounding squares
	target_board = KMoveDB.find(start)->second;
	// Remove squares with same coloured pieces
	target_board.board = target_board.board & (~cs->pieces[cs->turn][cs->ALL_PIECES]->board);	

	// TODO: CHECK IF TARGET SQUARE IS UNDER ATTACK

	// Positions of all target squares
	targets = target_board.getPosVector();

	// Add moves to vector
	for (i=0; i<targets.size(); ++i) {
		// Check for killing a piece
		if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(targets[i])) {
			killed = cs->getPieceType(!cs->turn, targets[i]);
		} else {
			killed = -1;	// Default
		}

		// Add to list of valid moves
		moves->push_back(Move(cs->KING, start, targets[i], killed));
	}
}
