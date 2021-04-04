
/* --- In this File ---
 * 1. ChessEngine Constructor
 * 2. Calculating Psudo-legal piece moves
 * 3. Calculating material value and advantages
 * 4. Calculate the best move from a given game state */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "U64.hpp"

using namespace std;

#include <time.h>
clock_t start_t, end_t;
double move_time = 0;
double rate_time = 0;
double material_time = 0;
double pos_vec_time = 0;

ChessEngine::ChessEngine() {

	srand(time(0));

	ifstream db_file;

	// Load king move database
	db_file.open("../data/king-moves.movelist");
	if (!db_file) {
		cout << "Fatal Error: Unable to read king moves file" << endl;
		return;
	} else {
		U64 a, b;
		for (short i=0; i<64; ++i) {	
			db_file >> a >> b;
			KMoveDB[i] = Bitboard(b);
		}
	}
	db_file.close();

	db_file.open("../data/knight-moves.movelist");
	if (!db_file) {
		cout << "Fatal Error: Unable to read knight moves file" << endl;
		return;
	} else {
		U64 a, b;
		for (short i=0; i<64; ++i) {
			db_file >> a >> b;
			NMoveDB[i] = Bitboard(b);
		}
	}
	db_file.close();
}

ChessEngine::~ChessEngine() {}

// ----- Generating Moves -----

vector<Move> ChessEngine::genPMoves(ChessState cs) {
	/* Generates all legal pawn moves */

	short i, j;
	vector<short> start;
	Bitboard move_board;
	Bitboard kill_board;
	vector<short> move_targets;
	vector<short> kill_targets;
	short killed;
	vector<Move> validMoves;
	
	// Get all pawn locations
	start = cs->pieces[cs->turn][cs->pawn]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		// Get potential squares
		move_board.board = 0;
		kill_board.board = 0;
		if (cs->turn == cs->white) {
			// Add square above
			move_board.setPos(start[i]+8, true);
			// Remove if blocked by enemy piece
			move_board.board &= ~(cs->pieces[cs->black][cs->all_pieces]->board);
			// Remove if blocked by friendly piece
			if (move_board.board != 0) {
				move_board.board &= ~(cs->pieces[cs->white][cs->all_pieces]->board);
			}

			// Try two squares up if can move up one and on home row
			if (move_board.board != 0 && start[i] >= 8 && start[i] <= 15) {
				move_board.setPos(start[i]+16, true);
				// Remove square if occupied
				move_board.board &= ~(cs->pieces[cs->white][cs->all_pieces]->board);
				move_board.board &= ~(cs->pieces[cs->black][cs->all_pieces]->board);
			}

			// Add left kill position if not on a column
			if (start[i] % 8 != 0) {
				kill_board.setPos(start[i]+7, true);
			}
			// Add right kill position if not on h column
			if (start[i] % 8 != 7) {
				kill_board.setPos(start[i]+9, true);
			}
			// Remove any kill positions without enemy pieces
			kill_board.board &= cs->pieces[cs->black][cs->all_pieces]->board;

		} else {	// Black
			// Add square above
			move_board.setPos(start[i]-8, true);
			// Remove if blocked by enemy piece
			move_board.board &= ~(cs->pieces[cs->white][cs->all_pieces]->board);
			// Remove if blocked by friendly piece
			if (move_board.board != 0) {
				move_board.board &= ~(cs->pieces[cs->black][cs->all_pieces]->board);
			}

			// Try two squares up if can move up one and on home row
			if (move_board.board != 0 && start[i] >= 48 && start[i] <= 55) {
				move_board.setPos(start[i]-16, true);
				// Remove square if occupied
				move_board.board &= ~(cs->pieces[cs->white][cs->all_pieces]->board);
				move_board.board &= ~(cs->pieces[cs->black][cs->all_pieces]->board);
			}

			// Add left kill position if not on a column
			if (start[i] % 8 != 0) {
				kill_board.setPos(start[i]-9, true);
			}
			// Add right kill position if not on h column
			if (start[i] % 8 != 7) {
				kill_board.setPos(start[i]-7, true);
			}
			// Remove any kill positions without pawns
			kill_board.board &= cs->pieces[cs->white][cs->all_pieces]->board;
		}

		// All squares that can be killed/moved to
		move_targets = move_board.getPosVector();
		kill_targets = kill_board.getPosVector();

		// TODO: Make sure king is safe after move

		// All possible non-kill moves
		for (j=0; j<move_targets.size(); ++j) {
			validMoves.push_back(Move(cs->pawn, start[i], move_targets[j]));
		}

		// All possible kill moves
		for (j=0; j<kill_targets.size(); ++j) {
			// Check for killing a piece
			killed = cs->getPieceType(!cs->turn, kill_targets[j]);
			validMoves.push_back(Move(cs->pawn, start[i], kill_targets[j], killed));
		}
	}

	return validMoves;
}


