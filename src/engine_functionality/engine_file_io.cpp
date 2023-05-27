
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "opening_table.hpp"
#include "size_defs.hpp"

#ifdef WINDOWS
	#include <windows.h>
#elif defined LINUX
	#include <linux/limits.h>
	#include <unistd.h>
#endif

string ChessEngine::DATA_DIR = "";

bool ChessEngine::loadDataDir() {
	ChessEngine::DATA_DIR = "";

	#ifdef WINDOWS
		char exePath[MAX_PATH] = "\0";
		HMODULE hModule = GetModuleHandle(NULL);
		if (hModule != NULL) {
			GetModuleFileName(hModule, exePath, (sizeof(exePath)));
			// Copy exePath to data dir
			for (short i=0; exePath[i]!='\0'; ++i) {
				DATA_DIR += exePath[i];
			}
			// Find bin directory from full path
			DATA_DIR = DATA_DIR.substr(0, DATA_DIR.find_last_of("/\\"));
			// Find data directory from bin directory
			DATA_DIR = DATA_DIR.substr(0, DATA_DIR.find_last_of("/\\"));
			DATA_DIR += "\\data\\";
			return true;
		} else {
			printf("Fatal Error: Unable to load data directory");
			return false;
		}
	#elif defined LINUX
		char exePath[PATH_MAX] = "\0";
		ssize_t count = readlink("/proc/self/exe", exePath, PATH_MAX);
		if (count != -1) {
			// Copy exePath to data dir
			for (short i=0; exePath[i]!='\0'; ++i) {
				DATA_DIR += exePath[i];
			}
			// Find bin directory from full path
			DATA_DIR = DATA_DIR.substr(0, DATA_DIR.find_last_of("/\\"));
			// Find data directory from bin directory
			DATA_DIR = DATA_DIR.substr(0, DATA_DIR.find_last_of("/\\"));
			DATA_DIR += "/data/";
			return true;
		} else {
			printf("Fatal Error: Unable to load data directory");
			return false;
		}
	#endif
}


// Reads files containing a table of bitboard data
bool ChessEngine::readBitboardTable(Bitboard table[64], string fileName) {
	ifstream db_file;

	db_file.open(DATA_DIR + fileName);
	if (!db_file) {
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << '\n';
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
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << '\n';
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
			cout << "Fatal Error: Unable to read " << DATA_DIR << directory << to_string(i) << ".tab" << '\n';
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
		cout << "Fatal Error: Unable to read " << DATA_DIR << fileName << '\n';
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
		cout << "Error: Unable to read " << DATA_DIR << fileName << '\n';
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
