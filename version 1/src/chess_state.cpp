
#include <iostream>
#include "../include/chess_state.hpp"

ChessState::ChessState() {
	wBitboards.push_back(&wP);
	wBitboards.push_back(&wN);
	wBitboards.push_back(&wB);
	wBitboards.push_back(&wR);
	wBitboards.push_back(&wQ);
	wBitboards.push_back(&wK);

	bBitboards.push_back(&bP);
	bBitboards.push_back(&bN);
	bBitboards.push_back(&bB);
	bBitboards.push_back(&bR);
	bBitboards.push_back(&bQ);
	bBitboards.push_back(&bK);

	this->reset();
};

ChessState::~ChessState() {};

void ChessState::reset() {
	/*	Resets chess board state to as it should
		be at the beginning of a game.	*/

	// Initial piece setup
	wP.setPos(8, true);	// White pawns
	wP.setPos(9, true);
	wP.setPos(10, true);
	wP.setPos(11, true);
	wP.setPos(12, true);
	wP.setPos(13, true);
	wP.setPos(14, true);
	wP.setPos(15, true);

	wN.setPos(1, true);	// White knights
	wN.setPos(6, true);

	wB.setPos(2, true);	// White bishops
	wB.setPos(5, true);

	wR.setPos(0, true);	// White rooks
	wR.setPos(7, true);

	wQ.setPos(3, true);	// White queen and king
	wK.setPos(4, true);

	bP.setPos(48, true); // Black pawns
	bP.setPos(49, true);
	bP.setPos(50, true);
	bP.setPos(51, true);
	bP.setPos(52, true);
	bP.setPos(53, true);
	bP.setPos(54, true);
	bP.setPos(55, true);

	bN.setPos(57, true);	// Black knights
	bN.setPos(62, true);

	bB.setPos(58, true);	// Black bishops
	bB.setPos(61, true);

	bR.setPos(56, true);	// Black rooks
	bR.setPos(63, true);

	bQ.setPos(59, true);	// Black queen and king
	bK.setPos(60, true);

	// Set universal bitboards
	wAll.board = 0;
	bAll.board = 0;
	for (short i=0; i<wBitboards.size(); ++i) {
		wAll.board |= wBitboards[i]->board;
		bAll.board |= bBitboards[i]->board;
	}

	turn = true;	// True for white; false for black
	
	wKCastle = true;	// Castle perms
	wQCcastle = true;
	bKCastle = true;
	bQCastle = true;

	en_passant[0] = '-';	// Square behind pawn else "-"
	en_passant[1] = '\0';	// Square behind pawn else "-"
	halfmoveClock = 0;	// # of halfmoves since last capture or pawn move
	moveNumber = 1;	// Game turn number
}

/* ----- Play Functions ----- */
void ChessState::move(short start, short end) {
	/* 	Moves a piece on the board.
		Assumes move is valid.	*/

	if (turn) {	// If white to play
		for (short i=0; i<wBitboards.size(); ++i){
			if (wBitboards[i]->getPos(start)) {
				wBitboards[i]->setPos(start, false);
				wBitboards[i]->setPos(end, true);
				turn = false;
				break;
			}
		}
	} else {	// Black to play
		for (short i=0; i<bBitboards.size(); ++i){
			if (bBitboards[i]->getPos(start)) {
				bBitboards[i]->setPos(start, false);
				bBitboards[i]->setPos(end, true);
				turn = true;
				break;
			}
		}
	}
}

/* ----- Output Functions -----*/
void ChessState::mapBoardToChar(Bitboard b, char arr[65], char target) {
	vector v = b.getPosVector();
	for (short i=0; i<v.size(); ++i) {
		arr[v[i]] = target;
	}
}

void ChessState::show() {
	this->show(true);
}

void ChessState::show(bool show_coords) {
	char board[65];
	short i;

	for (i=0; i<65; ++i) {
		board[i] = '.';
	}

	this->mapBoardToChar(wP, board, 'P');
	this->mapBoardToChar(wN, board, 'N');
	this->mapBoardToChar(wB, board, 'B');
	this->mapBoardToChar(wR, board, 'R');
	this->mapBoardToChar(wQ, board, 'Q');
	this->mapBoardToChar(wK, board, 'K');

	this->mapBoardToChar(bP, board, 'p');
	this->mapBoardToChar(bN, board, 'n');
	this->mapBoardToChar(bB, board, 'b');
	this->mapBoardToChar(bR, board, 'r');
	this->mapBoardToChar(bQ, board, 'q');
	this->mapBoardToChar(bK, board, 'k');

	// TODO: ADD COORDS

	cout << "---------------" << endl;
	for (int i=0; i<64; ++i) {
		cout << board[Bitboard::show_order[i]] << ' ';

		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}
	cout << "---------------" << endl;
}
