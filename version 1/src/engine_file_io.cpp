
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

void ChessEngine::read_opening_book(OpeningTable* openingTable, string fileName) {
	ifstream db_file;

	// Load king move database
	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << endl;
		return;
	} else {
		S8 i;
		ChessState cs;
		vector<Move> moves;
		string line;
		U8 lineIndex;
		size_t found;
		// Min percent of total moves removes outliers
		float minPopularity = 0.05;
		int total;
		vector<int> moveCounts;

		while (getline(db_file, line)) {
			moves.clear();
			lineIndex = 0;
			moveCounts.clear();
			total = 0;

			cs.loadFEN(line);
			getline(db_file, line);	// Moves

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

			if (moves.size() != 0) {
				// Remove move outliers
				for (i=moves.size()-1; i>=0; --i) {
					if (moveCounts[i] < total * minPopularity) {
						moves.erase(moves.begin() + i);
					}
				}

				openingTable->add(&cs, &moves);
			}
		}
	}
	db_file.close();
}
