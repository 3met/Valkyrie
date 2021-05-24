
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "opening_table.hpp"
#include "U64.hpp"
#include "U8.hpp"
#include "S8.hpp"

string DATA_DIR = "../data/";

void ChessEngine::readMoveTable(Bitboard moveTable[64], string fileName) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return;
	} else {
		U64 a, b;
		for (U8 i=0; i<64; ++i) {	
			db_file >> a >> b;
			moveTable[i] = Bitboard(b);
		}
	}
	db_file.close();
}

void ChessEngine::readBonusTable(map<U8, S8>* bonusTable, string fileName) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return;
	} else {
		short val;
		for (U8 i=0; i<64; ++i) {	
			db_file >> val;
			(*bonusTable)[Bitboard::SHOW_ORDER[i]] = val;
		}
	}
	db_file.close();
}

void ChessEngine::readOpeningBook(OpeningTable* openingTable, string fileName) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Error: Unable to read " << DATA_DIR << fileName << endl;
		return;
	} else {
		S8 i;
		ChessState cs;
		vector<Move> moves;
		string line;
		U8 lineIndex;
		size_t found;
		// Min percent of total moves used to removes outliers
		float minPopularity = 0.05;
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
				openingTable->add(&cs.bh, &moves);
			}
		}
	}
	db_file.close();
}
