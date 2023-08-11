
#include "chess_engine.hpp"
#include "move.hpp"
#include "size_defs.hpp"

// File: get_set.cpp
// File holds ChessEngine methods that get things,
// set things, setup things, and reset things.

// Add killer heuristic move
void ChessEngine::addKillerMove(Move* m, U8* depth) {
	killerHeuristic[*depth][killerHeuristicNext[*depth]] = *m;
	killerHeuristicNext[*depth] = !killerHeuristicNext[*depth];
}

// Sets up the PvTable for a search
// Shifts or clears the PvTable as needed based on the
// timeline between the previous and current search.
void ChessEngine::setupPvTable(ChessState* cs) {

	// Clear the table if we have gone back in time
	if (lastSearchHalfmoveClock > cs->halfmoveClock) {
		this->pvTable.clear();
	
	// Shift the table if we have gone forward in time
	} else if (lastSearchHalfmoveClock < cs->halfmoveClock) {
		// If the shift distance is too large, just clear the table instead
		if (cs->halfmoveClock - lastSearchHalfmoveClock >= this->MAX_SEARCH_DEPTH) {
			this->pvTable.clear();
		} else {
			this->pvTable.shiftForward(cs->halfmoveClock - lastSearchHalfmoveClock);
		}
	}

	// Else there is nothing to do if halfmoveClock has not changed between searches
	return;
}
