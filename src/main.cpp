/*
 *	Valkyrie
 *	Version 1
 *	By Emet Behrendt
 *	
 *	w/b ==> white/black
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
	// Turn off output buffering
	setbuf(stdout, NULL);
	setvbuf(stdout, NULL, _IONBF, 0);
	fflush(NULL);

	printf("Valkyrie | By Emet Behrendt\n");

	UCI uci;
	uci.run();
	
	return 0;
}
