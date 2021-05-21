
#pragma once
#ifndef CHESS_STATE_HPP
#define CHESS_STATE_HPP

#include <exception>
#include <string>
#include <utility>
#include "bitboard.hpp"
#include "move.hpp"
#include "U8.hpp"
#include "S8.hpp"

using namespace std;

class ChessState {
private:
	void mapBoardToChar(Bitboard b, char arr[64], char target);
	void updateAllBitboard(bool color);

public:
	ChessState();
	ChessState(const ChessState* cs);
	~ChessState();

	enum color {
		WHITE = 0,	// White must remain 0 for indexing to work
		BLACK = 1,
	};
	enum pieceType {
		PAWN = 0,	// Pawn must remain 0 for indexing to work
		KNIGHT,		// Changing order may cause errors
		BISHOP,
		ROOK,
		QUEEN,
		KING,
		ALL_PIECES,
	};
	enum castleSide {
		KING_SIDE = 0,
		QUEEN_SIDE,
	};

	// Sort chess state by turn
	friend bool operator<(const ChessState& a, const ChessState& b) {
		return a.turn < b.turn;
	}

	static const char piece_names[2][6];	// Note: must match piece indexing
	// Stores king and rook starting positions for castling
	static const U8 KING_START[2];	// [color]
	static const U8 ROOK_START[2][2];	// [color][king/queen side]

	// Bitboard to store piece locations
	Bitboard pieces[2][7];	// [colors][p/n/b/r/q/k/all]

	bool turn;	// False for white; true for black

	bool castlePerms[2][2];	// [color][king/queen side]
	short moveLostCastlePerms[2][2];	// Used for reversing moves; indexing based on turn and castleSide
	
	S8 enPassant;	// Pos behind pawn, else -1
	vector<S8> enPassantHistory;	// History of en passant for reverseMove()

	short halfmoveClock;	// # of halfmoves since last capture or pawn move
	short turnNumber;	// Game turn number
	short moveNumber;

	vector<Move> moveList;	// List of moves that lead to current game state

	// TODO: add Threefold repetition
	// https://en.wikipedia.org/wiki/Threefold_repetition

	// --- Custom Exceptions ---
	struct NoMoves : public exception {
		const char * what () const throw () {
			return "There are no legal moves.";
		}
	};
	
	// Query Methods
	S8 getPieceType(bool color, U8 pos);
	Move lastMove();

	// Setup Methods
	void reset();
	void clear();
	void place(short color, short piece, short pos);
	pair<bool, U8> charToPiece(char piece);
	void loadFEN(string FEN);
	Move notationToMove(string notation);

	// Playing Methods
	void move(Move m);
	void reverseMove();

	// Output Methods
	string stringFEN();
	void show();
	void show(bool show_coords);
};

#endif
