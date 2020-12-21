
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include "../include/bitboard.hpp"
#include "../include/chess_engine.hpp"
#include "../include/chess_state.hpp"
#include "../include/move.hpp"

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
		unsigned long long int a, b;
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
		unsigned long long int a, b;
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

	vector<Move> validMoves;
	vector<short> start;
	Bitboard target_board;
	vector<short> targets;
	short killed;
	
	// Get all knight locations
	start = cs.pieces[cs.turn][cs.knight]->getPosVector();

	if (start.size() > 2) {
		cout << "TOO MANY KNIGHTS" << endl;
	}

	for (short i=0; i<start.size(); ++i) {
		// Get potential squares
		target_board = NMoveDB.find(start[i])->second;
		// Remove squares with same coloured pieces
		target_board.board &= (~cs.pieces[cs.turn][cs.all_pieces]->board);	
		// Positions of all targets
		targets = target_board.getPosVector();

		// Check for killing a piece
		killed = -1;	// Default
		if (cs.pieces[!cs.turn][cs.all_pieces]->getPos(targets[i])) {
			for (short k=0; k<targets.size(); ++k) {
				if (cs.pieces[!cs.turn][k]->getPos(targets[i])) {
					killed = k;
				}
			}
		}

		// Add moves to vector
		for (short j=0; j<targets.size(); ++j) {
			validMoves.push_back(Move(cs.knight, start[i], targets[j]));
		}
	}

	if (validMoves.size() > 16) {
		cout << "N" << validMoves.size() << "S" << endl; 
	}

	return validMoves;
}

vector<Move> ChessEngine::genQMoves(ChessState cs){

}

vector<Move> ChessEngine::genKMoves(ChessState cs){
	/* Generates all legal king moves */

	vector<Move> validMoves;
	vector<short> start;
	Bitboard target_board;
	vector<short> targets;
	short killed;
	
	// Start position of the king
	start = cs.pieces[cs.turn][cs.king]->getPosVector(1);

	if (start.size() > 1) {
		cout << "TOO MANY KINGS" << endl;
	}

	// For each king (there should only be one)
	for (short i=0; i<start.size(); ++i) {
		// Get surrounding squares
		target_board = KMoveDB.find(start[i])->second;
		// Remove squares with same coloured pieces
		target_board.board = target_board.board & (~cs.pieces[cs.turn][cs.all_pieces]->board);	
		// TODO: CHECK IF TARGET SQUARE IS UNDER ATTACK

		// Positions of all target squares
		targets = target_board.getPosVector();

		// Add moves to vector
		for (short j=0; j<targets.size(); ++j) {
			killed = -1;	// Default
			// Check for killing a piece
			if (cs.pieces[!cs.turn][cs.all_pieces]->getPos(targets[i])) {
				// Find what piece is killed
				for (short k=0; k<targets.size(); ++k) {
					if (cs.pieces[!cs.turn][k]->getPos(targets[i])) {
						killed = k;
						break;
					}
				}
			}

			// Add to list of valid moves
			validMoves.push_back(Move(cs.king, start[i], targets[j], killed));
		}
	}

	if (validMoves.size() > 8) {
		cout << "K" << validMoves.size() << "S" << endl; 
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

	// -- Generate all Moves --
	m = genKMoves(cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	m = genNMoves(cs);
	validMoves.insert(validMoves.end(), m.begin(), m.end());

	// Check if valid moves were generated
	if (validMoves.size() == 0) {
		cout << "No valid moves found" << endl;
		throw "No valid moves found.";
	}

	// Recursively calculate best move
	if (depth <= 1) {
		// Create a vector of ratings paired with moves
		for (i=0; i<validMoves.size(); ++i) {
			ratedMoves.push_back(make_pair(validMoves[i], this->rate(cs)));
		}
	} else {
		// Make each move then recursively calculate its rating
		// before reversing the move. 
		for (i=0; i<validMoves.size(); ++i) {
			cs.move(validMoves[i]);
			try {
				ratedMoves.push_back(
					make_pair(validMoves[i], this->bestMove(cs, depth-1).second));
			} catch (...) {}
			cs.reverseMove(validMoves[i]);
		}
	}

	if (ratedMoves.size() > 24) {
		cout << "r" << ratedMoves.size() << "l" << endl;
	}

	// Return the best move for the current player
	sort(ratedMoves.begin(), ratedMoves.end(), this->sortRatedMove);

	if (cs.turn) {	// Highest rating for white, lowest for black
		return ratedMoves[ratedMoves.size() - 1];
	} else {
		return ratedMoves[0];
	}

}
