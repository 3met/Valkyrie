
#include <iostream>
#include <string>
#include <utility>
#include "chess_state.hpp"
#include "U8.hpp"

ChessState::ChessState() {
	this->reset();
};

ChessState::ChessState(const ChessState* cs) {
	wP.board = cs->wP.board;
	wN.board = cs->wN.board;
	wB.board = cs->wB.board;
	wR.board = cs->wR.board;
	wQ.board = cs->wQ.board;
	wK.board = cs->wK.board;
	wAll.board = cs->wAll.board;

	bP.board = cs->bP.board;
	bN.board = cs->bN.board;
	bB.board = cs->bB.board;
	bR.board = cs->bR.board;
	bQ.board = cs->bQ.board;
	bK.board = cs->bK.board;
	bAll.board = cs->bAll.board;

	turn = cs->turn;
	
	castlePerms[WHITE][KING_SIDE] = cs->castlePerms[WHITE][KING_SIDE];	// Castle perms
	castlePerms[WHITE][QUEEN_SIDE] = cs->castlePerms[WHITE][QUEEN_SIDE];
	castlePerms[BLACK][KING_SIDE] = cs->castlePerms[BLACK][KING_SIDE];
	castlePerms[BLACK][QUEEN_SIDE] = cs->castlePerms[BLACK][QUEEN_SIDE];
	
	halfmoveClock = cs->halfmoveClock;
	turnNumber = cs->turnNumber;
	moveNumber = cs->moveNumber;
}

ChessState::~ChessState() {};

const char ChessState::piece_names[2][6] = {	// Note: must match piece indexing
	{'P', 'N', 'B', 'R', 'Q', 'K'},
	{'p', 'n', 'b', 'r', 'q', 'k'},
};

const U8 ChessState::KING_START[2] = {4, 60};	// {white, black}

const U8 ChessState::ROOK_START[2][2] = {{7, 0}, {63, 56}};	// {{WK, WQ}, {BK, BQ}}

// |~| ----- Query Method -----
S8 ChessState::getPieceType(bool colour, U8 pos) {
	/* Returns the type of piece at the given position */

	for (U8 i=0; i<6; ++i) {
		if (pieces[colour][i]->getPos(pos)) {
			return i;
		}
	}

	return -1;
}

// Returns most recent move
Move ChessState::lastMove() {
	return *(moveList.end()-1);
}


// |~| ----- SETUP METHODS -----
void ChessState::reset() {
	/*	Resets chess board state to as it should
		be at the beginning of a game.	*/

	// Initial piece setup
	this->loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

	// Set universal bitboards
	this->updateAllBitboard(WHITE);
	this->updateAllBitboard(BLACK);

	turn = false;	// False for white; true for black

	turnLostCastlePerms[WHITE][KING_SIDE] = -1;	// For reversing moves
	turnLostCastlePerms[WHITE][QUEEN_SIDE] = -1;
	turnLostCastlePerms[BLACK][KING_SIDE] = -1;
	turnLostCastlePerms[BLACK][QUEEN_SIDE] = -1;

	enPassant = -1;
	halfmoveClock = 0;	// # of halfmoves since last capture or pawn move
	turnNumber = 1;	// Game turn number
	moveNumber = 1;
}

/* Clears the board and resets game data */
void ChessState::clear() {
	
	// Reset bitboards
	for (U8 i=0; i<2; ++i) {
		for (U8 j=0; j<6; ++j) {
			pieces[i][j]->board = 0;
		}
	}

	// Set universal bitboards
	this->updateAllBitboard(WHITE);
	this->updateAllBitboard(BLACK);

	turn = WHITE;
	
	this->castlePerms[this->WHITE][this->KING_SIDE] = false;	// Castle perms
	this->castlePerms[this->WHITE][this->QUEEN_SIDE] = false;
	this->castlePerms[this->BLACK][this->KING_SIDE] = false;
	this->castlePerms[this->BLACK][this->QUEEN_SIDE] = false;

	turnLostCastlePerms[WHITE][KING_SIDE] = -1;	// For reversing moves
	turnLostCastlePerms[WHITE][QUEEN_SIDE] = -1;
	turnLostCastlePerms[BLACK][KING_SIDE] = -1;
	turnLostCastlePerms[BLACK][QUEEN_SIDE] = -1;

	enPassant = -1;
	enPassantHistory.clear();
	halfmoveClock = 0;	// # of halfmoves since last capture or pawn move
	turnNumber = 1;	// Game turn number
	moveNumber = 1;
}

