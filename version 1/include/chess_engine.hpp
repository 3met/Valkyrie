
#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP

#include <map>
#include <vector>
#include "../include/chess_state.hpp"
#include "../include/move.hpp"

using namespace std;

class ChessEngine {
private:
	map<unsigned long long int, unsigned long long int> kMoveDB;
	
	vector<Move> genKMoves(ChessState c);

public:
	ChessEngine();
	~ChessEngine();

	Move bestMove(ChessState c);
};

#endif
