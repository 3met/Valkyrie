
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "U64.hpp"

string DATA_DIR = "../data/";

void ChessEngine::read_move_list(map<U64, Bitboard>* moveList, string fileName) {
	ifstream db_file;

	// Load king move database
	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << fileName << endl;
		return;
	} else {
		U64 a, b;
		for (U8 i=0; i<64; ++i) {	
			db_file >> a >> b;
			(*moveList)[i] = Bitboard(b);
		}
	}
	db_file.close();
}



