
#pragma once
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#include <chrono>
#include <utility>
#include <vector>
#include "bitboard.hpp"
#include "eval_score.hpp"
#include "opening_table.hpp"
#include "transposition_table.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "move_compare.hpp"
#include "U64.hpp"
#include "U8.hpp"
#include "S8.hpp"

using namespace std;

class ChessEngine {
private:
	enum cardinalDirections {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
	};

	enum diagonalDirections {
		UP_RIGHT = 0,
		DOWN_RIGHT,
		DOWN_LEFT,
		UP_LEFT,
	};

	const static S8 CARDINAL_POS_INC[4];
	const static S8 DIAGONAL_POS_INC[4];

	// King and knight move databases
	Bitboard KMoveDB[64];
	Bitboard NMoveDB[64];
	// Fancy magic bitboard data
	Bitboard rookMasks[64];
	Bitboard rookMagics[64];
	U8 rookMagicShifts[64];
	Bitboard* rookAttackTable[64];
	Bitboard bishopMasks[64];
	Bitboard bishopMagics[64];
	U8 bishopMagicShifts[64];
	Bitboard* bishopAttackTable[64];

	// Position bonuses
	S8 knightBonus[64];
	S8 bishopBonus[64];
	S8 queenBonus[64];
	S8 kingOpeningBonus[2][64];	// Flipped for black and white
	S8 kingEarlyBonus[2][64];
	S8 kingMidBonus[2][64];
	S8 kingLateBonus[2][64];
	S8 kingEndBonus[2][64];

	// Opening book database
	OpeningTable openingTable;

	// Material value systems
	const static short materialValsSTD[6];
	const static short materialValsLK[6];
	const static short materialValsHB[6];

	static bool sortRatedMove(const pair<Move, float> &a, const pair<Move, float> &b) {
		return a.second < b.second;
	};

	// File IO methods
	bool readBitboardTable(Bitboard table[64], string fileName);
	bool readMagicTable(U8 magicShifts[64], Bitboard magics[64], string fileName);
	bool readAttackTable(Bitboard* attackTable[64], string directory);
	bool readBonusTable(S8 bonusTable[64], string fileName, const U8 READ_ORDER[64]=Bitboard::SHOW_ORDER);
	bool readOpeningBook(OpeningTable* openingTable, string fileName);

public:
	ChessEngine();
	~ChessEngine();

	// Transposition table
	TranspositonTable transTable;

	// Move comparison class
	MoveCompare moveCompare = MoveCompare(&transTable);

	// UCI accessible members
	// Status Variables
	bool isLoaded = false;
	bool canSearch = false;
	int searchTime = 0;
	int nSearches = 0;	// Number of searches preformed
	// Information Variables
	short currDepth = 0;
	short currSelDepth = 0;
	EvalScore currScore = EvalScore(0);
	chrono::high_resolution_clock::time_point startTime;
	U64 nodesTotal = 0;

	// Move Generation
	void genPMoves(ChessState* cs, vector<Move>* moves);
	void genNMoves(ChessState* cs, vector<Move>* moves);
	void genBMoves(ChessState* cs, vector<Move>* moves);
	void genRMoves(ChessState* cs, vector<Move>* moves);
	void genQMoves(ChessState* cs, vector<Move>* moves);
	void genKMoves(ChessState* cs, vector<Move>* moves);
	void genAllMoves(ChessState* cs, vector<Move>* moves);

	// Evaluation Methods
	U8 rateGameStage(ChessState* cs, vector<U8> pieces[2][6]);
	short evalBoard(ChessState* cs, bool perspective);
	short evalSide(ChessState* cs, bool side, vector<U8> pieces[2][6]);
	
	// Move Selection
	Move searchOnTimer(ChessState cs, int timeLeft, int timeInc);
	Move searchInfinite(ChessState cs);
	Move searchNodes(ChessState cs);
	Move searchDepth(ChessState cs, U8 depth);
	Move searchExactTime(ChessState cs);
	pair<Move, EvalScore> bestMove(ChessState* cs, U8 depth);
	EvalScore negamaxSearch(ChessState* cs, U8 depth, U8 depthTarget, EvalScore alpha, EvalScore beta);
	void sortMoves(vector<Move>* moves);

	// Miscellaneous Methods
	void load();
	U64 divide(ChessState* cs, U8 depth);
	U64 perft(ChessState* cs, U8 depth);
	pair<bool, S8> nextPieceCardinal(ChessState* cs, U8 pos, U8 direction);
	pair<bool, S8> nextPieceDiagonal(ChessState* cs, U8 pos, U8 direction);
	bool isPosAttacked(ChessState* cs, bool turn, U8 pos);
};
#endif
