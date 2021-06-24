
#pragma once
#ifndef BOARD_HASH_HPP
#define BOARD_HASH_HPP

#include "bitboard.hpp"
#include "zobrist_values.hpp"
#include "U64.hpp"

// Zobrist Board Hash used to represent a chess board.
// Hash takes into account piece locations, color to play,
// castle permissions, and en passant square.
class BoardHash {
private:
	// The set of values used to create hashes
	static ZobristValues zobristValues;

public:
	BoardHash();
	BoardHash(const Bitboard pieces[2][7], const bool turn,
		const bool castlePerms[2][2], const U8 enPassant);
	~BoardHash();

	U64 hash;

	// Used to compare hashes
	friend bool operator<(const BoardHash& a, const BoardHash& b) {
		return a.hash < b.hash;
	};

	// Creates a hash from ChessState data
	void makeHash(const Bitboard pieces[2][7], const bool turn,
		const bool castlePerms[2][2], const U8 enPassant);

	// -- Hash Updating Functions --
	// These function update the hash as the state of the chess board changes
	void updatePiece(bool color, U8 pieceType, U8 pos);
	void updateTurn();
	void updateCastlePerms(bool color, bool side);
	void updateEnPassant(const U8 oldPos, const U8 newPos);
};

#endif
