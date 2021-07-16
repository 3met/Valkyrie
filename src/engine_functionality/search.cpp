
#include <chrono>
#include <cmath>
#include <random>
#include <utility>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "size_defs.hpp"

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
	this->currScore = 0;
	this->limitTime = true;
	this->passedMinTime = false;
	this->passedOptimalTime = false;
	this->canSearch = true;

	// Check if position in opening book
	if (useOwnBook && this->openingTable.contains(&cs.bh)) {
		std::vector<Move> moves(this->openingTable.get(&cs.bh));
		
		++nSearches;
		uniform_int_distribution<> distr(0, moves.size()-1);
		return moves[distr(gen)];
	}

	// Max time to choose move
	this->minEndTime = start + chrono::microseconds(timeLeft/40);
	this->optimalEndTime = start + chrono::microseconds(min((timeLeft/30) + timeInc, timeLeft/6));
	this->hardEndTime = start + chrono::microseconds(min((timeLeft/15) + timeInc, timeLeft));

	pair<Move, EvalScore> ratedMove;
	std::vector<Move> moveList;
	short i(1);
	// Loop to increase depth until time is up
	while (true) {
		// Set search iteration parameters
		this->currDepth = i;
		this->maxDepth = i * MAX_DEPTH_RATIO;

		ratedMove = this->bestMove(&cs, i);

		// Break if no longer has search permission
		if (!this->canSearch) {
			// Only update the score if a better moves was found
			if (ratedMove.second > this->currScore) {
				moveList.push_back(ratedMove.first);
				this->currScore = ratedMove.second;
			}

			break;
		}

		moveList.push_back(ratedMove.first);
		this->currScore = ratedMove.second;
		
		// Break on null move
		if (ratedMove.first.isNull()) {
			break;
		}

		i += 1;

		// Break early if past 3 searches have the same result
		// and we have already searched the minimum amount of time
		if (passedMinTime && moveList.size() >= 3
			&& moveList[i] == moveList[i-1] && moveList[i-1] == moveList[i-2]) {

			break;
		}

		// Break early if past 2 searches have the same result
		// and we have already searched the optimal amount of time
		if (passedOptimalTime && moveList.size() >= 2
			&& moveList[i] == moveList[i-1]) {
		
			break;
		}		
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

		// If search stopped due to lack of permissions
		if (!this->canSearch) {
			// Only update the score if a better moves was found
			if (ratedMove.second > this->currScore) {
				m = ratedMove.first;
				this->currScore = ratedMove.second;
			}

			break;
		}

		m = ratedMove.first;
		this->currScore = ratedMove.second;
		
		// Break on returned null move
		if (ratedMove.first.isNull()) {
			break;
		}


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
