
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
	transTable = new TranspostionTable(1000000);	// 1 MB
}

ChessEngine::~ChessEngine() {
	delete transTable;
}


// Larry Kaufman's material valuation
const short ChessEngine::materialVals[6] = {
	// https://web.archive.org/web/20160314214435/http://www.danheisman.com/Articles/evaluation_of_material_imbalance.htm
	100, 325, 325, 500, 975, 30000,
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
	// Load pawn bonus tables
	success &= readBonusTable(pawnOpeningBonus[0], "bonus-tables/pawn/opening-game-table.tab");
	success &= readBonusTable(pawnOpeningBonus[1], "bonus-tables/pawn/opening-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnEarlyBonus[0], "bonus-tables/pawn/early-game-table.tab");
	success &= readBonusTable(pawnEarlyBonus[1], "bonus-tables/pawn/early-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnMidBonus[0], "bonus-tables/pawn/mid-game-table.tab");
	success &= readBonusTable(pawnMidBonus[1], "bonus-tables/pawn/mid-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnLateBonus[0], "bonus-tables/pawn/late-game-table.tab");
	success &= readBonusTable(pawnLateBonus[1], "bonus-tables/pawn/late-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnEndBonus[0], "bonus-tables/pawn/end-game-table.tab");
	success &= readBonusTable(pawnEndBonus[1], "bonus-tables/pawn/end-game-table.tab", Bitboard::SHOW_ORDER_FLIPPED);

	// Load built-in opening book
	success &= readOpeningBook(&openingTable, "opening-book.dat");

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
	currScore = EvalScore::DEFAULT;
	nodesTotal = 0;
	this->transTable->clear();
	this->pvTable.clear();

	for (short i(0); i<MAX_SEARCH_DEPTH; ++i) {
		killerHeuristic[i][0] = Move::NULL_MOVE;
		killerHeuristic[i][1] = Move::NULL_MOVE;
	}
}

// Updates variables keeping track of search time
void ChessEngine::updateTimingVars() {
	// Check if we have passed the minimum search time
	if (!this->passedMinTime) {
		if (this->minEndTime < chrono::high_resolution_clock::now()) {
			this->passedMinTime = true;
			// If passed, check if we have passed the optimal search time
			if (!this->passedOptimalTime) {
				if (this->optimalEndTime < chrono::high_resolution_clock::now()) {
					this->passedOptimalTime = true;
					// If passed, check if we hit hard cut off
					if (this->hardEndTime < chrono::high_resolution_clock::now()) {
						this->canSearch = false;
					}
				}
			}
		}
	// Check if we have passed the optimal search time
	} else if (!this->passedOptimalTime) {
		if (this->optimalEndTime < chrono::high_resolution_clock::now()) {
			this->passedOptimalTime = true;
			// If passed, check if we hit hard cut off
			if (this->hardEndTime < chrono::high_resolution_clock::now()) {
				this->canSearch = false;
			}
		}
	// Check if we hit hard cut off
	} else if (this->canSearch) {
		if (this->hardEndTime < chrono::high_resolution_clock::now()) {
			this->canSearch = false;
		}
	}
}

// ----- Primary Operations -----

// Head of recursive negamax search for the best move
pair<Move, EvalScore> ChessEngine::bestMove(ChessState* cs, U8 depth) {
	
	// Generate psudo-legal moves
	U8 moveCount;		// Number of moves (in moveArr)
	genAllMoves(cs, moveArr[0], &moveCount);
	
	// Check if valid moves were generated
	if (moveCount == 0) {
		throw ChessState::NoMoves();
	}

	this->sortMoves(moveArr[0], &moveCount, 0);

	EvalScore alpha = -EvalScore::INFINITE;	// best score current color can achive 
	EvalScore beta = EvalScore::INFINITE;	// best score other color can achive
	short bestIndex(-1);	    // -1 as default

	EvalScore score;
	TTEntry ttEntry;			// Transposition table entry

	for (U8 i(0); i<moveCount; ++i) {

		// Every 4096 nodes, check search status
		if (this->limitTime && (this->nodesTotal & 4095) == 0) {
			updateTimingVars();
		}

		// Exit if no longer allowed to search
		if (this->canSearch == false) {
			// Return a nullmove if no moves have been searched
			if (bestIndex == -1) {
				return make_pair(Move::NULL_MOVE, alpha);
			} else {
				return make_pair(moveArr[0][bestIndex], alpha);
			}
		}

		cs->move(moveArr[0][i]);

		// Use hash table value if it exists
		if (this->transTable->contains(&cs->bh)) {
			ttEntry = this->transTable->get(&cs->bh);
			
			if (ttEntry.depth >= depth) {
				if (ttEntry.score > alpha) {
					alpha = ttEntry.score;
					bestIndex = i;
					pvTable[0][0] = moveArr[0][i];
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

			if (score.hasMate()) {
				score.addHalfMoveToMate();
			}

			// This calculation must be the best value b/c there was no
			// previous calculation good enough to use in its place
			this->transTable->add(&cs->bh, score, depth);

			if (score > alpha) {
				alpha = score;
				bestIndex = i;
				pvTable[0][0] = moveArr[0][i];
				pvTable.copyNext(0);
			}
		}

		cs->reverseMove();
	}

	// If there is no valid move, it is either checkmate or stalemate
	if (bestIndex == -1) {
		// If the active player's king is not being attacked
		// then the situation is stalemate
		if (!isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][cs->KING].getFirstPos())) {
			return make_pair(Move::NULL_MOVE, EvalScore::DRAW);
		}

		// Else there is checkmate
		return make_pair(Move::NULL_MOVE, -EvalScore::MATE_IN_0);
	}

	return make_pair(moveArr[0][bestIndex], alpha);
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
	U8 moveCount;		// Number of moves (in moveArr)
	genAllMoves(cs, moveArr[depth], &moveCount);
	
	// Check if valid moves were generated
	if (moveCount == 0) {
		throw ChessState::NoMoves();
	}

	// Move ordering
	sortMoves(moveArr[depth], &moveCount, depth);

	bool hasValidMove(false);
	EvalScore score;
	TTEntry ttEntry;	// Transposition table entry

	for (U8 i(0); i<moveCount; ++i) {

		// Every 4096 nodes, check search status
		if (this->limitTime && (this->nodesTotal & 4095) == 0) {
			updateTimingVars();
		}

		// Exit if no longer allowed to search
		if (this->canSearch == false) {
			return alpha;	// Return null move
		}

		cs->move(moveArr[depth][i]);

		// Use trans table value if it exists
		if (this->transTable->contains(&cs->bh)) {
			hasValidMove = true;

			ttEntry = this->transTable->get(&cs->bh);

			if (ttEntry.depth >= (depthTarget - depth)) {
				if (ttEntry.score >= beta) {
					cs->reverseMove();
					if (moveArr[depth][i].killed == -1) {
						this->addKillerMove(&moveArr[depth][i], &depth);
					}
					return beta;
				}

				if (ttEntry.score > alpha) {
					alpha = ttEntry.score;
					pvTable[depth][0] = moveArr[depth][i];
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

			if (score.hasMate()) {
				score.addHalfMoveToMate();
			}

			// This calculation must be the best value b/c there was no
			// previous calculation good enough to use in its place
			this->transTable->add(&cs->bh, score, (depthTarget - depth));

			if (score >= beta) {
				cs->reverseMove();
				if (moveArr[depth][i].killed == -1) {
					this->addKillerMove(&moveArr[depth][i], &depth);
				}
				return beta;
			}

			if (score > alpha) {
				alpha = score;
				pvTable[depth][0] = moveArr[depth][i];
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
			return EvalScore::DRAW;
		}

		// Else there is checkmate
		return -EvalScore::MATE_IN_0;
	}

	return alpha;
}
