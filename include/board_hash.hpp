
#pragma once
#ifndef BOARD_HASH_HPP
#define BOARD_HASH_HPP

#include "bitboard.hpp"
#include "chess_state.hpp"
#include "U64.hpp"
#include "zobrist_values.hpp"

/* Zobrist Board Hash */
class BoardHash {
private:
	U64 hash;
	static ZobristValues zobristValues;

public:
	BoardHash();
	BoardHash(const ChessState* cs);
	~BoardHash();

	friend bool operator<(const BoardHash& a, const BoardHash& b) {
		return a.hash < b.hash;
	}

	void makeHash(const ChessState* cs);
};

#endif
