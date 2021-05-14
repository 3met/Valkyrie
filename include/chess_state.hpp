
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
	void updateAllBitboard(bool colour);

public:
	ChessState();
	ChessState(const ChessState* cs);
	~ChessState();

	enum colour {
		WHITE = 0,	// White must remain 0 for turn indexing to work
		BLACK = 1,
	};
	enum pieceType {
		PAWN = 0,
		KNIGHT,
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

	Bitboard wP;
	Bitboard wN;
	Bitboard wB;
	Bitboard wR;
	Bitboard wQ;
	Bitboard wK;
	Bitboard wAll;

	Bitboard bP;
	Bitboard bN;
	Bitboard bB;
	Bitboard bR;
	Bitboard bQ;
	Bitboard bK;
	Bitboard bAll;

	Bitboard* pieces[2][7] = {
		{&wP, &wN, &wB, &wR, &wQ, &wK, &wAll},
		{&bP, &bN, &bB, &bR, &bQ, &bK, &bAll},
	};

	static const char piece_names[2][6];	// Note: must match piece indexing

	bool turn;	// False for white; true for black
	
	bool wKCastle;	// Castle perms
	bool wQCastle;
	bool bKCastle;
	bool bQCastle;
	bool* castlePerms[2][2] = {
		{&wKCastle, &wQCastle},	// Order based on castleSide enum
		{&bKCastle, &bQCastle},
	};
	short turnLostCastlePerms[2][2];	// Used for reversing moves; indexing based on turn and castleSide
	
	S8 enPassant;	// Pos behind pawn, else -1
	vector<S8> enPassantHistory;	// History of en passant for reverseMove()

	short halfmoveClock;	// # of halfmoves since last capture or pawn move
	short turnNumber;	// Game turn number

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
	S8 getPieceType(bool colour, U8 pos);
	Move lastMove();

	// Setup Methods
	void reset();
	void clear();
	void place(short colour, short piece, short pos);
	pair<bool, U8> charToPiece(char piece);
	void loadFEN(string FEN);
	Move notationToMove(string notation);

	// Playing Methods
	void move(Move m);
	void reverseMove();

	// Output Methods
	void show();
	void show(bool show_coords);
};

#endif
