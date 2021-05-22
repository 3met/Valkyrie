
#pragma once
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#define USE_TRANS_TABLE

#include <map>
#include <utility>
#include <vector>
#include "bitboard.hpp"
#include "eval_score.hpp"
#include "opening_table.hpp"
#include "transposition_table.hpp"
#include "chess_state.hpp"
#include "move.hpp"
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

	// Opening book database
	OpeningTable openingTable;

	// Position bonuses
	map<U8, S8> knightBonus;
	map<U8, S8> bishopBonus;
	map<U8, S8> queenBonus;

	// Material value systems
	const static short materialValsSTD[6];
	const static short materialValsLK[6];
	const static short materialValsHB[6];

	static bool sortRatedMove(const pair<Move, float> &a, const pair<Move, float> &b) {
		return a.second < b.second;
	};

	// File IO methods
	void readMoveTable(Bitboard moveTable[64], string fileName);
	void readBonusTable(map<U8, S8>* bonusTable, string fileName);
	void readOpeningBook(OpeningTable* openingTable, string fileName);

public:
	ChessEngine();
	~ChessEngine();

	// Transposition table
	#ifdef USE_TRANS_TABLE
	bool useTransTable = true;
	TranspositonTable transTable;
	#endif

	// UCI accessable functions
	bool isLoaded = false;
	bool canSearch = false;
	int nodesTotal = 0;
	int nodesPerSecond = 0;
	short currDepth = 0;
	short currSelDepth = 0;
	int searchTime = 0;

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
	Move searchDepth(ChessState cs);
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
