
#include <iostream>
#include <fstream>
#include "../include/chess_engine.hpp"
#include "../include/move.hpp"

using namespace std;

ChessEngine::ChessEngine() {
	ifstream db_file;
	db_file.open("../data/king-moves.movelist");
	if (!db_file) {
		cout << "Fatal Error: Unable to read king moves file" << endl;
	} else {
		unsigned long long int a, b;
		while (true) {			
			db_file >> a >> b;
			kMoveDB.insert(pair<unsigned long long int, unsigned long long int>(a, b));
			if (db_file.eof()) {
				break;
			}
		}
	}
	cout << kMoveDB.size() << endl;
}

ChessEngine::~ChessEngine() {}


vector<Move> ChessEngine::genKMoves(ChessState c){
	vector<Move> m;
	kMoveDB
}

Move ChessEngine::bestMove(ChessState c) {
	v1.insert(v1.end(),v2.begin(),v2.end());
}

