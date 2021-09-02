
#pragma once
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#include <chrono>
#include <utility>
#include "bitboard.hpp"
#include "board_defs.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "opening_table.hpp"
#include "pv_table.hpp"
#include "size_defs.hpp"
#include "transposition_table.hpp"

using namespace std;

// ----- Game Stages -----
// A measure of how far the game has progressed
// 0   - 31  ==> opening
// 32  - 63  ==> late opening
// 64  - 95  ==> early game
// 96  - 127 ==> late early game
// 128 - 159 ==> mid game
// 160 - 191 ==> late mid game
// 192 - 223 ==> end game
// 224 - 255 ==> late end game
const U8 OPENING = 0;
const U8 LATE_OPENING = 1;
const U8 EARLY_GAME = 2;
const U8 LATE_EARLY_GAME = 3;
const U8 MID_GAME = 4;
const U8 LATE_MID_GAME = 5;
const U8 END_GAME = 6;
const U8 LATE_END_GAME = 7;

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
	inline static S8 pawnBonus[8][2][64]; // (game stage, colour, position)
	inline static S8 kingBonus[8][2][64]; // (game stage, colour, position)

	// Maximum depth the engine can search
	static const short MAX_SEARCH_DEPTH = 100;

	// Move Generation and Evaluation Variables
	// In some methods, the only first index of these arrays are used
	// to save the memory from declaring a new variable.
	U8 pawnPosArr[2][8];	// Piece positions by color
	U8 knightPosArr[2][10];
	U8 bishopPosArr[2][10];
	U8 rookPosArr[2][10];
	U8 queenPosArr[2][10];
	U8 kingPos[2];
	U8 pieceCount[2][5];	// The number of each piece type
	Bitboard bufferBoard;	// Bitboard to use as a buffer
	Bitboard moveBoard;		// Bitboard with all move target locations
	Bitboard killBoard;		// Bitboard with all kill target locations
	U8 posTargets[27];		// Target location positions
	U8 targetCount;			// Number of target positions
	Move moveArr[MAX_SEARCH_DEPTH][218];
	bool inNullMoveSearch;

	// Time management search variables
	bool limitTime;		// Whether to limit time in search
	bool passedMinTime;
	bool passedOptimalTime;
	bool passedSoftEndTime;
	chrono::high_resolution_clock::time_point minEndTime;
	chrono::high_resolution_clock::time_point optimalEndTime;
	chrono::high_resolution_clock::time_point softEndTime;
	chrono::high_resolution_clock::time_point hardEndTime;

	// Opening book database
	inline static OpeningTable openingTable;

	// Material value systems
	const static short materialVals[6];

	// File IO methods
	static string DATA_DIR;
	static bool loadDataDir();
	static bool readBitboardTable(Bitboard table[64], string fileName);
	static bool readMagicTable(U8 magicShifts[64], Bitboard magics[64], string fileName);
	static bool readAttackTable(Bitboard* attackTable[64], string directory);
	static bool readBonusTable(S8 bonusTable[64], string fileName, const U8 READ_ORDER[64]=SHOW_ORDER);
	static bool readOpeningBook(OpeningTable* openingTable, string fileName);

	// Move Generation
	// Note: 218 is the max number of moves in a position
	void genWhitePMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genBlackPMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genNMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genBMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genRMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genQMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genKMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genAllMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genWhitePKillMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genBlackPKillMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genNKillMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genBKillMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genRKillMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genQKillMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genKKillMoves(ChessState* cs, Move moves[218], U8* moveCount);
	void genAllKillMoves(ChessState* cs, Move moves[218], U8* moveCount);

	// Search Helper Methods
	void updateTimingVars();
	EvalScore quiescence(ChessState* cs, U8 depth, EvalScore alpha, EvalScore beta);
	EvalScore zwSearch(ChessState* cs, U8 depth, U8 ply, EvalScore beta);
	pair<Move, EvalScore> bestMove(ChessState* cs, U8 depth);
	EvalScore negamaxSearch(ChessState* cs, U8 depth, U8 ply, EvalScore alpha, EvalScore beta);

	// Move Ordering
	static const U8 centerBias[64];
	void swap(Move moves[218], U8 a, U8 b);
	bool sortPV(Move moves[218], U8 left, U8 right, U8 depth);
	bool sortHashMove(Move moves[218], U8 left, U8 right, Move hashMove);
	U8 sortKillerMoves(Move moves[218], U8 left, U8 right, U8 depth);
	U8 separateCaptures(Move moves[218], U8 left, U8 right);
	U8 separatePromotions(Move moves[218], U8 left, U8 right);
	U8 separateNonQuiet(Move moves[218], U8 left, U8 right);
	void sortMVVLVA(Move moves[218], U8 left, U8 right, ChessState* cs);
	void sortCenter(Move moves[218], U8 left, U8 right);
	void sortQuiet(Move moves[218], U8 left, U8 right, U8 depth);
	void sortNonQuiet(Move moves[218], U8 left, U8 right, ChessState* cs);
	void sortMain(Move moves[218], U8 left, U8 right, ChessState* cs, U8 depth, Move hashMove);
	void sortMainNoHash(Move moves[218], U8 left, U8 right, ChessState* cs, U8 depth);

	// Get + Set Methods
	void addKillerMove(Move* m, U8* depth);

public:
	ChessEngine();
	~ChessEngine();

	static bool isLoaded;
	
	// Configurable option
	bool useOwnBook = true;

	// Search Variables
	TranspostionTable* transTable;					// Transposition table
	bool killerHeuristicNext[MAX_SEARCH_DEPTH] = {false};
	Move killerHeuristic[MAX_SEARCH_DEPTH][2];		// Killer Heuristic
	PvTable pvTable = PvTable(MAX_SEARCH_DEPTH);	// Principal Variation Table

	// Evaluation variables
	U8 gameStage;			// The number of each piece type
	short pawnEvalResult[2];	// Evaluation results
	short knightEvalResult[2];
	short bishopEvalResult[2];
	short rookEvalResult[2];
	short queenEvalResult[2];
	short kingEvalResult[2];
	short kingSafetyEvalResult[2];

	// UCI accessible members
	// Status Variables
	bool canSearch = false;
	short nSearches = 0;	// Number of searches preformed
	// Information Variables
	short uciDepth = 0;
	short currSelDepth = 0;
	EvalScore currScore = EvalScore::DEFAULT;
	chrono::high_resolution_clock::time_point startTime;
	U64 nodesTotal = 0;

	// Evaluation Methods
	void setGameStage();
	void prepEval(ChessState* cs);
	short evalSide(bool side);
	void evalPawns(bool side);
	void evalKnights(bool side);
	void evalBishops(bool side);
	void evalRooks(bool side);
	void evalQueens(bool side);
	void evalKings(bool side);
	void evalKingSafety(bool side);
	short evalBoard(ChessState* cs, bool side);
	
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
