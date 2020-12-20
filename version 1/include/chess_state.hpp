
#ifndef CHESS_STATE_HPP
#define CHESS_STATE_HPP

#include "bitboard.hpp"
#include "move.hpp"

using namespace std;

class ChessState {
private:
	void mapBoardToChar(Bitboard b, char arr[65], char target);
	void updateAllBitboard(bool turn);

public:
	enum colour {
		white = 0,	// White must remain 0 for turn indexing to work
		black = 1,
	};
	enum piece_type {
		pawn = 0,
		knight,
		bishop,
		rook,
		queen,
		king,
		all_pieces,
	};

	Bitboard* pieces[2][7];	// Indexed as ordered below

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

	static const char piece_names[2][6];	// Note: must match piece indexing

	bool turn;	// True for white; false for black
	
	bool wKCastle;	// Castle perms
	bool wQCcastle;
	bool bKCastle;
	bool bQCastle;
	
	char en_passant[3];	// Square behind pawn else "-"
	short halfmoveClock;	// # of halfmoves since last capture or pawn move
	short moveNumber;	// Game turn number

	// TODO: add Threefold repetition
	// https://en.wikipedia.org/wiki/Threefold_repetition

	ChessState();
	~ChessState();
	bool isLegalMove();
	void reset();
	void move(Move m);
	void move(short start, short end);
	void move(short start, short end, char promotion);
	void show();
	void show(bool show_coords);
};

#endif
