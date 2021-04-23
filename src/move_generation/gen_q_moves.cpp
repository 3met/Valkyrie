#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

/* Generates all legal Queen moves */
void ChessEngine::genQMoves(ChessState* cs, vector<Move>* moves){
	U8 i, j;
	U8 pos;
	vector<U8> start;
	vector<short> kill_targets;
	short killed;
	
	// Get all queen locations
	start = cs->pieces[cs->turn][cs->QUEEN]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		kill_targets.clear();

		// Upward moves
		pos = start[i];
		while (Bitboard::RANK[pos] != 7) {	// Not top row
			// If an enemy piece at target position
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos+8)) {
				kill_targets.push_back(pos+8);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos+8)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos+8));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 8;
		}

		// Downward moves
		pos = start[i];
		while (Bitboard::RANK[pos] != 0) {	// Not bottom row
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos-8)) {
				kill_targets.push_back(pos-8);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos-8)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos-8));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 8;
		}

		// Leftward moves
		pos = start[i];
		while (Bitboard::FILE[pos] != 0) {	// Not leftmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos-1)) {
				kill_targets.push_back(pos-1);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos-1)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos-1));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 1;
		}

		// Rightward moves
		pos = start[i];
		while (Bitboard::FILE[pos] != 7) {	// Not rightmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos+1)) {
				kill_targets.push_back(pos+1);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos+1)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos+1));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 1;
		}

		// Up-right movement
		pos = start[i];
		while (Bitboard::RANK[pos] != 7 && Bitboard::FILE[pos] != 7) {	// Not top row or right col
			// If an enemy piece at target position
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos+9)) {
				kill_targets.push_back(pos+9);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos+9)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos+9));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 9;
		}

		// Down-right moves
		pos = start[i];
		while (Bitboard::RANK[pos] != 0 && Bitboard::FILE[pos] != 7) {	// Not bottom row or right col
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos-7)) {
				kill_targets.push_back(pos-7);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos-7)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos-7));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 7;
		}

		// Down-left moves
		pos = start[i];
		while (Bitboard::RANK[pos] != 0 && Bitboard::FILE[pos] != 0) {	// Not bottom row or left col
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos-9)) {
				kill_targets.push_back(pos-9);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos-9)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos-9));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 9;
		}

		// Up-left moves
		pos = start[i];
		while (Bitboard::RANK[pos] != 7 && Bitboard::FILE[pos] != 0) {	// Not rightmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->ALL_PIECES]->getPos(pos+7)) {
				kill_targets.push_back(pos+7);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->ALL_PIECES]->getPos(pos+7)) {
				moves->push_back(Move(cs->QUEEN, start[i], pos+7));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 7;
		}

		// TODO: Make sure king is safe after move

		// All possible kill moves
		for (j=0; j<kill_targets.size(); ++j) {
			// Check for killing a piece
			killed = cs->getPieceType(!cs->turn, kill_targets[j]);
			moves->push_back(Move(cs->QUEEN, start[i], kill_targets[j], killed));
		}
	}
}
