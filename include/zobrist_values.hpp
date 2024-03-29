
#pragma once
#ifndef ZOBRIST_VALUES_HPP
#define ZOBRIST_VALUES_HPP

#include "size_defs.hpp"

// Stores a set of Zobrish hash values.
// Values can be used to hash the state of a 
// chess board into a 64-bit number. 
class ZobristValues {
public:
	ZobristValues();
	~ZobristValues();

	// Pseudorandom numbers for generating Zobrist Hash
	U64 pieces[2][6][64];
	U64 whiteToMove;
	U64 castlePerms[2][2];	// Castling Permissions [color][king/queen side]
	U64 enPassant[8];		// En Passant file
};

#endif
