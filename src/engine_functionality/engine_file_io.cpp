
#include <fstream>
#include <iostream>
#include <string>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "opening_table.hpp"
#include "U64.hpp"
#include "U8.hpp"
#include "S8.hpp"

string DATA_DIR = "../data/";

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

bool ChessEngine::readOpeningBook(OpeningTable* table, string fileName) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Error: Unable to read " << DATA_DIR << fileName << endl;
		return false;
	} else {
		S8 i;
		ChessState cs;
		vector<Move> moves;
		string line;
		U8 lineIndex;
		size_t found;
		// Min percent of total moves used to removes outliers
		float minPopularity(0.05);
		int total;
		vector<int> moveCounts;

		while (getline(db_file, line)) {
			// Load FEN
			cs.loadFEN(line);

			moves.clear();
			moveCounts.clear();
			total = 0;
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
				found = line.find("{", lineIndex);
				moves.push_back(cs.notationToMove(line.substr(lineIndex, found-lineIndex)));
				
				// Beginning of move count
				lineIndex = found + 1;
				found = line.find("}", lineIndex);
				moveCounts.push_back(stoi(line.substr(lineIndex, found-lineIndex)));
				total += moveCounts[moveCounts.size()-1];

				lineIndex = found;
			}

			// Remove move outliers
			for (i=moves.size()-1; i>=0; --i) {
				if (moveCounts[i] < total * minPopularity) {
					moves.erase(moves.begin() + i);
				}
			}
			// Add moves to opening table
			if (moves.size() != 0) {
				table->add(&cs.bh, &moves);
			}
		}
	}
	db_file.close();
	return true;
}
