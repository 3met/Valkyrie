
#ifndef CHESS_STATE_HPP
#define CHESS_STATE_HPP

#include "bitboard.hpp"

using namespace std;

class ChessState {
private:
	Bitboard wP;
	Bitboard wN;
	Bitboard wB;
	Bitboard wR;
	Bitboard wQ;
	Bitboard wK;
	Bitboard wAll;
	vector<Bitboard*> wBitboards;	// References to all white bit boards

	Bitboard bP;
	Bitboard bN;
	Bitboard bB;
	Bitboard bR;
	Bitboard bQ;
	Bitboard bK;
	Bitboard bAll;
	vector<Bitboard*> bBitboards;

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

	bool isLegalMove();
	void mapBoardToChar(Bitboard b, char arr[65], char target);

public:
	ChessState();
	~ChessState();
	void reset();
	void move(short start, short end);
	void move(short start, short end, char promotion);
	void show();
	void show(bool show_coords);

};

#endif
