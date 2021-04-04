
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#include <map>
#include <vector>
#include "bitboard.hpp"
#include "chess_state.hpp"
#include "move.hpp"
#include "state_tree.hpp"
#include "U64.hpp"
#include "U8.hpp"

using namespace std;

class ChessEngine {
private:
	map<U64, Bitboard> KMoveDB;
	map<U64, Bitboard> NMoveDB;
	
	// Material value systems
	const static float materialValsSTD[6];
	const static float materialValsLK[6];
	const static float materialValsHB[6];

	static bool sortRatedMove(const pair<Move, float> &a, const pair<Move, float> &b) {
		return a.second < b.second;
	};

public:
	ChessEngine();
	~ChessEngine();

	// Generating Moves
	void genPMoves(ChessState* cs, vector<Move>* moves);
	void genNMoves(ChessState* cs, vector<Move>* moves);
	void genBMoves(ChessState* cs, vector<Move>* moves);
	void genRMoves(ChessState* cs, vector<Move>* moves);
	void genQMoves(ChessState* cs, vector<Move>* moves);
	void genKMoves(ChessState* cs, vector<Move>* moves);
	void genAllMoves(ChessState* cs, vector<Move>* moves);

	float eval(ChessState* cs);
	float eval_side(ChessState* cs, bool side, vector<U8> pieces[2][6]);
	pair<Move, float> bestMove(ChessState* cs, U8 depth);
	float minimax_eval(ChessState* cs, U8 depth, float alpha, float beta);
	float minimax_eval_top(ChessState* cs, U8 depth, float alpha, float beta, Move* bestMove);

	StateTree stateTree;
};

#endif
