
#include "../include/chess_state.hpp"

ChessState::ChessState() {
	this->reset();
};

ChessState::~ChessState() {};

void ChessState::reset() {
	/*	Resets chess board state to as it should
		be at the beginning of a game.	*/

	// Initial piece setup
	wP.setPos(true, 8);	// White pawns
	wP.setPos(true, 9);
	wP.setPos(true, 10);
	wP.setPos(true, 11);
	wP.setPos(true, 12);
	wP.setPos(true, 13);
	wP.setPos(true, 14);
	wP.setPos(true, 15);

	wN.setPos(true, 1);	// White knights
	wN.setPos(true, 6);

	wB.setPos(true, 2);	// White bishops
	wB.setPos(true, 5);

	wR.setPos(true, 0);	// White rooks
	wR.setPos(true, 7);

	wQ.setPos(true, 3);	// White queen and king
	wK.setPos(true, 4);

	bP.setPos(true, 48); // Black pawns
	bP.setPos(true, 49);
	bP.setPos(true, 50);
	bP.setPos(true, 51);
	bP.setPos(true, 52);
	bP.setPos(true, 53);
	bP.setPos(true, 54);
	bP.setPos(true, 55);

	bN.setPos(true, 57);	// Black knights
	bN.setPos(true, 62);

	bB.setPos(true, 58);	// Black bishops
	bB.setPos(true, 61);

	bR.setPos(true, 56);	// Black rooks
	bR.setPos(true, 63);

	bQ.setPos(true, 59);	// Black queen and king
	bK.setPos(true, 60);

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
