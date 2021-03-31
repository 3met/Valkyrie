
#include <iostream>
#include <string>
#include "../include/chess_state.hpp"

ChessState::ChessState() {
	pieces[0][0] = &wP;
	pieces[0][1] = &wN;
	pieces[0][2] = &wB;
	pieces[0][3] = &wR;
	pieces[0][4] = &wQ;
	pieces[0][5] = &wK;
	pieces[0][6] = &wAll;

	pieces[1][0] = &bP;
	pieces[1][1] = &bN;
	pieces[1][2] = &bB;
	pieces[1][3] = &bR;
	pieces[1][4] = &bQ;
	pieces[1][5] = &bK;
	pieces[1][6] = &bAll;

	this->reset();
};

ChessState::~ChessState() {};

const char ChessState::piece_names[2][6] = {	// Note: must match piece indexing
	{'P', 'N', 'B', 'R', 'Q', 'K'},
	{'p', 'n', 'b', 'r', 'q', 'k'},
};

short ChessState::getPieceType(bool colour, short pos) {
	/* Returns the type of piece at the given position */

	for (short i=0; i<6; ++i) {
		if (pieces[colour][i]->getPos(pos)) {
			return i;
		}
	}

	//cout << "Warning: No piece found." << endl;
	return -1;
}

// |~| ----- SETUP METHODS -----
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

	//wB.setPos(2, true);	// White bishops
	//wB.setPos(5, true);

	//wR.setPos(0, true);	// White rooks
	//wR.setPos(7, true);

	//wQ.setPos(3, true);	// White queen and king
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

	//bB.setPos(58, true);	// Black bishops
	//bB.setPos(61, true);

	//bR.setPos(56, true);	// Black rooks
	//bR.setPos(63, true);

	//bQ.setPos(59, true);	// Black queen and king
	bK.setPos(60, true);

	// Set universal bitboards
	this->updateAllBitboard(white);
	this->updateAllBitboard(black);

	turn = false;	// False for white; true for black
	
	wKCastle = true;	// Castle perms
	wQCcastle = true;
	bKCastle = true;
	bQCastle = true;

	en_passant[0] = '-';	// Square behind pawn else "-"
	en_passant[1] = '\0';	// Square behind pawn else "\0"
	halfmoveClock = 0;	// # of halfmoves since last capture or pawn move
	turnNumber = 1;	// Game turn number
}

void ChessState::clear() {
	/* Clears the board and resets game data */

	for (short i=0; i<2; ++i) {
		for (short j=0; j<6; ++j) {
			pieces[i][j]->board = 0;
		}
	}

	// Set universal bitboards
	this->updateAllBitboard(white);
	this->updateAllBitboard(black);

	turn = false;	// False for white; true for black
	
	wKCastle = true;	// Castle perms
	wQCcastle = true;
	bKCastle = true;
	bQCastle = true;

	en_passant[0] = '-';	// Square behind pawn else "-"
	en_passant[1] = '\0';	// Square behind pawn else "\0"
	halfmoveClock = 0;	// # of halfmoves since last capture or pawn move
	turnNumber = 1;	// Game turn number
}

void ChessState::place(short colour, short piece, short pos) {
	/* Place piece on the board */

	pieces[colour][piece]->setPos(pos, true);
	this->updateAllBitboard(colour);
}

void ChessState::loadFEN(string FEN) {
	/* Loads FEN econding into chess state */

}


/* ----- Playing Functions ----- */
void ChessState::move(Move m) {
	/* 	Moves a piece on the board.
		Assumes move is valid.	*/

	short i;

	// Removes potential killed piece from bitboard
	if (m.killed != -1) {
		for (i=0; i<6; ++i) {
			if (pieces[!turn][i]->getPos(m.end)) {
				pieces[!turn][i]->setPos(m.end, false);
				break;
			}
		}
	}

	// Updates piece location on bitboard
	pieces[turn][m.piece]->setPos(m.start, false);
	pieces[turn][m.piece]->setPos(m.end, true);
	/*
	if (m.start == 61) {
		cout << "61 SHOULD BE FALSE!!!!!!!!!!!!!!!" << endl;
		cout << "Piece: " << m.piece << endl;
		cout << "Turn: " << turn << endl;
		pieces[turn][m.piece]->show();
		cout << endl;
	}*/

	this->updateAllBitboard(turn);

	// Update other side if a piece was killed
	if (m.killed != -1) {
		this->updateAllBitboard(!turn);
	}	

	if (turn) {	// If black completed turn
		turnNumber += 1;
	}

	turn = !turn;
}

void ChessState::reverseMove(Move m) {
	/* 	Reverses a moves.
		Assumes move is valid.	*/

	short i;

	turn = !turn;	// Swaps turn

	if (!turn) {	// If reversing white turn
		turnNumber -= 1;
	}

	// Updates piece location on bitboard
	pieces[turn][m.piece]->setPos(m.end, false);
	pieces[turn][m.piece]->setPos(m.start, true);

	// Adds previously killed piece to bitboard
	if (m.killed != -1) {
		pieces[!turn][m.killed]->setPos(m.end, true);
	}

	// Update both universal bitboards
	this->updateAllBitboard(turn);
	this->updateAllBitboard(!turn);
};

/* ----- Update State Functions ----- */
void ChessState::updateAllBitboard(bool colour) {
	if (colour) {
		//cout << "updated b" << endl;
		bAll.board = (bP.board | bN.board | bB.board | bR.board | bQ.board | bK.board);
	} else {
		//cout << "updated w" << endl;
		wAll.board = (wP.board | wN.board | wB.board | wR.board | wQ.board | wK.board);
	}
}

/* ----- Output Functions -----*/
void ChessState::mapBoardToChar(Bitboard b, char arr[64], char target) {
	vector v = b.getPosVector();
	for (short i=0; i<v.size(); ++i) {
		arr[v[i]] = target;
	}
}

void ChessState::show() {
	this->show(true);
}

void ChessState::show(bool show_coords) {
	char board[64];
	short i;

	for (i=0; i<64; ++i) {
		board[i] = '.';
	}

	for (i=0; i<6; ++i) {
		this->mapBoardToChar(*pieces[0][i], board, piece_names[0][i]);
		this->mapBoardToChar(*pieces[1][i], board, piece_names[1][i]);
	}

	// TODO: ADD COORDS around board

	cout << "---------------" << endl;
	for (int i=0; i<64; ++i) {
		cout << board[Bitboard::show_order[i]] << ' ';

		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}
	cout << "---------------" << endl;
}
