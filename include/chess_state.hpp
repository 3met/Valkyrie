
#pragma once
#ifndef CHESS_STATE_HPP
#define CHESS_STATE_HPP

#include <array>
#include <exception>
#include <stack>
#include <string>
#include <utility>
#include "bitboard.hpp"
#include "board_hash.hpp"
#include "move.hpp"
#include "size_defs.hpp"

using namespace std;

class ChessState {
private:
	// Misc Methods
	static void mapBoardToChar(Bitboard b, char arr[64], char target);
	void updateAllBitboard(bool color);

public:
	enum Color {
		WHITE = 0,	// White must remain 0 for indexing to work
		BLACK = 1,
	};
	enum PieceType {
		PAWN = 0,	// Pawn must remain 0 for indexing to work
		KNIGHT,		// Changing order may cause errors
		BISHOP,
		ROOK,
		QUEEN,
		KING,
		ALL_PIECES,
	};
	enum CastleSide {
		KING_SIDE = 0,
		QUEEN_SIDE,
	};

	ChessState();
	~ChessState();

	// Sort chess state by turn
	friend bool operator<(const ChessState& a, const ChessState& b) {
		return a.turn < b.turn;
	}

	static const char piece_names[2][6];	// Note: must match piece indexing
	// Stores king and rook starting positions for castling
	static const U8 KING_START[2];		// [color]
	static const U8 ROOK_START[2][2];	// [color][king/queen side]

	// Maximum number of moves the chess state will store
	static const short MAX_MOVES = 1024;

	// Bitboard to store piece locations
	Bitboard pieces[2][7];	// [colors][p/n/b/r/q/k/all]

	bool turn;	// False for white; true for black

	bool castlePerms[2][2];				// [color][king/queen side]
	short moveLostCastlePerms[2][2];	// Used for reversing moves; indexing based on turn and castleSide
	
	array<S8, MAX_MOVES> enPassantHistory;		// History of en passant for reverseMove()

	array<Move, MAX_MOVES> moveHistory;	// List of moves that lead to current game state
	
	short halfmoveClock;		// # of halfmoves since last capture or pawn move
	short turnNumber;			// Game turn number
	short moveNumber;

	BoardHash bh;	// Current chess board hash

	// TODO: add Threefold repetition
	// https://en.wikipedia.org/wiki/Threefold_repetition

	// --- Custom Exception ---
	struct NoMoves : public exception {
		const char * what () const throw () {
			return "There are no legal moves.";
		}
	};
	
	// Move Methods
	void move(Move m);
	void reverseMove();

	// Configuration Methods
	void clear();
	void loadFEN(string FEN);
	void place(bool color, U8 piece, U8 pos);
	void reset();

	// Printing Methods
	string stringFEN();
	void show();
	void show(bool show_coords);

	// Misc Methods
	Move lastMove();
	S8 getPieceType(bool color, U8 pos);
	Move notationToMove(string notation);
	static pair<bool, U8> charToPiece(char piece);
};

#endif
