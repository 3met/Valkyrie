
#include <iostream>
#include <fstream>
#include "../include/bitboard.hpp"
#include "../include/chess_engine.hpp"
#include "../include/move.hpp"

using namespace std;

ChessEngine::ChessEngine() {

	// Load king move database
	ifstream db_file;
	db_file.open("../data/king-moves.movelist");
	if (!db_file) {
		cout << "Fatal Error: Unable to read king moves file" << endl;
	} else {
		unsigned long long int a, b;
		while (true) {			
			db_file >> a >> b;
			kMoveDB.insert(pair<unsigned long long int, Bitboard>(a, Bitboard(b)));
			if (db_file.eof()) {
				break;
			}
		}
	}
	db_file.close();
}

ChessEngine::~ChessEngine() {}

vector<Move> ChessEngine::genKMoves(ChessState c){
	/* Generates all legal king moves */

	vector<Move> m;
	vector<short> start;
	vector<short> targets;
	
	start = c.pieces[c.turn][c.king]->getPosVector(1);
	targets = kMoveDB.find(c.pieces[c.turn][c.king]->board)->second.getPosVector();

	for (short i=0; i<targets.size(); ++i) {
		m.push_back(Move(c.piece_names[c.turn][c.king], start[0], targets[i]));
	}

	return m;
}

Move ChessEngine::bestMove(ChessState c) {
	vector<Move> validMoves;
	vector<Move> m;

	m = genKMoves(c);

	validMoves.insert(validMoves.end(), m.begin(), m.end());

	return validMoves[1];
}

