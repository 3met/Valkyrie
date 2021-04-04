
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "U64.hpp"
#include "U8.hpp"
#include "S8.hpp"

string DATA_DIR = "../data/";

void ChessEngine::read_move_table(map<U64, Bitboard>* moveTable, string fileName) {
	ifstream db_file;

	// Load king move database
	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return;
	} else {
		U64 a, b;
		for (U8 i=0; i<64; ++i) {	
			db_file >> a >> b;
			(*moveTable)[i] = Bitboard(b);
		}
	}
	db_file.close();
}

void ChessEngine::read_bonus_table(map<U8, S8>* bonusTable, string fileName) {
	ifstream db_file;

	// Load king move database
	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return;
	} else {
		S8 val;
		for (U8 i=0; i<64; ++i) {	
			db_file >> val;
			(*bonusTable)[Bitboard::SHOW_ORDER[i]] = val;
		}
	}
	db_file.close();
}
