/*
 *	Chess Engine
 *	Version 1
 *	By Emet Behrendt
 *	
 *	w/b ==> white/black
 *	Capital/lowercase ==> White/black
 *	
 *	P/p ==> Pawn
 *	N/n ==> Knight
 *	B/b ==> Bishop
 *	R/r ==> Rook
 *	Q/q ==> Queen
 *	K/k ==> King
 *	
 */

/*
For Consideration:
 - For rook move generation, remove non-horizontal pieces from bitboard before calculation
 - Add engine settings class for things like material scoring system
 - Pawn structure rating lookup
 - End search function early

 - Name: Coronation Chess?

*/

#include <iostream>
#include "testing.hpp"
#include "U64.hpp"
#include "uci.hpp"

using namespace std;

int main() {
	cout << "Chess Engine | By Emet Behrendt" << endl;

	// Bitboard size check
	if (sizeof(U64) != 8) {
		cout << "ERROR: Bitboard has incorrect number of bits" << endl;
		return 1;
	}

	UCI uci;
	uci.run();
	
	return 0;
}

