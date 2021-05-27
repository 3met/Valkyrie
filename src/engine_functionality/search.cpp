
#include <chrono>
#include <cmath>
#include <random>
#include <utility>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "S8.hpp"

// TEMP
#include <iostream>

using namespace std::chrono;

// Generating random numbers
random_device rd;	// Seed
mt19937 gen(rd());

/* Needs to be run in it's own thread
   in order to execute properly.
   Time is in milliseconds. "timeLeft" is time
   left on the clock. "timeInc" is the time 
   increment per move. */
Move ChessEngine::searchOnTimer(ChessState cs, int timeLeft, int timeInc) {
	// Start timer
	auto start = high_resolution_clock::now();
	// Set Stats
	this->nodesTotal = 0;
	this->canSearch = true;

	// Check if position in opening book
	if (this->openingTable.contains(&cs.bh)) {
		std::vector<Move> moves = this->openingTable.get(&cs.bh);
		
		uniform_int_distribution<> distr(0, moves.size()-1);
		return moves[distr(gen)];
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

		cout << "info score cp " << ratedMove.second << endl;

		// Check if time remains
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start).count();
		// Break if more than 95% of the target time has passed
		if (duration >= maxTime*0.95) {
			break;
		}

		// Break early if past 3 searches have the same result
		// and more than half the target search time has passed
		if (duration > maxTime*0.5 && moveList.size() >= 3
			&& moveList[i-1] == moveList[i-2] && moveList[i-2] == moveList[i-3]) {

			break;
		}

		if (this->canSearch == false) {
			break;
		}

		i += 1;
	}

	return moveList[i-1];
}

/* Seaches to a specific depth as specified */
Move ChessEngine::searchDepth(ChessState cs, U8 depth) {

	// Start timer
	auto start = high_resolution_clock::now();

	// Set Stats
	this->nodesTotal = 0;
	this->canSearch = true;

	pair<Move, EvalScore> ratedMove;
	Move m;
	short i = 1;
	// Loop to increase depth until time is up
	while (i <= depth) {

		currDepth = i;
		cout << "info depth " << i << endl;

		ratedMove = this->bestMove(&cs, i);
		m = ratedMove.first;

		cout << "info score cp " << ratedMove.second << endl;

		if (this->canSearch == false) {
			break;
		}

		i += 1;
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start).count();
	cout << "info time " << duration << endl;

	return m;
}

/* Searches for the best move until "canSearch"
   is false. Needs to be run in it's own thread
   in order to execute properly. */
Move ChessEngine::searchInfinite(ChessState cs) {
	// Start timer
	auto start = high_resolution_clock::now();

	this->nodesTotal = 0;
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