vector<Move> ChessEngine::genNMoves(ChessState* cs) {
	/* Generates all legal knight moves */

	short i, j;
	vector<Move> validMoves;
	vector<short> start;
	Bitboard target_board;
	vector<short> targets;
	short killed;
	
	// Get all knight locations
	start = cs->pieces[cs->turn][cs->knight]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		// Get potential squares
		target_board = NMoveDB.find(start[i])->second;
		// Remove squares with same coloured pieces
		target_board.board &= ~(cs->pieces[cs->turn][cs->all_pieces]->board);	
		// Positions of all targets
		targets = target_board.getPosVector();

		// TODO: Make sure king is safe after move

		// Add moves to vector
		for (j=0; j<targets.size(); ++j) {
			// Check for killing a piece
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(targets[j])) {
				killed = cs->getPieceType(!cs->turn, targets[j]);
			} else {
				killed = -1;	// Default
			}

			// Store as Move object
			validMoves.push_back(Move(cs->knight, start[i], targets[j], killed));
		}
	}

	return validMoves;
}


vector<Move> ChessEngine::genBMoves(ChessState* cs) {
	/* Generates all legal Bishop moves */

	U8 i, j;
	U8 pos;
	vector<U8> start;
	vector<U8> kill_targets;
	U8 killed;
	vector<Move> validMoves;
	
	// Get all bishop locations
	start = cs->pieces[cs->turn][cs->bishop]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		kill_targets.clear();

		// Up-right movement
		pos = start[i];
		while (pos < 56 && pos % 8 != 7) {	// Not top row or right col
			// If an enemy piece at target position
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+9)) {
				kill_targets.push_back(pos+9);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+9)) {
				validMoves.push_back(Move(cs->bishop, start[i], pos+9));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 9;
		}

		// Down-right moves
		pos = start[i];
		while (pos > 7 && pos % 8 != 7) {	// Not bottom row or right col
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-7)) {
				kill_targets.push_back(pos-7);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-7)) {
				validMoves.push_back(Move(cs->bishop, start[i], pos-7));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 7;
		}

		// Down-left moves
		pos = start[i];
		while (pos > 7 && pos % 8 != 0) {	// Not bottom row or left col
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-9)) {
				kill_targets.push_back(pos-9);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-9)) {
				validMoves.push_back(Move(cs->bishop, start[i], pos-9));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 9;
		}

		// Up-left moves
		pos = start[i];
		while (pos < 56 && pos % 8 != 0) {	// Not rightmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+7)) {
				kill_targets.push_back(pos+7);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+7)) {
				validMoves.push_back(Move(cs->bishop, start[i], pos+7));
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
			validMoves.push_back(Move(cs->bishop, start[i], kill_targets[j], killed));
		}
	}

	return validMoves;
}


vector<Move> ChessEngine::genRMoves(ChessState* cs) {
	/* Generates all legal rook moves */

	short i, j;
	short pos;
	vector<short> start;
	vector<short> kill_targets;
	short killed;
	vector<Move> validMoves;
	
	// Get all rook locations
	start = cs->pieces[cs->turn][cs->rook]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		kill_targets.clear();

		// Upward moves
		pos = start[i];
		while (pos < 56) {	// Not top row
			// If an enemy piece at target position
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+8)) {
				kill_targets.push_back(pos+8);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+8)) {
				validMoves.push_back(Move(cs->rook, start[i], pos+8));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 8;
		}

		// Downward moves
		pos = start[i];
		while (pos > 7) {	// Not bottom row
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-8)) {
				kill_targets.push_back(pos-8);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-8)) {
				validMoves.push_back(Move(cs->rook, start[i], pos-8));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 8;
		}

		// Leftward moves
		pos = start[i];
		while (pos % 8 != 0) {	// Not leftmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-1)) {
				kill_targets.push_back(pos-1);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-1)) {
				validMoves.push_back(Move(cs->rook, start[i], pos-1));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 1;
		}

		// Rightward moves
		pos = start[i];
		while (pos % 8 != 7) {	// Not rightmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+1)) {
				kill_targets.push_back(pos+1);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+1)) {
				validMoves.push_back(Move(cs->rook, start[i], pos+1));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 1;
		}

		// TODO: Make sure king is safe after move

		// All possible kill moves
		for (j=0; j<kill_targets.size(); ++j) {
			// Check for killing a piece
			killed = cs->getPieceType(!cs->turn, kill_targets[j]);
			validMoves.push_back(Move(cs->rook, start[i], kill_targets[j], killed));
		}
	}

	return validMoves;
}

