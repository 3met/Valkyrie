
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
vector<Move> ChessEngine::genNMoves(ChessState cs){
	/* Generates all legal knight moves */

	short i, j, k;
	vector<Move> validMoves;
	vector<short> start;
	Bitboard target_board;
	vector<short> targets;
	short killed;
	
	// Get all knight locations
	start = cs.pieces[cs.turn][cs.knight]->getPosVector();

	for (i=0; i<start.size(); ++i) {
		// Get potential squares
		target_board = NMoveDB.find(start[i])->second;
		// Remove squares with same coloured pieces
		target_board.board &= ~(cs.pieces[cs.turn][cs.all_pieces]->board);	
		// Positions of all targets
		targets = target_board.getPosVector();

		// TODO: Make sure king is safe after move

		// Add moves to vector
		for (j=0; j<targets.size(); ++j) {
			// Check for killing a piece
			if (cs.pieces[!cs.turn][cs.all_pieces]->getPos(targets[j])) {
				killed = cs.getPieceType(!cs.turn, targets[j]);
			} else {
				killed = -1;	// Default
			}

			// Store as Move object
			validMoves.push_back(Move(cs.knight, start[i], targets[j], killed));
		}
	}

	return validMoves;
}

vector<Move> ChessEngine::genQMoves(ChessState cs){

}

vector<Move> ChessEngine::genKMoves(ChessState cs){
	/* Generates all legal king moves */

	short i, j, k;
	vector<Move> validMoves;
	vector<short> start;
	Bitboard target_board;
	vector<short> targets;
	short killed;
	
	// Start position of the king
	start = cs.pieces[cs.turn][cs.king]->getPosVector(1);

	// For each king (there should only be one)
	for (i=0; i<start.size(); ++i) {

		// Get surrounding squares
		target_board = KMoveDB.find(start[i])->second;
		// Remove squares with same coloured pieces
		target_board.board = target_board.board & (~cs.pieces[cs.turn][cs.all_pieces]->board);	
		// TODO: CHECK IF TARGET SQUARE IS UNDER ATTACK

		// Positions of all target squares
		targets = target_board.getPosVector();

		// Add moves to vector
		for (j=0; j<targets.size(); ++j) {
			// Check for killing a piece
			if (cs.pieces[!cs.turn][cs.all_pieces]->getPos(targets[j])) {
				killed = cs.getPieceType(!cs.turn, targets[j]);
			} else {
				killed = -1;	// Default
			}

			// Add to list of valid moves
			validMoves.push_back(Move(cs.king, start[i], targets[j], killed));
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

float ChessEngine::scoreMaterialSTD(ChessState cs) {
	/*	Provides a score based on the material on the board 
	 * 	using the standard system */

	float total = 0;

	for (short i=0; i<6; ++i) {
		total += cs.pieces[0][i]->getPosVector().size() * materialValsSTD[0][i];
		total += cs.pieces[1][i]->getPosVector().size() * materialValsSTD[1][i];
	}

	return total;
}

// Larry Kaufman's material valuation
const float ChessEngine::materialValsLK[2][6] = {
	{1, 3.25, 3.25, 5, 9.75, 200},
	{-1, -3.25, -3.25, -5, -9.75, -200},
};

float ChessEngine::scoreMaterialLK(ChessState cs) {
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

float ChessEngine::scoreMaterialHB(ChessState cs) {
	/*	Provides a score based on the material on the board
	 * 	using the Hans Berliner's system
	 *	https://en.wikipedia.org/wiki/Chess_piece_relative_value#Hans_Berliner's_system */
	return 0;
}

// ----- Primary Operations -----
float ChessEngine::rate(ChessState cs) {
	/* Rates the status of game in terms of advantage */

	return this->scoreMaterialSTD(cs);
}

pair<Move, float> ChessEngine::bestMove(ChessState cs, short depth) {

	short i;
	vector<Move> validMoves;
	vector<Move> m;	// Buffer vector
	vector<pair<Move, float>> ratedMoves;

	// --- Generate Valid Moves ---
	m = genKMoves(cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	m = genNMoves(cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	// Check if valid moves were generated
	if (validMoves.size() == 0) {
		throw ChessState::NoMoves();
	}

	// --- Recursively Calculate Best Move ---
	if (depth <= 1) {
		// Create a vector of ratings paired with moves
		for (i=0; i<validMoves.size(); ++i) {
			cs.move(validMoves[i]);
			ratedMoves.push_back(make_pair(validMoves[i], this->rate(cs)));
			cs.reverseMove(validMoves[i]);
		}
	} else {
		// Make each move then recursively calculate its rating
		// before reversing the move. 
		for (i=0; i<validMoves.size(); ++i) {
			cs.move(validMoves[i]);
			try {
				ratedMoves.push_back(
					make_pair(validMoves[i], this->bestMove(cs, depth-1).second));
			} catch (ChessState::NoMoves& e) {
				ratedMoves.push_back(
					make_pair(validMoves[i], 0));
			}
			cs.reverseMove(validMoves[i]);
		}
	}

	// Return the best move for the current player
	sort(ratedMoves.begin(), ratedMoves.end(), this->sortRatedMove);

	if (cs.turn) {	// Highest rating for white, lowest for black
		stateTree.add(cs, ratedMoves[0].first);
		return ratedMoves[0];
	} else {
		stateTree.add(cs, ratedMoves[ratedMoves.size() - 1].first);
		return ratedMoves[ratedMoves.size() - 1];
	}
}
