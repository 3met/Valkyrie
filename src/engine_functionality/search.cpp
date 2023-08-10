
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
Move ChessEngine::searchOnClock(ChessState cs, U64 timeLeft, U64 timeInc) {

	// Set Stats
	this->startTime = high_resolution_clock::now();		// Start timer
	this->uciDepth = 0;
	this->nodesTotal = 0;
	this->currScore = 0;
	this->limitTime = true;
	this->passedMinTime = false;
	this->passedOptimalTime = false;
	this->passedSoftEndTime = false;
	this->canSearch = true;

	// Check if position in opening book
	if (useOwnBook && this->openingTable.contains(&cs.bh)) {
		std::vector<Move> moves(this->openingTable.get(&cs.bh));
		
		++nSearches;
		uniform_int_distribution<> distr(0, moves.size()-1);
		return moves[distr(gen)];
	}

	// Max time to choose move
	this->minEndTime = this->startTime + chrono::microseconds(timeLeft/40);
	this->optimalEndTime = this->startTime + chrono::microseconds(min((timeLeft/30) + timeInc, timeLeft/6));
	this->softEndTime = this->startTime + chrono::microseconds(min((timeLeft/20) + timeInc, timeLeft));
	this->hardEndTime = this->startTime + chrono::microseconds(min((timeLeft/15) + timeInc, timeLeft));

	pair<Move, EvalScore> ratedMove;
	std::vector<Move> moveList;

	// Loop to increase depth until time is up
	for (short i(1); i<this->MAX_SEARCH_DEPTH; ++i) {
		// Set search iteration parameters
		this->uciDepth = i;

		ratedMove = this->bestMove(&cs, i);

		// Break if no longer has search permission
		if (!this->canSearch) {
			// Use results from incomplete depth
			if (!ratedMove.first.isNull()) {
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

		updateTimingVars();

		// Break early if past 3 searches have the same result
		// and we have already searched the minimum amount of time
		if (passedMinTime && moveList.size() >= 3
			&& moveList[i-1] == moveList[i-2] && moveList[i-2] == moveList[i-3]) {

			break;
		}

		// Break early if past 2 searches have the same result
		// and we have already searched the optimal amount of time
		if (passedOptimalTime && moveList.size() >= 2
			&& moveList[i-1] == moveList[i-2]) {
		
			break;
		}

		if (passedSoftEndTime) {
			break;
		}	
	}

	++nSearches;
	return *(moveList.end()-1);
}


// Needs to be run in it's own thread in order to execute properly.
// Time is in microseconds.
// "movetime" is the target time to search for
Move ChessEngine::searchSetTime(ChessState cs, U64 movetime) {

	// Set Stats
	this->startTime = high_resolution_clock::now();		// Start timer
	this->uciDepth = 0;
	this->nodesTotal = 0;
	this->currScore = 0;
	this->limitTime = true;
	this->passedMinTime = false;
	this->passedOptimalTime = false;
	this->passedSoftEndTime = false;
	this->canSearch = true;

	// Check if position in opening book
	if (useOwnBook && this->openingTable.contains(&cs.bh)) {
		std::vector<Move> moves(this->openingTable.get(&cs.bh));
		
		++nSearches;
		uniform_int_distribution<> distr(0, moves.size()-1);
		return moves[distr(gen)];
	}

	// Timing variables to control the search
	this->minEndTime 	 = this->startTime + chrono::microseconds(movetime - 2000);
	this->optimalEndTime = this->startTime + chrono::microseconds(movetime - 1000);
	this->softEndTime	 = this->startTime + chrono::microseconds(movetime);
	this->hardEndTime	 = this->startTime + chrono::microseconds(movetime + 400);

	pair<Move, EvalScore> ratedMove;
	std::vector<Move> moveList;

	// Loop to increase depth until time is up
	for (short i(1); i<this->MAX_SEARCH_DEPTH; ++i) {
		// Set search iteration parameters
		this->uciDepth = i;

		ratedMove = this->bestMove(&cs, i);

		// Break if no longer has search permission
		// Save the updated move only if it is not null
		if (!this->canSearch) {
			// Use results from incomplete depth
			if (!ratedMove.first.isNull()) {
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

		updateTimingVars();

		if (passedOptimalTime) {
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
	this->uciDepth = 0;
	this->nodesTotal = 0;
	this->canSearch = true;
	this->limitTime = false;

	pair<Move, EvalScore> ratedMove;
	Move m;

	// Cap depth at the max allowed
	if (depth > MAX_SEARCH_DEPTH) {
		depth = MAX_SEARCH_DEPTH;
	}

	// Loop to increase depth until time is up
	for (short i(1); i<=depth; ++i) {

		this->uciDepth = i;

		ratedMove = this->bestMove(&cs, i);

		// If search stopped due to lack of permissions
		if (!this->canSearch) {
			// Use results from incomplete depth
			if (!ratedMove.first.isNull()) {
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
	}

	++nSearches;
	return m;
}

// Searches for the best move until "canSearch" is false.
// Needs to be run in it's own thread in order to execute properly.
Move ChessEngine::searchInfinite(ChessState cs) {

	// Set Stats
	this->startTime = high_resolution_clock::now();
	this->uciDepth = 0;
	this->nodesTotal = 0;
	this->canSearch = true;
	this->limitTime = false;

	pair<Move, EvalScore> ratedMove;
	Move m;

	// Loop to increase depth until time is up
	for (short i(1); i<this->MAX_SEARCH_DEPTH; ++i) {

		this->uciDepth = i;

		ratedMove = this->bestMove(&cs, i);
	
		if (!this->canSearch) {
			// Use results from incomplete depth
			if (!ratedMove.first.isNull()) {
				m = ratedMove.first;
				this->currScore = ratedMove.second;
			}
			break;
		}

		m = ratedMove.first;
		this->currScore = ratedMove.second;
	}

	++nSearches;
	return m;
}
