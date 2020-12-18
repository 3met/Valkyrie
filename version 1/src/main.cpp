
#include <iostream>
#include "..\include\chess_state.hpp"
#include "..\include\bitboard.hpp"

using namespace std;

int main() {
	cout << "start" << endl;

	// Bitboard size check
	if (sizeof(unsigned long long int) != 8) {
		cout << "Warning: Bitboard has incorrect number of bits" << endl;
	}

	ChessState chessState;

	return 0;
}
