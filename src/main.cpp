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

#include <iostream>
#include "size_defs.hpp"
#include "testing.hpp"
#include "uci.hpp"

using namespace std;

int main() {
	cout << "Chess Engine | By Emet Behrendt" << endl;

	UCI uci;
	uci.run();
	
	return 0;
}
