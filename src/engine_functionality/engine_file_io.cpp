
#include <fstream>
#include <iostream>
#include <string>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "opening_table.hpp"
#include "size_defs.hpp"

string DATA_DIR = "../data/";

// Reads files containing a table of bitboard data
bool ChessEngine::readBitboardTable(Bitboard table[64], string fileName) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return false;
	} else {
		U64 val;
		for (U8 i(0); i<64; ++i) {	
			db_file >> val;
			table[i] = Bitboard(val);
		}
	}
	db_file.close();
	return true;
}

// Reads magic bitboard file data
bool ChessEngine::readMagicTable(U8 magicShifts[64], Bitboard magics[64], string fileName) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return false;
	} else {
		short shift;
		U64 magic;
		for (U8 i(0); i<64; ++i) {	
			db_file >> shift >> magic;
			magicShifts[i] = shift;
			magics[i] = Bitboard(magic);
		}
	}
	db_file.close();
	return true;
}

// Reads magic bitboard attack table data
bool ChessEngine::readAttackTable(Bitboard* attackTable[64], string directory) {
	ifstream db_file;

	for (U8 i(0); i<64; ++i) {
		db_file.open(DATA_DIR + directory + to_string(i) + ".tab");
		if (!db_file) {
			cout << "Fatal Error: Unable to read " << DATA_DIR << directory << to_string(i) << ".tab" << endl;
			return false;
		} else {
			U64 val;
			attackTable[i] = new Bitboard[4096];
			for (short j=0; j<4096; ++j) {
				db_file >> val;
				attackTable[i][j] = Bitboard(val);
			}
		}
		db_file.close();
	}
	return true;
}

// Reads bonus table data
bool ChessEngine::readBonusTable(S8 bonusTable[64], string fileName, const U8 READ_ORDER[64]) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return false;
	} else {
		short val;
		for (U8 i(0); i<64; ++i) {	
			db_file >> val;
			bonusTable[READ_ORDER[i]] = val;
		}
	}
	db_file.close();
	return true;
}

// Reads opening book data
bool ChessEngine::readOpeningBook(OpeningTable* table, string fileName) {
	ifstream db_file;
	db_file.open(DATA_DIR + fileName);

	if (!db_file) {
		cout << "Error: Unable to read " << DATA_DIR << fileName << endl;
		return false;
	} else {
		ChessState cs;
		vector<Move> moves;
		string line;
		U8 lineIndex;
		size_t found;

		while (getline(db_file, line)) {
			// Load FEN
			cs.loadFEN(line);

			moves.clear();
			lineIndex = 0;
			getline(db_file, line);	// Moves

			// Get moves paired with FEN
			while (true) {
				// Find begining of move
				found = line.find_first_of("abcdefgh", lineIndex);
				if (found != string::npos) {	// If found
					lineIndex = found;
				} else {
					break;
				}

				// Find end of move
				found = line.find(' ', lineIndex);

				moves.push_back(cs.notationToMove(line.substr(lineIndex, found-lineIndex)));
				lineIndex = found;
			}

			table->add(&cs.bh, &moves);
		}
	}
	
	db_file.close();
	return true;
}
