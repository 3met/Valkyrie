
#include <chrono>
#include <cmath>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "move.hpp"

// TEMP
#include <iostream>
#include <utility>
#include "eval_score.hpp"

using namespace std::chrono;

/* Needs to be run in it's own thread
   in order to execute properly.
   Time is in milliseconds. "timeLeft" is time
   left on the clock. "timeInc" is the time 
   increment per move. */
Move ChessEngine::searchOnTimer(ChessState cs, int timeLeft, int timeInc) {
	// Start timer
	auto start = high_resolution_clock::now();
	
	this->canSearch = true;

	// Check if position in opening book
	if (this->openingTable.contains(&cs)) {
		std::vector<Move> moves = this->openingTable.get(&cs);;
		return moves[rand() % moves.size()];
	}

	// Max time to choose move
	int maxTime = min((timeLeft/20) + timeInc, timeLeft-100);

	pair<Move, EvalScore> ratedMove;
	std::vector<Move> moveList;
	short i = 1;
	// Loop to increase depth until time is up
	while (true) {

		currDepth = i;
		cout << "info depth " << i << endl;

		ratedMove = this->bestMove(&cs, i);
		moveList.push_back(ratedMove.first);

		cout << "info score " << ratedMove.second << endl;

		// Check if time remains
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		if (duration.count() >= maxTime) {
			break;
		}

		// Break early if past 3 searches have the same result
		if (moveList.size() >= 3 && moveList[i-1] == moveList[i-2]
			&& moveList[i-2] == moveList[i-3]) {

			break;
		}

		if (this->canSearch == false) {
			break;
		}

		i += 1;
	}

	return moveList[i-1];
}

/* Searches for the best move until "canSearch"
   is false. Needs to be run in it's own thread
   in order to execute properly. */
Move ChessEngine::searchInfinite(ChessState cs) {
	// Start timer
	auto start = high_resolution_clock::now();
	
	this->canSearch = true;

	pair<Move, EvalScore> ratedMove;
	Move bestMove;
	short i = 2;
	// Loop to increase depth until time is up
	while (true) {

		currDepth = i;
		cout << "info depth " << i << endl;

		bestMove = this->bestMove(&cs, i).first;


		if (this->canSearch == false) {
			break;
		}

		i += 1;
	}

	return bestMove;
}