vector<Move> ChessEngine::genQMoves(ChessState* cs){
	/* Generates all legal Queen moves */

	vector<Move> validMoves;
	
	// Get all queen locations
	start = cs->pieces[cs->turn][cs->queen]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		kill_targets.clear();

		// Upward moves
		pos = start[i];
		while (pos < 56) {	// Not top row
			// If an enemy piece at target position
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+8)) {
				kill_targets.push_back(pos+8);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+8)) {
				validMoves.push_back(Move(cs->queen, start[i], pos+8));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 8;
		}

		// Downward moves
		pos = start[i];
		while (pos > 7) {	// Not bottom row
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-8)) {
				kill_targets.push_back(pos-8);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-8)) {
				validMoves.push_back(Move(cs->queen, start[i], pos-8));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 8;
		}

		// Leftward moves
		pos = start[i];
		while (pos % 8 != 0) {	// Not leftmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-1)) {
				kill_targets.push_back(pos-1);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-1)) {
				validMoves.push_back(Move(cs->queen, start[i], pos-1));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 1;
		}

		// Rightward moves
		pos = start[i];
		while (pos % 8 != 7) {	// Not rightmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+1)) {
				kill_targets.push_back(pos+1);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+1)) {
				validMoves.push_back(Move(cs->queen, start[i], pos+1));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 1;
		}

		// Up-right movement
		pos = start[i];
		while (pos < 56 && pos % 8 != 7) {	// Not top row or right col
			// If an enemy piece at target position
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+9)) {
				kill_targets.push_back(pos+9);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+9)) {
				validMoves.push_back(Move(cs->queen, start[i], pos+9));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos += 9;
		}

		// Down-right moves
		pos = start[i];
		while (pos > 7 && pos % 8 != 7) {	// Not bottom row or right col
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-7)) {
				kill_targets.push_back(pos-7);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-7)) {
				validMoves.push_back(Move(cs->queen, start[i], pos-7));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 7;
		}

		// Down-left moves
		pos = start[i];
		while (pos > 7 && pos % 8 != 0) {	// Not bottom row or left col
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos-9)) {
				kill_targets.push_back(pos-9);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos-9)) {
				validMoves.push_back(Move(cs->queen, start[i], pos-9));
			// Else there is a friendly piece at target position
			} else {
				break;
			}
			pos -= 9;
		}

		// Up-left moves
		pos = start[i];
		while (pos < 56 && pos % 8 != 0) {	// Not rightmost column
			// If an enemy piece at target
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(pos+7)) {
				kill_targets.push_back(pos+7);
				break;
			// Else if there is no friendly piece at the target position
			} else if (!cs->pieces[cs->turn][cs->all_pieces]->getPos(pos+7)) {
				validMoves.push_back(Move(cs->queen, start[i], pos+7));
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
			validMoves.push_back(Move(cs->queen, start[i], kill_targets[j], killed));
		}
	}

	return validMoves;
}

vector<Move> ChessEngine::genKMoves(ChessState* cs){
	/* Generates all legal king moves */

	short i, j, k;
	vector<Move> validMoves;
	vector<short> start;
	Bitboard target_board;
	vector<short> targets;
	short killed;
	
	// Start position of the king
	start = cs->pieces[cs->turn][cs->king]->getPosVector(1);

	// For each king (there should only be one)
	for (i=0; i<start.size(); ++i) {

		// Get surrounding squares
		target_board = KMoveDB.find(start[i])->second;
		// Remove squares with same coloured pieces
		target_board.board = target_board.board & (~cs->pieces[cs->turn][cs->all_pieces]->board);	

		// TODO: CHECK IF TARGET SQUARE IS UNDER ATTACK

		// Positions of all target squares
		targets = target_board.getPosVector();

		// Add moves to vector
		for (j=0; j<targets.size(); ++j) {
			// Check for killing a piece
			if (cs->pieces[!cs->turn][cs->all_pieces]->getPos(targets[j])) {
				killed = cs->getPieceType(!cs->turn, targets[j]);
			} else {
				killed = -1;	// Default
			}

			// Add to list of valid moves
			validMoves.push_back(Move(cs->king, start[i], targets[j], killed));
		}
	}

	return validMoves;
}

