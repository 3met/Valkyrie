
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

// Generates all psudo-legal queen moves
void ChessEngine::genQMoves(ChessState* cs, vector<Move>* moves) {
	// Get piece locations
	vector<U8> start;
	cs->pieces[cs->turn][cs->QUEEN].getPosVec(&start);

	U8 i, j;
	U64 occ;
	Bitboard open;
	Bitboard targets;
	vector<U8> end;
	for (i=0; i<start.size(); ++i) {
		occ = cs->pieces[0][6].board | cs->pieces[1][6].board;
		// --- Bishop-like moves ---
		// Find all potential squares
		open.board = occ;
		open.board &= bishopMasks[start[i]].board;
		open.board *= bishopMagics[start[i]].board;
		open.board >>= bishopMagicShifts[start[i]];
		open = bishopAttackTable[start[i]][open.board];
		// Remove friendly targets
		open.board &= ~(cs->pieces[cs->turn][6].board);
		// Seperate enemy targets
		targets.board = open.board & cs->pieces[!cs->turn][6].board;
		// Remove enemies from open
		open.board &= ~(cs->pieces[!cs->turn][6].board);

		// Add non-kill moves
		if (open.board != 0) {
			end = open.popPosVec();
			for (j=0; j<end.size(); ++j) {
				moves->push_back(Move(cs->QUEEN, start[i], end[j]));
			}
		}
		// Add kill moves
		if (targets.board != 0) {
			end = targets.popPosVec();
			for (j=0; j<end.size(); ++j) {
				moves->push_back(Move(cs->QUEEN, start[i], end[j], cs->getPieceType(!cs->turn, end[j])));
			}
		}

		// --- Rook-like moves ---
		// Find all potential squares
		open.board = occ;
		open.board &= rookMasks[start[i]].board;
		open.board *= rookMagics[start[i]].board;
		open.board >>= rookMagicShifts[start[i]];
		open = rookAttackTable[start[i]][open.board];
		// Remove friendly targets
		open.board &= ~(cs->pieces[cs->turn][6].board);
		// Seperate enemy targets
		targets.board = open.board & cs->pieces[!cs->turn][6].board;
		// Remove enemies from open
		open.board &= ~(cs->pieces[!cs->turn][6].board);

		// Add non-kill moves
		if (open.board != 0) {
			end = open.popPosVec();
			for (j=0; j<end.size(); ++j) {
				moves->push_back(Move(cs->QUEEN, start[i], end[j]));
			}
		}
		// Add kill moves
		if (targets.board != 0) {
			end = targets.popPosVec();
			for (j=0; j<end.size(); ++j) {
				moves->push_back(Move(cs->QUEEN, start[i], end[j], cs->getPieceType(!cs->turn, end[j])));
			}
		}
	}
}
