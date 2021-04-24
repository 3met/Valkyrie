
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#include <map>
#include <vector>
#include "bitboard.hpp"
#include "eval_score.hpp"
#include "opening_table.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "U64.hpp"
#include "U8.hpp"
#include "S8.hpp"

using namespace std;

class ChessEngine {
private:
	// King and knight move databases
	map<U64, Bitboard> KMoveDB;
	map<U64, Bitboard> NMoveDB;

	// Opening book database
	OpeningTable openingTable;

	// Position bonuses
	map<U8, S8> knightBonus;

	// Material value systems
	const static short materialValsSTD[6];
	const static short materialValsLK[6];
	const static short materialValsHB[6];

	static bool sortRatedMove(const pair<Move, float> &a, const pair<Move, float> &b) {
		return a.second < b.second;
	};

	// File IO methods
	void read_move_table(map<U64, Bitboard>* moveTable, string fileName);
	void read_bonus_table(map<U8, S8>* bonusTable, string fileName);
	void read_opening_book(OpeningTable* openingTable, string fileName);

public:
	ChessEngine();
	~ChessEngine();

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
	short evalBoard(ChessState* cs);
	short evalSide(ChessState* cs, bool side, vector<U8> pieces[2][6]);
	
	// Move Selection
	pair<Move, EvalScore> bestMove(ChessState* cs, U8 depth);
	EvalScore negamaxSearch(ChessState* cs, U8 depth, EvalScore alpha, EvalScore beta);
};
#endif