// ----- Scoring Game State -----
// Standard material valuation
const float ChessEngine::materialValsSTD[2][6] = {
	{1, 3, 3, 5, 9, 200},
	{-1, -3, -3, -5, -9, -200},
};

float ChessEngine::scoreMaterialSTD(ChessState *cs) {
	/*	Provides a score based on the material on the board 
	 * 	using the standard system */


	start_t = clock();
	float total = 0;

	for (U8 i=0; i<6; ++i) {
		total += cs->pieces[0][i]->getPosVector().size() * materialValsSTD[0][i];
		total += cs->pieces[1][i]->getPosVector().size() * materialValsSTD[1][i];
	}

	end_t = clock();	// TEMP
	material_time += ((double) (end_t - start_t)) / CLOCKS_PER_SEC;	// TEMP

	return total;
}

// Larry Kaufman's material valuation
const float ChessEngine::materialValsLK[2][6] = {
	{1, 3.25, 3.25, 5, 9.75, 200},
	{-1, -3.25, -3.25, -5, -9.75, -200},
};

float ChessEngine::scoreMaterialLK(ChessState* cs) {
	/*	Provides a score based on the material on the board
	 * 	using the Larry Kaufman's system
	 *	https://web.archive.org/web/20160314214435/http://www.danheisman.com/Articles/evaluation_of_material_imbalance.htm */

	return 0;
}

// Hans Berliner's material valuation
const float ChessEngine::materialValsHB[2][6] = {
	{1, 3.2, 3.33, 5.1, 8.8, 200},
	{-1, -3.2, -3.33, -5.1, -8.8, -200},
};

float ChessEngine::scoreMaterialHB(ChessState* cs) {
	/*	Provides a score based on the material on the board
	 * 	using the Hans Berliner's system
	 *	https://en.wikipedia.org/wiki/Chess_piece_relative_value#Hans_Berliner's_system */
	return 0;
}

// ----- Primary Operations -----
float ChessEngine::rate(ChessState* cs) {
	/* Rates the status of game in terms of advantage */

	return this->scoreMaterialSTD(cs);
}

pair<Move, float> ChessEngine::bestMove(ChessState* asdf, U8 depth) {
	ChessState cs(asdf);

	short i;
	vector<Move> validMoves;
	vector<Move> m;	// Buffer vector
	vector<pair<Move, float>> ratedMoves;

	// --- Generate Valid Moves ---
	start_t = clock();	// TEMP

	m = genPMoves(&cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	m = genNMoves(&cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	m = genBMoves(&cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	m = genRMoves(&cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	m = genQMoves(&cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	m = genKMoves(&cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());
	
	end_t = clock();	// TEMP
	move_time += ((double) (end_t - start_t)) / CLOCKS_PER_SEC;	// TEMP

	// Check if valid moves were generated
	if (validMoves.size() == 0) {
		throw ChessState::NoMoves();
	}

	// --- Recursively Calculate Best Move ---
	if (depth <= 1) {
		// Create a vector of ratings paired with moves
		for (i=0; i<validMoves.size(); ++i) {
			cs.move(validMoves[i]);

			start_t = clock();	// TEMP
			ratedMoves.push_back(make_pair(validMoves[i], this->rate(&cs)));
			end_t = clock();	// TEMP
			rate_time += ((double) (end_t - start_t)) / CLOCKS_PER_SEC;	// TEMP

			cs.reverseMove(validMoves[i]);
		}

	} else {
		// Make each move then recursively calculate its rating
		// before reversing the move. 
		for (i=0; i<validMoves.size(); ++i) {
			cs.move(validMoves[i]);
			try {
				ratedMoves.push_back(
					make_pair(validMoves[i], this->bestMove(&cs, depth-1).second));
			} catch (ChessState::NoMoves& e) {
				ratedMoves.push_back(
					make_pair(validMoves[i], 0));
			}
			cs.reverseMove(validMoves[i]);
		}
	}

	// Return the best move for the current player
	sort(ratedMoves.begin(), ratedMoves.end(), this->sortRatedMove);

	// Return highest rating for white, lowest for black
	if (cs.turn) {
		return ratedMoves[0];
	} else {
		return ratedMoves[ratedMoves.size() - 1];
	}
}
