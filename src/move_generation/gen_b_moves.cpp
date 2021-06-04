
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

void ChessEngine::genBMoves(ChessState* cs, vector<Move>* moves) {
	// Get piece locations
	vector<U8> start;
	cs->pieces[cs->turn][cs->BISHOP].getPosVector(&start);

	U8 i, j;
	Bitboard occ;
	Bitboard targets;
	vector<U8> end;
	for (i=0; i<start.size(); ++i) {
		// Find all potential squares
		occ.board = cs->pieces[0][6].board | cs->pieces[1][6].board;
		occ.board &= bishopMasks[start[i]].board;
		occ.board *= bishopMagics[start[i]].board;
		occ.board >>= bishopMagicShifts[start[i]];
		occ = bishopAttackTable[start[i]][occ.board];
		// Remove friendly targets
		occ.board &= ~(cs->pieces[cs->turn][6].board);
		// Seperate enemy targets
		targets.board = occ.board & cs->pieces[!cs->turn][6].board;
		// Remove enemies from occ
		occ.board &= ~(cs->pieces[!cs->turn][6].board);

		// Add non-kill moves
		if (occ.board != 0) {
			end = occ.getPosVector();
			for (j=0; j<end.size(); ++j) {
				moves->push_back(Move(cs->BISHOP, start[i], end[j]));
			}
		}

		// Add kill moves
		if (targets.board != 0) {
			end = targets.getPosVector();
			for (j=0; j<end.size(); ++j) {
				moves->push_back(Move(cs->BISHOP, start[i], end[j], cs->getPieceType(!cs->turn, end[j])));
			}
		}
	}
}
