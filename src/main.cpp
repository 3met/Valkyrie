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

#include <stdio.h>
#include "uci.hpp"

using namespace std;

int main() {
	printf("Chess Engine | By Emet Behrendt\n");

	UCI uci;
	uci.run();
	
	return 0;
}
