
// --- In this File ---
// 1. ChessEngine Constructor
// 2. Material value sets
// 3. Best move calculations (negamax, alpha-beta)

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <utility>
#include "board_defs.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "eval_score.hpp"
#include "move.hpp"
#include "transposition_table.hpp"

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

	// Load data directory string
	success &= loadDataDir();

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
	success &= readBonusTable(kingBonus[OPENING][0], "bonus-tables/king/opening.tab");
	success &= readBonusTable(kingBonus[OPENING][1], "bonus-tables/king/opening.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingBonus[LATE_OPENING][0], "bonus-tables/king/late-opening.tab");
	success &= readBonusTable(kingBonus[LATE_OPENING][1], "bonus-tables/king/late-opening.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingBonus[EARLY_GAME][0], "bonus-tables/king/early-game.tab");
	success &= readBonusTable(kingBonus[EARLY_GAME][1], "bonus-tables/king/early-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingBonus[LATE_EARLY_GAME][0], "bonus-tables/king/late-early-game.tab");
	success &= readBonusTable(kingBonus[LATE_EARLY_GAME][1], "bonus-tables/king/late-early-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingBonus[MID_GAME][0], "bonus-tables/king/mid-game.tab");
	success &= readBonusTable(kingBonus[MID_GAME][1], "bonus-tables/king/mid-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingBonus[LATE_MID_GAME][0], "bonus-tables/king/late-mid-game.tab");
	success &= readBonusTable(kingBonus[LATE_MID_GAME][1], "bonus-tables/king/late-mid-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingBonus[END_GAME][0], "bonus-tables/king/end-game.tab");
	success &= readBonusTable(kingBonus[END_GAME][1], "bonus-tables/king/end-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(kingBonus[LATE_END_GAME][0], "bonus-tables/king/late-end-game.tab");
	success &= readBonusTable(kingBonus[LATE_END_GAME][1], "bonus-tables/king/late-end-game.tab", SHOW_ORDER_FLIPPED);
	// Load pawn bonus tables
	success &= readBonusTable(pawnBonus[OPENING][0], "bonus-tables/pawn/opening.tab");
	success &= readBonusTable(pawnBonus[OPENING][1], "bonus-tables/pawn/opening.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnBonus[LATE_OPENING][0], "bonus-tables/pawn/late-opening.tab");
	success &= readBonusTable(pawnBonus[LATE_OPENING][1], "bonus-tables/pawn/late-opening.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnBonus[EARLY_GAME][0], "bonus-tables/pawn/early-game.tab");
	success &= readBonusTable(pawnBonus[EARLY_GAME][1], "bonus-tables/pawn/early-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnBonus[LATE_EARLY_GAME][0], "bonus-tables/pawn/late-early-game.tab");
	success &= readBonusTable(pawnBonus[LATE_EARLY_GAME][1], "bonus-tables/pawn/late-early-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnBonus[MID_GAME][0], "bonus-tables/pawn/mid-game.tab");
	success &= readBonusTable(pawnBonus[MID_GAME][1], "bonus-tables/pawn/mid-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnBonus[LATE_MID_GAME][0], "bonus-tables/pawn/late-mid-game.tab");
	success &= readBonusTable(pawnBonus[LATE_MID_GAME][1], "bonus-tables/pawn/late-mid-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnBonus[END_GAME][0], "bonus-tables/pawn/end-game.tab");
	success &= readBonusTable(pawnBonus[END_GAME][1], "bonus-tables/pawn/end-game.tab", SHOW_ORDER_FLIPPED);
	success &= readBonusTable(pawnBonus[LATE_END_GAME][0], "bonus-tables/pawn/late-end-game.tab");
	success &= readBonusTable(pawnBonus[LATE_END_GAME][1], "bonus-tables/pawn/late-end-game.tab", SHOW_ORDER_FLIPPED);

	// Load built-in opening book
	success &= readOpeningBook(&openingTable, "opening-book.dat");

	if (success) {
		isLoaded = true;
	} else {
		printf("Error: Engine failed to load\n");
		isLoaded = false;
	}
}

// Deleted memory allocated during load
void ChessEngine::unload() {
	isLoaded = false;
	
	for (U8 i=0; i<64; ++i) {
		delete [] bishopAttackTable[i];
		delete [] rookAttackTable[i];
	}
}

// Clear all temporary data
void ChessEngine::clear() {
	nSearches = 0;	// Number of searches preformed
	uciDepth = 0;
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
					// If passed, check if we hit soft cut off
					if (this->softEndTime < chrono::high_resolution_clock::now()) {
						this->passedSoftEndTime = true;
						// If passed, check if we hit hard cut off
						if (this->hardEndTime < chrono::high_resolution_clock::now()) {
							this->canSearch = false;
						}
					}
				}
			}
		}
	// Check if we have passed the optimal search time
	} else if (!this->passedOptimalTime) {
		if (this->optimalEndTime < chrono::high_resolution_clock::now()) {
			this->passedOptimalTime = true;
			// If passed, check if we hit soft cut off
			if (this->softEndTime < chrono::high_resolution_clock::now()) {
				this->passedSoftEndTime = true;
				// If passed, check if we hit hard cut off
				if (this->hardEndTime < chrono::high_resolution_clock::now()) {
					this->canSearch = false;
				}
			}
		}
	// Check if we hit soft cut off
	} else if (!this->passedSoftEndTime) {
		if (this->softEndTime < chrono::high_resolution_clock::now()) {
			this->passedSoftEndTime = true;
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
	
	// Check if the calculation has already been made
	TTEntry* hashEntry = transTable->getEntryPointer(&cs->bh);
	if (hashEntry->bh == cs->bh && hashEntry->depth >= depth) {
		if (hashEntry->scoreType == hashEntry->EXACT_SCORE) {
			return make_pair(hashEntry->bestMove, hashEntry->score);
		}
	}

	// Generate psudo-legal moves
	U8 moveCount;		// Number of moves (in moveArr)
	genAllMoves(cs, moveArr[0], &moveCount);

	if (hashEntry->bh == cs->bh) {
		sortMain(moveArr[0], 0, moveCount-1, cs, 0, hashEntry->bestMove);
	} else {
		sortMainNoHash(moveArr[0], 0, moveCount-1, cs, 0);
	}

	EvalScore alpha = -EvalScore::INFINITE;	// best score current color can achive 
	EvalScore beta = EvalScore::INFINITE;	// best score other color can achive
	this->inNullMoveSearch = false;
	short bestIndex(-1);	    // -1 as default

	EvalScore score;
	TTEntry ttEntry;			// Transposition table entry

	for (U8 i(0); i<moveCount; ++i) {

		// Every 4096 nodes, check search status
		if (this->limitTime && (this->nodesTotal & U64(4095)) == 0) {
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

		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][KING].getFirstPos())) {
			
			// Check for draw by repetition or 50-moves
			if (cs->halfmoveClock >= 8) {
				// Check for three-move repetition
				if (cs->isThreeRepetition()) {
					score = EvalScore::DRAW;
				// Check for draw by 50-move rule
				} else if (cs->is50MoveDraw()) {
					score = EvalScore::DRAW;
				} else {
					score = -negamaxSearch(cs, depth-1, 1, -beta, -alpha);
				}
			} else {
				score = -negamaxSearch(cs, depth-1, 1, -beta, -alpha);
			}

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
		if (!isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][KING].getFirstPos())) {
			return make_pair(Move::NULL_MOVE, EvalScore::DRAW);
		}

		// Else there is checkmate
		return make_pair(Move::NULL_MOVE, -EvalScore::MATE_IN_0);
	}

	hashEntry->setMoveData(&cs->bh, depth, alpha, hashEntry->EXACT_SCORE, moveArr[0][bestIndex]);
	return make_pair(moveArr[0][bestIndex], alpha);
}

// Recursive negamax search for the best move
EvalScore ChessEngine::negamaxSearch(ChessState* cs, U8 depth, U8 ply, EvalScore alpha, EvalScore beta) {

	// Check for draw by repetition or 50-moves
	if (cs->halfmoveClock >= 8) {
		// Check for three-move repetition
		if (cs->isThreeRepetition()) {
			return EvalScore::DRAW;
		}
		// Check for draw by 50-move rule
		if (cs->is50MoveDraw()) {
			return EvalScore::DRAW;
		}
	}

	// Check if the calculation has already been made
	TTEntry* hashEntry = transTable->getEntryPointer(&cs->bh);
	if (hashEntry->bh == cs->bh && hashEntry->depth >= depth) {
		if (hashEntry->scoreType == hashEntry->EXACT_SCORE) {
			return hashEntry->score;
		}
		if (hashEntry->scoreType == hashEntry->ALPHA_SCORE
			&& hashEntry->score <= alpha) {
			return alpha;
		}
		if (hashEntry->scoreType == hashEntry->BETA_SCORE
			&& hashEntry->score >= beta) {
			return beta;
		}
	}

	EvalScore score;
	
	if (depth == 0) {
		score = quiescence(cs, ply+1, alpha, beta);
		hashEntry->setScoreData(&cs->bh, 0, score, hashEntry->EXACT_SCORE);
		return score;
	}

	// Null Move Pruning.
	// If not already in a null move search and not currently in check.
	if (!this->inNullMoveSearch && depth > 2 && cs->zugzwangSafe()) {

		this->inNullMoveSearch = true;
		cs->moveNull();

		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][KING].getFirstPos())) {

			U8 R = 2 + (depth / 5);

			score = -zwSearch(cs, depth-1-R, ply+1, -beta+1);

			if (score >= beta) {
				cs->reverseNullMove();
				this->inNullMoveSearch = false;
				return beta;
			}
		}

		cs->reverseNullMove();
		this->inNullMoveSearch = false;
	}

	// Generate and sort moves
	U8 moveCount;		// Number of moves (in moveArr)
	genAllMoves(cs, moveArr[ply], &moveCount);
	
	// Move ordering
	if (hashEntry->bh == cs->bh) {
		sortMain(moveArr[ply], 0, moveCount-1, cs, ply, hashEntry->bestMove);
	} else {
		sortMainNoHash(moveArr[ply], 0, moveCount-1, cs, ply);
	}
	
	bool hasValidMove(false);
	short bestIndex(-1);

	for (U8 i(0); i<moveCount; ++i) {

		// Every 4096 nodes, check search status
		if (this->limitTime && (this->nodesTotal & 4095) == 0) {
			updateTimingVars();
		}

		// Exit if no longer allowed to search
		if (this->canSearch == false) {
			return alpha;
		}

		cs->move(moveArr[ply][i]);

		// Check if move is legal before preceding
		if (!isPosAttacked(cs, cs->turn, cs->pieces[!cs->turn][KING].getFirstPos())) {
			
			score = -negamaxSearch(cs, depth-1, ply+1, -beta, -alpha);

			hasValidMove = true;

			if (score >= beta) {
				cs->reverseMove();
				if (moveArr[ply][i].getFlags() == Move::QUIET) {
					this->addKillerMove(&moveArr[ply][i], &ply);
				}
				hashEntry->setScoreData(&cs->bh, depth, beta, hashEntry->BETA_SCORE);
				return beta;
			}

			if (score > alpha) {
				bestIndex = i;
				alpha = score;
				pvTable[ply][0] = moveArr[ply][i];
				pvTable.copyNext(ply);
			}
		}

		cs->reverseMove();
	}

	// If there is no valid move, it is either checkmate or stalemate
	if (!hasValidMove) {
		// If the active player's king is not being attacked
		// then the situation is stalemate
		if (!isPosAttacked(cs, !cs->turn, cs->pieces[cs->turn][KING].getFirstPos())) {
			hashEntry->setScoreData(&cs->bh, 255, EvalScore::DRAW, hashEntry->EXACT_SCORE);
			return EvalScore::DRAW;
		}

		// Else there is checkmate
		return -EvalScore::MATE_IN_0 + ply;
	}

	if (bestIndex == -1) {
		hashEntry->setScoreData(&cs->bh, depth, alpha, hashEntry->ALPHA_SCORE);
	} else {
		hashEntry->setMoveData(&cs->bh, depth, alpha, hashEntry->EXACT_SCORE, moveArr[ply][bestIndex]);
	}
	
	return alpha;
}
