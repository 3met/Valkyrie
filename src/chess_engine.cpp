
// --- In this File ---
// 1. ChessEngine Constructor
// 2. Material value sets
// 3. Best move calculations (negamax, alpha-beta)

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <utility>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"

using namespace std;

ChessEngine::ChessEngine() {
	this->load();
	transTable = new TranspostionTable(67108864);	// 64 MiB
}

ChessEngine::~ChessEngine() {
	delete transTable;
}

// ----- Scoring Game State -----
// Standard material valuation
const short ChessEngine::materialValsSTD[6] = {
	100, 300, 300, 500, 900, 30000,
};

// Larry Kaufman's material valuation
const short ChessEngine::materialValsLK[6] = {
	// https://web.archive.org/web/20160314214435/http://www.danheisman.com/Articles/evaluation_of_material_imbalance.htm
	100, 325, 325, 500, 975, 30000,
};

// Hans Berliner's material valuation
const short ChessEngine::materialValsHB[6] = {
	// https://en.wikipedia.org/wiki/Chess_piece_relative_value#Hans_Berliner's_system
	100, 320, 333, 510, 880, 30000,
};

bool ChessEngine::isLoaded = false;

// Load data nessesary to operate chess engine
void ChessEngine::load() {
	isLoaded = false;

	bool success(true);

	// Load knight and king moves
	success &= readBitboardTable(NMoveDB, "move-tables/knight/moves.dat");
	success &= readBitboardTable(KMoveDB, "move-tables/king/moves.dat");
	// Load magic bitboard data
	success &= readBitboardTable(bishopMasks, "magic-bitboards/bishop/masks.tab");
	success &= readBitboardTable(rookMasks, "magic-bitboards/rook/masks.tab");
	success &= readMagicTable(bishopMagicShifts, bishopMagics, "magic-bitboards/bishop/magics.tab");
	success &= readMagicTable(rookMagicShifts, rookMagics, "magic-bitboards/rook/magics.tab");
	success &= readAttackTable(bishopAttackTable, "magic-bitboards/bishop/attack-tables/");
	success &= readAttackTable(rookAttackTable, "magic-bitboards/rook/attack-tables/");

	// Load knight, bishop, and queen bonus tables
	success &= readBonusTable(knightBonus, "bonus-tables/knight/table.tab");
	success &= readBonusTable(bishopBonus, "bonus-tables/bishop/table.tab");
	success &= readBonusTable(queenBonus, "bonus-tables/queen/table.tab");
	// Load king bonus tables
	success &= readBonusTable(kingOpeningBonus[0], "bonus-tables/king/opening-game-table.tab");
	success &= readBonusTable(kingOpeningBonus[1], "bonus-tables/king/opening-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingEarlyBonus[0], "bonus-tables/king/early-game-table.tab");
	success &= readBonusTable(kingEarlyBonus[1], "bonus-tables/king/early-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingMidBonus[0], "bonus-tables/king/mid-game-table.tab");
	success &= readBonusTable(kingMidBonus[1], "bonus-tables/king/mid-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingLateBonus[0], "bonus-tables/king/late-game-table.tab");
	success &= readBonusTable(kingLateBonus[1], "bonus-tables/king/late-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingEndBonus[0], "bonus-tables/king/end-game-table.tab");
	success &= readBonusTable(kingEndBonus[1], "bonus-tables/king/end-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	// Load built-in opening book
	// success &= readOpeningBook(&openingTable, "opening_book.dat");

	if (success) {
		isLoaded = true;
	} else {
		cout << "Error: Engine failed to load" << endl;
		isLoaded = false;
	}
}

// Clear all temporary data
void ChessEngine::clear() {
	nSearches = 0;	// Number of searches preformed
	currDepth = 0;
	currSelDepth = 0;
	currScore = EvalScore(0);
	nodesTotal = 0;
	this->transTable->clear();
	this->pvTable.clear();

	Move nullMove = Move();
	for (short i(0); i<MAX_SEARCH_DEPTH; ++i) {
		killerHeuristic[i][0] = nullMove;
		killerHeuristic[i][1] = nullMove;
	}
}

// ----- Primary Operations -----

// Head of recursive negamax search for the best move
pair<Move, EvalScore> ChessEngine::bestMove(ChessState* cs, U8 depth) {
	
	// Generate psudo-legal moves
	vector<Move> moves;
	genAllMoves(cs, &moves);
	
	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	this->sortMoves(&moves, 0);

	EvalScore alpha(-1, true, 0);	// -INF; best score current color can achive 
	EvalScore beta(1, true, 0);	// INF; best score other color can achive
	short bestIndex(-1);	    // -1 as default

	EvalScore score;
	TTEntry ttEntry;			// Transposition table entry

	for (U8 i(0); i<moves.size(); ++i) {

		// Exit if no longer allowed to search
		if (this->canSearch == false) {
			return make_pair(Move(), alpha);	// Return null move
		}

		// Every 4096 nodes, check search status
		if (this->limitTime && (this->nodesTotal & 4095) == 0) {
			// Check if we have passed the optimal search time
			if (!this->passedOptimalTime) {
				if (this->optimalEndTime < chrono::high_resolution_clock::now()) {
					this->passedOptimalTime = true;
					// If passed, check if we hit hard cut off
					if (this->hardEndTime < chrono::high_resolution_clock::now()) {
						this->canSearch = false;
					}
				}
			// If passed, check if we hit hard cut off
			} else {
				if (this->hardEndTime < chrono::high_resolution_clock::now()) {
					this->canSearch = false;
				}
			}
		}

		cs->move(moves[i]);

		// Use hash table value if it exists
		if (this->transTable->contains(&cs->bh)) {
			ttEntry = this->transTable->get(&cs->bh);
			
			if (ttEntry.depth >= depth) {
				if (ttEntry.score > alpha) {
					alpha = ttEntry.score;
					bestIndex = i;
					pvTable[0][0] = moves[i];
					// TODO: NO NEXT TO COPY ....?
				}

				// Continue to next move at this point because the hash 
				// table data has already looked further than we plan to
				cs->reverseMove();
				continue;
			}
		}

		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING].getFirstPos())) {
			
			score = -negamaxSearch(cs, 1, depth, -beta, -alpha);

			if (score.foundMate) {
				score.movesToMate += 1;
			}

			// This calculation must be the best value b/c there was no
			// previous calculation good enough to use in its place
			this->transTable->add(&cs->bh, score, depth);

			if (score > alpha) {
				alpha = score;
				bestIndex = i;
				pvTable[0][0] = moves[i];
				pvTable.copyNext(0);
			}
		}

		cs->reverseMove();
	}

	// Throw error if no move were legal
	if (bestIndex == -1) {
		throw ChessState::NoMoves();
	}

	return make_pair(moves[bestIndex], alpha);
}

// Recursive negamax search for the best move
EvalScore ChessEngine::negamaxSearch(ChessState* cs, U8 depth, U8 depthTarget, EvalScore alpha, EvalScore beta) {

	// Check for recursion termination
	if (depth == depthTarget) {
		// Return static if reached max depth
		if (depthTarget >= this->maxDepth) {
			return EvalScore(evalBoard(cs, cs->turn));
		// Extend if last move was a kill
		} else if (cs->lastMove().killed != -1) {
			depthTarget += 1;
		// Extend if a pawn was promoted
		} else if (cs->lastMove().promoted != -1) {
			depthTarget += 1;
		// Extend if move was a check
		// } else if (isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][cs->KING].getFirstPos())) {
			// depthTarget += 2;
		} else {
			return EvalScore(evalBoard(cs, cs->turn));
		}		 
	}

	// Generate and sort moves
	vector<Move> moves;
	genAllMoves(cs, &moves);
	
	// Check if valid moves were generated
	if (moves.size() == 0) {
		throw ChessState::NoMoves();
	}

	// Move ordering
	sortMoves(&moves, depth);

	bool hasValidMove(false);
	EvalScore score;
	TTEntry ttEntry;	// Transposition table entry

	for (U8 i(0); i<moves.size(); ++i) {

		// Exit if no longer allowed to search
		if (this->canSearch == false) {
			return alpha;	// Return null move
		}

		// Every 4096 nodes, check search status
		if (this->limitTime && (this->nodesTotal & 4095) == 0) {
			// Check if we have passed the optimal search time
			if (!this->passedOptimalTime) {
				if (this->optimalEndTime < chrono::high_resolution_clock::now()) {
					this->passedOptimalTime = true;
					// If passed, check if we hit hard cut off
					if (this->hardEndTime < chrono::high_resolution_clock::now()) {
						this->canSearch = false;
					}
				}
			// If passed, check if we hit hard cut off
			} else {
				if (this->hardEndTime < chrono::high_resolution_clock::now()) {
					this->canSearch = false;
				}
			}
		}

		cs->move(moves[i]);

		// Use trans table value if it exists
		if (this->transTable->contains(&cs->bh)) {
			hasValidMove = true;

			ttEntry = this->transTable->get(&cs->bh);

			if (ttEntry.depth >= (depthTarget - depth)) {
				if (ttEntry.score >= beta) {
					cs->reverseMove();
					if (moves[i].killed == -1) {
						this->addKillerMove(&moves[i], &depth);
					}
					return beta;
				}

				if (ttEntry.score > alpha) {
					alpha = ttEntry.score;
					pvTable[depth][0] = moves[i];
					// TODO: NO NEXT TO COPY ....?
				}

				// Continue to next move at this point because the hash 
				// table data has already looked further than we plan to
				cs->reverseMove();
				continue;
			}
		}

		// Check if move is legal before preceding
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][cs->KING].getFirstPos())) {
			
			score = -negamaxSearch(cs, depth+1, depthTarget, -beta, -alpha);

			hasValidMove = true;

			if (score.foundMate) {
				score.movesToMate += 1;
			}

			// This calculation must be the best value b/c there was no
			// previous calculation good enough to use in its place
			this->transTable->add(&cs->bh, score, (depthTarget - depth));

			if (score >= beta) {
				cs->reverseMove();
				if (moves[i].killed == -1) {
					this->addKillerMove(&moves[i], &depth);
				}
				return beta;
			}

			if (score > alpha) {
				alpha = score;
				pvTable[depth][0] = moves[i];
				pvTable.copyNext(depth);
			}
		}

		cs->reverseMove();
	}

	// If there is no valid move, it is either checkmate or stalemate
	if (!hasValidMove) {
		// If the active player's king is not being attacked
		// then the situation is stalemate
		if (!isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][cs->KING].getFirstPos())) {
			return EvalScore(0);
		}

		// Else there is checkmate
		return EvalScore(-1, true, 0);
	}

	return alpha;
}
