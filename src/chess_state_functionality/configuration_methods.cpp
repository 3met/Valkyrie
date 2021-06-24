
#include <iostream>
#include <string>
#include <utility>
#include "chess_state.hpp"
#include "U8.hpp"

// Clears the board and resets game data
void ChessState::clear() {
	
	// Reset bitboards
	for (U8 i(0); i<2; ++i) {
		for (U8 j=0; j<6; ++j) {
			pieces[i][j].board = 0;
		}
	}

	// Set universal bitboards
	this->updateAllBitboard(WHITE);
	this->updateAllBitboard(BLACK);

	turn = WHITE;
	
	castlePerms[WHITE][KING_SIDE] = false;	// Castle perms
	castlePerms[WHITE][QUEEN_SIDE] = false;
	castlePerms[BLACK][KING_SIDE] = false;
	castlePerms[BLACK][QUEEN_SIDE] = false;

	moveLostCastlePerms[WHITE][KING_SIDE] = -1;	// For reversing moves
	moveLostCastlePerms[WHITE][QUEEN_SIDE] = -1;
	moveLostCastlePerms[BLACK][KING_SIDE] = -1;
	moveLostCastlePerms[BLACK][QUEEN_SIDE] = -1;

	// Reset en passant data
	enPassant = -1;
	while (!enPassantHistory.empty()) {
		enPassantHistory.pop();
	}
	enPassantHistory.push(enPassant);

	halfmoveClock = 0;		// # of halfmoves since last capture or pawn move
	turnNumber = 1;			// Game turn number
	moveNumber = 1;

	this->bh.makeHash(pieces, turn, castlePerms, enPassant);
}

// Loads FEN encoding into ChessState
void ChessState::loadFEN(string FEN) {
	this->clear();

	U8 i(0);
	pair<bool, U8> piece;
	U8 FEN_index(0);
	// Load board
	while (i < 64) {
		// If number at index
		if (FEN[FEN_index] >= '0' && FEN[FEN_index] <= '8') {
			i += FEN[FEN_index] - '0';
			++FEN_index;
			continue;
		}
		// If '/' at index
		if (FEN[FEN_index] == '/') {
			++FEN_index;
			continue;
		}

		piece = charToPiece(FEN[FEN_index]);
		this->place(piece.first, piece.second, Bitboard::SHOW_ORDER[i]);
		++FEN_index;
		++i;
	}

	// Set universal bitboards
	this->updateAllBitboard(WHITE);
	this->updateAllBitboard(BLACK);

	++FEN_index;	// Skip over space

	// Read turn
	if (FEN[FEN_index] == 'w') {
		this->turn = WHITE;
	} else {
		this->turn = BLACK;
	}

	++FEN_index;	// Skip over space
	++FEN_index;

	// Read castling permissions
	if (FEN[FEN_index] != '-') {
		do {
			switch (FEN[FEN_index]) {
				case 'K':
					this->castlePerms[this->WHITE][KING_SIDE] = true;
					break;
				case 'Q':
					this->castlePerms[this->WHITE][QUEEN_SIDE] = true;
					break;
				case 'k':
					this->castlePerms[this->BLACK][KING_SIDE] = true;
					break;
				case 'q':
					this->castlePerms[this->BLACK][QUEEN_SIDE] = true;
					break;
				default:
					cout << "ERROR: Invalid FEN"
						<< " (\"" << FEN[FEN_index] << "\""
						<< " is an invalid castle character)" << endl;
			}

			++FEN_index;
		} while (FEN[FEN_index] != ' ');
	} else {
		++FEN_index;
	}

	++FEN_index;	// Skip over space

	// Read en passant
	if (FEN[FEN_index] != '-') {
		this->enPassant = Move::coordToPos(FEN.substr(FEN_index, 2));
		enPassantHistory.push(this->enPassant);	
		++FEN_index;	// Move to last en passant character
	}

	// Return if no more to read
	if (FEN_index == FEN.size()-1) {
		halfmoveClock = 0;
		turnNumber = 1;
		this->bh.makeHash(pieces, turn, castlePerms, enPassant);
		return;
	}

	++FEN_index;	// Skip over space
	++FEN_index;

	U8 nLength(1);	// Number digit length (567 ==> 3)
	// Add halfmove count (allows varible number of digits)
	while (true) {
		if (FEN_index != FEN.size()-1 && FEN[FEN_index+1] != ' ') {
			++nLength;
			++FEN_index;
		} else {
			break;
		}
	}

	halfmoveClock = stoi(FEN.substr(FEN_index-(nLength-1), nLength));

	// Return if no more to read
	if (FEN_index == FEN.size()-1) {
		turnNumber = 1;
		this->bh.makeHash(pieces, turn, castlePerms, enPassant);
		return;
	}

	++FEN_index;	// Skip over space
	++FEN_index;

	nLength = 1;
	// Add turn number (allows varible number of digits)
	while (true) {
		if (FEN_index != FEN.size()-1 && FEN[FEN_index+1] != ' ') {
			++nLength;
			++FEN_index;
		} else {
			break;
		}
	}

	turnNumber = stoi(FEN.substr(FEN_index-(nLength-1), nLength));

	this->bh.makeHash(pieces, turn, castlePerms, enPassant);
}

// Place piece on the board
void ChessState::place(bool color, U8 pieceType, U8 pos) {

	pieces[color][pieceType].setPosOn(pos);
	this->updateAllBitboard(color);

	bh.updatePiece(color, pieceType, pos);
}

// Resets chess board to the start position.
void ChessState::reset() {

	// Initial piece setup
	this->loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

	// Set universal bitboards
	this->updateAllBitboard(WHITE);
	this->updateAllBitboard(BLACK);

	turn = WHITE;	// False for white; true for black

	moveLostCastlePerms[WHITE][KING_SIDE] = -1;	// For reversing moves
	moveLostCastlePerms[WHITE][QUEEN_SIDE] = -1;
	moveLostCastlePerms[BLACK][KING_SIDE] = -1;
	moveLostCastlePerms[BLACK][QUEEN_SIDE] = -1;

	enPassant = -1;
	while (!enPassantHistory.empty()) {
		enPassantHistory.pop();
	}
	enPassantHistory.push(enPassant);
	halfmoveClock = 0;		// # of halfmoves since last capture or pawn move
	turnNumber = 1;			// Game turn number
	moveNumber = 1;

	this->bh.makeHash(pieces, turn, castlePerms, enPassant);
}