void ChessState::place(short colour, short piece, short pos) {
	/* Place piece on the board */

	pieces[colour][piece]->setPos(pos, true);
	this->updateAllBitboard(colour);
}

pair<bool, U8> ChessState::charToPiece(char piece) {
	/* Returns colour and types of piece */
	switch (piece) {
		case 'P':
			return make_pair(WHITE, PAWN);
		case 'p':
			return make_pair(BLACK, PAWN);
		case 'N':
			return make_pair(WHITE, KNIGHT);
		case 'n':
			return make_pair(BLACK, KNIGHT);
		case 'B':
			return make_pair(WHITE, BISHOP);
		case 'b':
			return make_pair(BLACK, BISHOP);
		case 'R':
			return make_pair(WHITE, ROOK);
		case 'r':
			return make_pair(BLACK, ROOK);
		case 'Q':
			return make_pair(WHITE, QUEEN);
		case 'q':
			return make_pair(BLACK, QUEEN);
		case 'K':
			return make_pair(WHITE, KING);
		case 'k':
			return make_pair(BLACK, KING);
		default:
			cout << "ERROR: \"" << piece << "\" is an invalid character" << endl;
			return make_pair(WHITE, -1);
	}
}

void ChessState::loadFEN(string FEN) {
	/* Loads FEN encoding into chess state */
	this->clear();

	U8 i=0;
	pair<bool, U8> piece;
	U8 FEN_index = 0;
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
						<< " is an castle character)" << endl;
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
		enPassantHistory.push_back(this->enPassant);	
		++FEN_index;	// Move to last en passant character
	}

	// Return if no more to read
	if (FEN_index == FEN.size()-1) {
		return;
	}

	++FEN_index;	// Skip over space
	++FEN_index;

	U8 nLength = 1;	// Number digit length (567 ==> 3)
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
}

