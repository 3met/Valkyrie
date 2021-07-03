
#pragma once
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#include <chrono>
#include <deque>
#include <utility>
#include <vector>
#include "bitboard.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "opening_table.hpp"
#include "pv_table.hpp"
#include "transposition_table.hpp"
#include "S8.hpp"
#include "U8.hpp"
#include "U64.hpp"

using namespace std;

class ChessEngine {
private:
	// King and knight move databases
	inline static Bitboard KMoveDB[64];
	inline static Bitboard NMoveDB[64];
	// Fancy magic bitboard data
	inline static Bitboard rookMasks[64];
	inline static Bitboard rookMagics[64];
	inline static U8 rookMagicShifts[64];
	inline static Bitboard* rookAttackTable[64];
	inline static Bitboard bishopMasks[64];
	inline static Bitboard bishopMagics[64];
	inline static U8 bishopMagicShifts[64];
	inline static Bitboard* bishopAttackTable[64];

	// Position bonuses
	inline static S8 knightBonus[64];
	inline static S8 bishopBonus[64];
	inline static S8 queenBonus[64];
	inline static S8 kingOpeningBonus[2][64];	// Flipped for black and white
	inline static S8 kingEarlyBonus[2][64];
	inline static S8 kingMidBonus[2][64];
	inline static S8 kingLateBonus[2][64];
	inline static S8 kingEndBonus[2][64];

	// Opening book database
	OpeningTable openingTable;

	// Material value systems
	const static short materialValsSTD[6];
	const static short materialValsLK[6];
	const static short materialValsHB[6];

	// File IO methods
	static bool readBitboardTable(Bitboard table[64], string fileName);
	static bool readMagicTable(U8 magicShifts[64], Bitboard magics[64], string fileName);
	static bool readAttackTable(Bitboard* attackTable[64], string directory);
	static bool readBonusTable(S8 bonusTable[64], string fileName, const U8 READ_ORDER[64]=Bitboard::SHOW_ORDER);
	static bool readOpeningBook(OpeningTable* openingTable, string fileName);

	// Move Generation
	static void genWhitePMoves(ChessState* cs, vector<Move>* moves);
	static void genBlackPMoves(ChessState* cs, vector<Move>* moves);
	static void genNMoves(ChessState* cs, vector<Move>* moves);
	static void genBMoves(ChessState* cs, vector<Move>* moves);
	static void genRMoves(ChessState* cs, vector<Move>* moves);
	static void genQMoves(ChessState* cs, vector<Move>* moves);
	static void genKMoves(ChessState* cs, vector<Move>* moves);
	static void genAllMoves(ChessState* cs, vector<Move>* moves);

	// Search Helper Methods
	void sortMoves(vector<Move>* moves, U8 depth);
	pair<Move, EvalScore> bestMove(ChessState* cs, U8 depth);
	EvalScore negamaxSearch(ChessState* cs, U8 depth, U8 depthTarget, EvalScore alpha, EvalScore beta);

	// Get + Set Methods
	void addKillerMove(Move* m, U8* depth);

public:
	ChessEngine();
	~ChessEngine();

	static bool isLoaded;
	static const short MAX_SEARCH_DEPTH = 256;
	
	// Configurable option
	U8 KILL_QUEUE_MAX_SIZE = 2;			// For killer heurisitc
	float MAX_DEPTH_RATIO = 1.5;		// For search extensions

	// Search Variables
	U8 maxDepth;
	TranspositonTable transTable;					// Transposition table
	deque<Move> killerHeuristic[MAX_SEARCH_DEPTH];	// Killer Heuristic
	PvTable pvTable = PvTable(MAX_SEARCH_DEPTH);	// Principal Variation Table

	// UCI accessible members
	// Status Variables
	bool canSearch = false;
	short nSearches = 0;	// Number of searches preformed
	// Information Variables
	short currDepth = 0;
	short currSelDepth = 0;
	EvalScore currScore = EvalScore(0);
	chrono::high_resolution_clock::time_point startTime;
	U64 nodesTotal = 0;

	// Evaluation Methods
	static U8 rateGameStage(ChessState* cs, vector<U8> pieces[2][6]);
	short evalBoard(ChessState* cs, bool perspective);
	short evalSide(ChessState* cs, bool side, vector<U8> pieces[2][6]);
	
	// Search Methods
	Move searchOnTimer(ChessState cs, U64 timeLeft, U64 timeInc);
	Move searchInfinite(ChessState cs);
	Move searchNodes(ChessState cs);
	Move searchDepth(ChessState cs, U8 depth);
	Move searchExactTime(ChessState cs);

	// Miscellaneous Methods
	static void load();
	static bool isPosAttacked(ChessState* cs, bool turn, U8 pos);
	void clear();

	// Debugging Methods
	U64 divide(ChessState* cs, U8 depth);
	U64 perft(ChessState* cs, U8 depth);
};
#endif
