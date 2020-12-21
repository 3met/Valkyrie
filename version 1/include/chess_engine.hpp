
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#include <map>
#include <vector>
#include "../include/bitboard.hpp"
#include "../include/chess_state.hpp"
#include "../include/move.hpp"

using namespace std;

class ChessEngine {
private:
	map<unsigned long long int, Bitboard> KMoveDB;
	map<unsigned long long int, Bitboard> NMoveDB;
	
	const static float materialValsSTD[2][6];
	const static float materialValsLK[2][6];
	const static float materialValsHB[2][6];

	// Generating Moves
	vector<Move> genPMoves(ChessState cs);
	vector<Move> genNMoves(ChessState cs);
	vector<Move> genBMoves(ChessState cs);
	vector<Move> genRMoves(ChessState cs);
	vector<Move> genQMoves(ChessState cs);
	vector<Move> genKMoves(ChessState cs);

	// Scoring Game State
	float scoreMaterialSTD(ChessState cs);
	float scoreMaterialLK(ChessState cs);
	float scoreMaterialHB(ChessState cs);

	static bool sortRatedMove(const pair<Move, float> &a, const pair<Move, float> &b) {
		return a.second < b.second;
	};

public:
	ChessEngine();
	~ChessEngine();

	float rate(ChessState cs);
	pair<Move, float> bestMove(ChessState cs, short depth);
};

#endif
