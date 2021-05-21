
#pragma once
#ifndef ZOBRIST_VALUES_HPP
#define ZOBRIST_VALUES_HPP

#include "U64.hpp"

/* Class to store Zobrish Hash values */
class ZobristValues {
public:
	ZobristValues();
	~ZobristValues();

	// Pseudorandom numbers for generating Zobrist Hash
	U64 pieces[2][6][64];
	U64 whiteToMove;
	U64 castlePerms[2][2];	// Castling Permissions [color][king/queen side]
	U64 enPassant[8];	// En Passant file
};

#endif
