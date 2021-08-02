
#include <iostream>
#include <stdio.h>
#include <string>
#include "board_defs.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

// Returns a string containing the FEN of the current chess state.
string ChessState::stringFEN() {
	string FEN;
	char board[64];
	U8 i;

	for (i=0; i<64; ++i) {
		board[i] = '.';
	}

	for (i=0; i<6; ++i) {
		this->mapBoardToChar(pieces[0][i], board, PIECE_NAMES[0][i]);
		this->mapBoardToChar(pieces[1][i], board, PIECE_NAMES[1][i]);
	}

	U8 gap(0);
	for (i=0; i<64; ++i) {
		if (board[SHOW_ORDER[i]] == '.') {
			gap += 1;
		} else {
			if (gap > 0) {
				FEN += to_string(gap);
				gap = 0;
			}
			FEN += board[SHOW_ORDER[i]];
		}

		if (i != 0 && (i+1) % 8 == 0) {	
			if (gap > 0) {
				FEN += to_string(gap);
				gap = 0;
			}

			if (i != 63) {
				FEN += '/';
			}
		}
	}

	FEN += ' ';

	if (this->turn == WHITE) {
		FEN += 'w';
	} else {
		FEN += 'b';
	}

	FEN += ' ';

	bool addDash(true);
	if (castlePerms[WHITE][KING_SIDE]) {
		FEN += 'K';
		addDash = false;
	}
	if (castlePerms[WHITE][QUEEN_SIDE]) {
		FEN += 'Q';
		addDash = false;
	}
	if (castlePerms[BLACK][KING_SIDE]) {
		FEN += 'k';
		addDash = false;
	}
	if (castlePerms[BLACK][QUEEN_SIDE]) {
		FEN += 'q';
		addDash = false;
	}
	if (addDash) {
		FEN += '-';
	}

	FEN += ' ';

	if (enPassantHistory[moveNumber-1] == -1) {
		FEN += '-';
	} else {
		FEN += BaseMove::posToCoord(enPassantHistory[moveNumber-1]);
	}

	FEN += ' ';
	FEN += to_string(halfmoveClock);
	FEN += ' ';
	FEN += to_string(turnNumber);

	return FEN;
}

// Pastes target on array where bitboard is true
void ChessState::mapBoardToChar(Bitboard b, char arr[64], char target) {
	U8 posArr[9];
	U8 posArrSize;
	posArrSize = b.popPosArr(posArr);
	for (U8 i(0); i<posArrSize; ++i) {
		arr[posArr[i]] = target;
	}
}

// Prints the chess board and some related data.
void ChessState::show(bool show_coords) {
	char board[64];
	char ranks[8] = {'8', '7', '6', '5', '4', '3', '2', '1'};
	U8 i;

	for (i=0; i<64; ++i) {
		board[i] = '.';
	}

	// Add pieces to array
	for (i=0; i<6; ++i) {
		this->mapBoardToChar(pieces[0][i], board, PIECE_NAMES[0][i]);
		this->mapBoardToChar(pieces[1][i], board, PIECE_NAMES[1][i]);
	}

	if (show_coords) {
		printf("-------------------\n");
	}
	for (i=0; i<64; ++i) {
		if (show_coords && (i+1) % 8 == 1) {
			printf("%c | ", ranks[i/8]);
		}

		printf("%c ", board[SHOW_ORDER[i]]);

		if ((i+1) % 8 == 0) {
			printf("\n");
		}
	}

	if (show_coords) {
		printf("  +----------------\n");
		printf("    a b c d e f g h\n");
		cout << "FEN: " <<  this->stringFEN() << '\n';
		cout << "BoardHash: " << this->bh.hash << '\n';
		printf("-------------------\n");
	}
}

// Show coordinates by default
void ChessState::show() {
	this->show(true);
}
