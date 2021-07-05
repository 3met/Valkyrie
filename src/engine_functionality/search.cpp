
#include <chrono>
#include <cmath>
#include <random>
#include <utility>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "U64.hpp"

using namespace std::chrono;

// Generating random numbers
random_device rd;	// Seed
mt19937 gen(rd());

// Needs to be run in it's own thread in order to execute properly.
// Time is in microseconds.
// "timeLeft" is time left on the clock.
// "timeInc" is the time increment per move.
Move ChessEngine::searchOnTimer(ChessState cs, U64 timeLeft, U64 timeInc) {
	
	// Start timer
	high_resolution_clock::time_point start(high_resolution_clock::now());

	// Set Stats
	this->startTime = start;
	this->currDepth = 0;
	this->nodesTotal = 0;
	this->canSearch = true;
	this->currScore = 0;
	this->limitTime = true;

	// Check if position in opening book
	if (this->openingTable.contains(&cs.bh)) {
		std::vector<Move> moves(this->openingTable.get(&cs.bh));
		
		++nSearches;
		uniform_int_distribution<> distr(0, moves.size()-1);
		return moves[distr(gen)];
	}

	// Max time to choose move
	U64 optimalTime(min((timeLeft/20) + timeInc, timeLeft-200));
	U64 maxTime(optimalTime + 10);
	this->optimalEndTime = start + chrono::microseconds(optimalTime);
	this->hardEndTime = start + chrono::microseconds(maxTime);

	pair<Move, EvalScore> ratedMove;
	std::vector<Move> moveList;
	short i(1);
	// Loop to increase depth until time is up
	while (true) {

		this->currDepth = i;
		this->maxDepth = i * MAX_DEPTH_RATIO;

		ratedMove = this->bestMove(&cs, i);
		if (ratedMove.first.isNull()) {	// Look for null move
			break;
		}

		moveList.push_back(ratedMove.first);
		this->currScore = ratedMove.second;
		
		// Check if time remains
		high_resolution_clock::time_point stop(high_resolution_clock::now());
		U64 duration(duration_cast<microseconds>(stop - start).count());
		// Break if more than 90% of the target time has passed
		if (duration >= maxTime*0.90) {
			break;
		}

		// Break early if past 3 searches have the same result
		// and more than half the target search time has passed
		if (duration > maxTime*0.5 && moveList.size() >= 3
			&& moveList[i-1] == moveList[i-2] && moveList[i-2] == moveList[i-3]) {

			break;
		}

		i += 1;
	}

	++nSearches;
	return *(moveList.end()-1);
}

// Seaches to a specific depth as specified
Move ChessEngine::searchDepth(ChessState cs, U8 depth) {

	// Set Stats
	this->startTime = high_resolution_clock::now();
	this->currDepth = 0;
	this->nodesTotal = 0;
	this->canSearch = true;
	this->limitTime = false;

	pair<Move, EvalScore> ratedMove;
	Move m;
	short i(1);
	// Loop to increase depth until time is up
	while (i <= depth) {

		this->currDepth = i;
		this->maxDepth = i;

		ratedMove = this->bestMove(&cs, i);
		if (ratedMove.first.isNull()) {	// Look for null move
			break;
		}

		m = ratedMove.first;
		this->currScore = ratedMove.second;

		i += 1;
	}

	++nSearches;
	return m;
}

// Searches for the best move until "canSearch" is false.
// Needs to be run in it's own thread in order to execute properly.
Move ChessEngine::searchInfinite(ChessState cs) {

	// Set Stats
	this->startTime = high_resolution_clock::now();
	this->currDepth = 0;
	this->nodesTotal = 0;
	this->canSearch = true;
	this->limitTime = false;

	pair<Move, EvalScore> ratedMove;
	Move m;
	short i(2);
	// Loop to increase depth until time is up
	while (true) {

		this->currDepth = i;
		this->maxDepth = i * MAX_DEPTH_RATIO;

		ratedMove = this->bestMove(&cs, i);
		if (ratedMove.first.isNull()) {	// Look for null move
			break;
		}

		m = ratedMove.first;
		this->currScore = ratedMove.second;

		i += 1;
	}

	++nSearches;
	return m;
}
