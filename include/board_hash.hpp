
#pragma once
#ifndef BOARD_HASH_HPP
#define BOARD_HASH_HPP

#include "bitboard.hpp"
#include "U64.hpp"
#include "zobrist_values.hpp"

/* Zobrist Board Hash */
class BoardHash {
private:
	U64 hash;
	static ZobristValues zobristValues;

public:
	BoardHash();
	BoardHash(const Bitboard pieces[2][7], const bool turn,
		const bool castlePerms[2][2], const U8 enPassant);
	~BoardHash();

	friend bool operator<(const BoardHash& a, const BoardHash& b) {
		return a.hash < b.hash;
	}

	void makeHash(const Bitboard pieces[2][7], const bool turn,
		const bool castlePerms[2][2], const U8 enPassant);

	// -- Hash Updating Functions --
	void updatePiece(bool color, U8 pieceType, U8 pos);
	void updateTurn();
	void updateCastlePerms(bool color, bool side);
	void updateEnPassant(const U8 oldPos, const U8 newPos);
};

#endif