string ChessState::stringFEN() {
	string FEN;
	char board[64];
	U8 i, j;

	for (i=0; i<64; ++i) {
		board[i] = '.';
	}

	for (i=0; i<6; ++i) {
		this->mapBoardToChar(*pieces[0][i], board, piece_names[0][i]);
		this->mapBoardToChar(*pieces[1][i], board, piece_names[1][i]);
	}

	int gap = 0;
	for (i=0; i<64; ++i) {
		if (board[Bitboard::SHOW_ORDER[i]] == '.') {
			gap += 1;
		} else {
			if (gap > 0) {
				FEN += to_string(gap);
				gap = 0;
			}
			FEN += board[Bitboard::SHOW_ORDER[i]];
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

	if (this->turn == this->WHITE) {
		FEN += 'w';
	} else {
		FEN += 'b';
	}

	FEN += ' ';

	if (castlePerms[WHITE][KING_SIDE]) {
		FEN += 'K';
	}
	if (castlePerms[WHITE][QUEEN_SIDE]) {
		FEN += 'Q';
	}
	if (castlePerms[BLACK][KING_SIDE]) {
		FEN += 'k';
	}
	if (castlePerms[BLACK][QUEEN_SIDE]) {
		FEN += 'q';
	}

	FEN += ' ';

	if (enPassant == -1) {
		FEN += '-';
	} else {
		FEN += Move::posToCoord(enPassant);
	}

	FEN += ' ';
	FEN += to_string(halfmoveClock);
	FEN += ' ';
	FEN += to_string(turnNumber);

	return FEN;
}

Move ChessState::notationToMove(string notation) {
	// Converts chess notation to Move object (a7b8q ==> Move)
	U8 start = Move::coordToPos(notation.substr(0, 2));
	U8 end = Move::coordToPos(notation.substr(2, 2));
	U8 pieceType = getPieceType(turn, start);
	U8 killed;
	// Check for en passant killing
	if (end == enPassant && pieceType == PAWN) {
		killed = PAWN;
	} else {
		killed = getPieceType(!turn, end);
	}
	S8 promoted;
	// Check for pawn promotion
	if (pieceType == PAWN && (end >= 56 || end <= 7)) {
		if (notation.size() >= 5) {
			promoted = charToPiece(notation[4]).second;
		} else {
			promoted = QUEEN;
		}
	} else {
		promoted = -1;
	}
	
	return Move(pieceType,
				start,
				end,
				killed,
				promoted);
}

/* ----- Playing Functions ----- */
void ChessState::move(Move m) {
	/* 	Moves a piece on the board.
		Assumes move is valid.	*/

	moveList.push_back(m);

	U8 i;

	// Removes potential killed piece from bitboard
	if (m.killed != -1) {
		if (m.end == enPassant) {
			// Remove piece killed by en passant
			if (turn == WHITE) {
				pieces[BLACK][PAWN]->setPos(m.end-8, false);
			} else {	// Black's turn
				pieces[WHITE][PAWN]->setPos(m.end+8, false);
			}
		} else {
			pieces[!turn][m.killed]->setPos(m.end, false);
		}
	}

	// Updates moving piece location on bitboard
	pieces[turn][m.piece]->setPos(m.start, false);
	if (m.promoted == -1) {
		pieces[turn][m.piece]->setPos(m.end, true);
	} else {
		pieces[turn][m.promoted]->setPos(m.end, true);
	}

	// Account for castling in king movement
	if (m.piece == KING) {
		// Update castle permissions
		if (castlePerms[turn][KING_SIDE]) {
			castlePerms[turn][KING_SIDE] = false;
			turnLostCastlePerms[turn][KING_SIDE] = moveNumber;
		}
		if (castlePerms[turn][QUEEN_SIDE]) {
			castlePerms[turn][QUEEN_SIDE] = false;
			turnLostCastlePerms[turn][QUEEN_SIDE] = moveNumber;
		}

		// Update rook positions if castled
		if (m.start == KING_START[turn]) {
			// If king side castled
			if (m.end == KING_START[turn]+2) {
				pieces[turn][ROOK]->setPos(KING_START[turn]+3, false);
				pieces[turn][ROOK]->setPos(KING_START[turn]+1, true);
			// If queen side castled
			} else if (m.end == KING_START[turn]-2) {
				pieces[turn][ROOK]->setPos(KING_START[turn]-4, false);
				pieces[turn][ROOK]->setPos(KING_START[turn]-1, true);
			}
		}
	}

	// Account for castling in rook movement
	// Removing relevant castling perms if rook moves
	if (m.piece == ROOK) {
		// King's side
		if (m.start == ROOK_START[turn][KING_SIDE]
			&& castlePerms[turn][KING_SIDE]) {
			
			castlePerms[turn][KING_SIDE] = false;
			turnLostCastlePerms[turn][KING_SIDE] = moveNumber;
		// Queen's side
		} else if (ROOK_START[turn][KING_SIDE]
			&& castlePerms[turn][QUEEN_SIDE]) {
			
			castlePerms[turn][QUEEN_SIDE] = false;
			turnLostCastlePerms[turn][QUEEN_SIDE] = moveNumber;
		}
	}

	// Account for castling in rook death
	if (m.killed == ROOK) {
		if (castlePerms[!turn][KING_SIDE] && m.end == ROOK_START[!turn][KING_SIDE]) {
			castlePerms[!turn][KING_SIDE] = false;
			turnLostCastlePerms[!turn][KING_SIDE] = moveNumber;	
		} else if (castlePerms[!turn][QUEEN_SIDE] && m.end == ROOK_START[!turn][QUEEN_SIDE]) {
			castlePerms[!turn][QUEEN_SIDE] = false;
			turnLostCastlePerms[!turn][QUEEN_SIDE] = moveNumber;	
		}
	}

	// Update enPassant value
	if (m.piece == PAWN) {
		if (turn == WHITE) {
			// If pawn moved two squares forward
			if (Bitboard::RANK[m.start] == 1 && Bitboard::RANK[m.end] == 3) {
				enPassant = m.end - 8;
			} else {
				enPassant = -1;
			}
		} else {	// If black's turn
			if (Bitboard::RANK[m.start] == 6 && Bitboard::RANK[m.end] == 4) {
				enPassant = m.end + 8;
			} else {
				enPassant = -1;
			}
		}
	} else {
		enPassant = -1;
	}
	enPassantHistory.push_back(enPassant);

	// Update "all" bitboard
	this->updateAllBitboard(turn);
	// Update other color if a piece was killed
	if (m.killed != -1) {
		this->updateAllBitboard(!turn);
	}	

	// Updates move count
	if (turn == BLACK) {	// If black completed turn
		turnNumber += 1;
	}
	moveNumber += 1;

	// Update turn color
	turn = !turn;
}

void ChessState::reverseMove() {
	/*	Reverses a moves.
		Assumes move is valid.	*/

	Move* m = &(*(moveList.end()-1));

	turn = !turn;	// Swaps turn

	if (turn == BLACK) {
		turnNumber -= 1;
	}
	moveNumber -= 1;

	// Revert en passant value
	enPassantHistory.pop_back();
	enPassant = *(enPassantHistory.end()-1);

	// Updates piece location on bitboard
	if (m->promoted == -1) {
		pieces[turn][m->piece]->setPos(m->end, false);
	} else {
		pieces[turn][m->promoted]->setPos(m->end, false);
	}
	pieces[turn][m->piece]->setPos(m->start, true);

	// Adds previously killed piece to bitboard
	if (m->killed != -1) {
		if (m->end == enPassant) {
			// Place killed en passant piece
			if (turn == WHITE) {
				pieces[BLACK][PAWN]->setPos(m->end-8, true);
			} else {	// Black's turn
				pieces[WHITE][PAWN]->setPos(m->end+8, true);
			}
		} else {
			pieces[!turn][m->killed]->setPos(m->end, true);
		}
	}

	// Reverse rook movement in castling
	if (m->piece == KING) {
		if (m->start == KING_START[turn]) {
			if (m->end == KING_START[turn]+2) {
				pieces[turn][ROOK]->setPos(KING_START[turn]+3, true);
				pieces[turn][ROOK]->setPos(KING_START[turn]+1, false);
			} else if (m->end == KING_START[turn]-2) {
				pieces[turn][ROOK]->setPos(KING_START[turn]-4, true);
				pieces[turn][ROOK]->setPos(KING_START[turn]-1, false);
			}
		}
	}

	// Update castle permissions
	if (turnLostCastlePerms[turn][KING_SIDE] == moveNumber) {
		castlePerms[turn][KING_SIDE] = true;
		turnLostCastlePerms[turn][KING_SIDE] = -1;
	}
	if (turnLostCastlePerms[turn][QUEEN_SIDE] == moveNumber) {
		castlePerms[turn][QUEEN_SIDE] = true;
		turnLostCastlePerms[turn][QUEEN_SIDE] = -1;
	}
	// One can lose castle perms regardless of turn
	if (turnLostCastlePerms[!turn][KING_SIDE] == moveNumber) {
		castlePerms[!turn][KING_SIDE] = true;
		turnLostCastlePerms[!turn][KING_SIDE] = -1;
	}
	if (turnLostCastlePerms[!turn][QUEEN_SIDE] == moveNumber) {
		castlePerms[!turn][QUEEN_SIDE] = true;
		turnLostCastlePerms[!turn][QUEEN_SIDE] = -1;
	}

	// Remove reversed move from move list
	moveList.pop_back();

	// Update both universal bitboards
	this->updateAllBitboard(turn);
	this->updateAllBitboard(!turn);
}

/* ----- Update State Functions ----- */
void ChessState::updateAllBitboard(bool colour) {
	if (colour == BLACK) {
		bAll.board = (bP.board | bN.board | bB.board | bR.board | bQ.board | bK.board);
	} else {
		wAll.board = (wP.board | wN.board | wB.board | wR.board | wQ.board | wK.board);
	}
}

/* ----- Output Functions -----*/
void ChessState::mapBoardToChar(Bitboard b, char arr[64], char target) {
	vector<U8> v = b.getPosVector();
	for (U8 i=0; i<v.size(); ++i) {
		arr[v[i]] = target;
	}
}

void ChessState::show() {
	this->show(true);
}

void ChessState::show(bool show_coords) {
	char board[64];
	char ranks[8] = {'8', '7', '6', '5', '4', '3', '2', '1'};
	U8 i;

	for (i=0; i<64; ++i) {
		board[i] = '.';
	}

	for (i=0; i<6; ++i) {
		this->mapBoardToChar(*pieces[0][i], board, piece_names[0][i]);
		this->mapBoardToChar(*pieces[1][i], board, piece_names[1][i]);
	}

	// TODO: ADD COORDS around board
	if (show_coords) {
		cout << "-------------------" << endl;
	}
	for (i=0; i<64; ++i) {
		if (show_coords && (i+1) % 8 == 1) {
			cout << ranks[i/8] << " | ";
		}		

		cout << board[Bitboard::SHOW_ORDER[i]] << ' ';

		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}

	if (show_coords) {
		cout << "  +----------------" << endl;
		cout << "    a b c d e f g h" << endl;
		cout << "FEN: " << this->stringFEN() << endl;
		cout << "-------------------" << endl;
	}
}
