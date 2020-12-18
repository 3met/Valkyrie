
#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "bitboard.hpp"

class ChessState {
private:
	Bitboard wP;
	Bitboard wN;
	Bitboard wB;
	Bitboard wR;
	Bitboard wQ;
	Bitboard wK;
	Bitboard bP;
	Bitboard bN;
	Bitboard bB;
	Bitboard bR;
	Bitboard bQ;
	Bitboard bK;
	
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

public:
	ChessState();
	~ChessState();
	void reset();

};

#endif
