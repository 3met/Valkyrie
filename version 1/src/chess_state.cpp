
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
	
	wKCastle = cs->wKCastle;	// Castle perms
	wQCastle = cs->wQCastle;
	bKCastle = cs->bKCastle;
	bQCastle = cs->bQCastle;
	
	halfmoveClock = cs->halfmoveClock;
	turnNumber = cs->turnNumber;
}

ChessState::~ChessState() {};

const char ChessState::piece_names[2][6] = {	// Note: must match piece indexing
	{'P', 'N', 'B', 'R', 'Q', 'K'},
	{'p', 'n', 'b', 'r', 'q', 'k'},
};

S8 ChessState::getPieceType(bool colour, U8 pos) {
	/* Returns the type of piece at the given position */

	for (U8 i=0; i<6; ++i) {
		if (pieces[colour][i]->getPos(pos)) {
			return i;
		}
	}

	return -1;
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
	
	wKCastle = true;	// Castle perms
	wQCastle = true;
	bKCastle = true;
	bQCastle = true;

	turnLostCastlePerms[WHITE][KING_SIDE] = -1;	// For reversing moves
	turnLostCastlePerms[WHITE][QUEEN_SIDE] = -1;
	turnLostCastlePerms[BLACK][KING_SIDE] = -1;
	turnLostCastlePerms[BLACK][QUEEN_SIDE] = -1;

	enPassant = -1;
	halfmoveClock = 0;	// # of halfmoves since last capture or pawn move
	turnNumber = 1;	// Game turn number
}

void ChessState::clear() {
	/* Clears the board and resets game data */

	for (U8 i=0; i<2; ++i) {
		for (U8 j=0; j<6; ++j) {
			pieces[i][j]->board = 0;
		}
	}

	// Set universal bitboards
	this->updateAllBitboard(WHITE);
	this->updateAllBitboard(BLACK);

	turn = false;	// False for white; true for black
	
	wKCastle = false;	// Castle perms
	wQCastle = false;
	bKCastle = false;
	bQCastle = false;

	enPassant = -1;
	halfmoveClock = 0;	// # of halfmoves since last capture or pawn move
	turnNumber = 1;	// Game turn number
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
					wKCastle = true;
					break;
				case 'Q':
					wQCastle = true;
					break;
				case 'k':
					bKCastle = true;
					break;
				case 'q':
					bQCastle = true;
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

	if (FEN[FEN_index] != '-') {
		this->enPassant = Move::coordToPos(FEN.substr(FEN_index, 2));
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


Move ChessState::notationToMove(string notation) {
	// Converts chess notation to Move object (a7b8q ==> Move)
	U8 start = Move::coordToPos(notation.substr(0, 2));
	U8 end = Move::coordToPos(notation.substr(2, 2));
	U8 pieceType = getPieceType(turn, start);
	S8 promoted;
	// Check for pawn promotion
	if (pieceType == PAWN && (end >= 56 || end <= 7)) {
		promoted = QUEEN;
	} else {
		promoted = -1;
	}
	return Move(pieceType,
				start,
				end,
				getPieceType(!turn, end),
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
		for (i=0; i<6; ++i) {
			if (pieces[!turn][i]->getPos(m.end)) {
				pieces[!turn][i]->setPos(m.end, false);
				break;
			}
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
		if (*castlePerms[turn][KING_SIDE]) {
			*castlePerms[turn][KING_SIDE] = false;
			turnLostCastlePerms[turn][KING_SIDE] = turnNumber;
		}
		if (*castlePerms[turn][QUEEN_SIDE]) {
			*castlePerms[turn][QUEEN_SIDE] = false;
			turnLostCastlePerms[turn][QUEEN_SIDE] = turnNumber;
		}

		// Updated rook position if King castled
		if (turn == WHITE) {
			// Hard-coded castle positions
			if (m.start == 4) {
				if (m.end == 6) {
					wR.setPos(7, false);
					wR.setPos(5, true);
				} else if (m.end == 2) {
					wR.setPos(0, false);
					wR.setPos(3, true);
				}
			}
		} else {	// Black's turn
			if (m.start == 60) {
				if (m.end == 62) {
					bR.setPos(63, false);
					bR.setPos(61, true);
				} else if (m.end == 58) {
					bR.setPos(56, false);
					bR.setPos(59, true);
				}
			}
		}
	}

	// Account for castling in rook movement
	if (m.piece == ROOK) {
		if (turn == WHITE) {
			// Removing relevant castling perms if rook moves
			if (m.start == 7 && wKCastle) {
				wKCastle = false;
				turnLostCastlePerms[WHITE][KING_SIDE] = turnNumber;
			} else if (m.start == 0 && wQCastle) {
				wQCastle = false;
				turnLostCastlePerms[WHITE][QUEEN_SIDE] = turnNumber;
			}
		} else {	// Black's turn
			if (m.start == 63 && bKCastle) {
				bKCastle = false;
				turnLostCastlePerms[BLACK][KING_SIDE] = turnNumber;
			} else if (m.start == 56 && bQCastle) {
				bQCastle = false;
				turnLostCastlePerms[BLACK][QUEEN_SIDE] = turnNumber;
			}
		}
	}

	// Update "all" bitboard
	this->updateAllBitboard(turn);
	// Update other color if a piece was killed
	if (m.killed != -1) {
		this->updateAllBitboard(!turn);
	}	

	// Account for en passant
	if (m.piece == PAWN) {
		if (turn == WHITE) {
			if (m.start >= 7 && m.start <= 15 && m.end >=24 && m.end <= 31) {
				enPassant = m.end - 8;
			} else {
				enPassant = -1;
			}
		} else {	// If black's turn
			if (m.start >= 48 && m.start <= 55 && m.end >=32 && m.end <= 39) {
				enPassant = m.end + 8;
			} else {
				enPassant = -1;
			}
		}
	} else {
		enPassant = -1;
	}

	// Updates move count
	if (turn == BLACK) {	// If black completed turn
		turnNumber += 1;
	}

	// Update turn color
	turn = !turn;
}

void ChessState::reverseMove() {
	/* 	Reverses a moves.
		Assumes move is valid.	*/

	Move* m = &moveList[moveList.size()-1];

	turn = !turn;	// Swaps turn

	if (turn == BLACK) {
		turnNumber -= 1;
	}

	// Updates piece location on bitboard
	if (m->promoted == -1) {
		pieces[turn][m->piece]->setPos(m->end, false);
	} else {
		pieces[turn][m->promoted]->setPos(m->end, false);
	}
	pieces[turn][m->piece]->setPos(m->start, true);

	// Adds previously killed piece to bitboard
	if (m->killed != -1) {
		pieces[!turn][m->killed]->setPos(m->end, true);
	}

	// Reverse rook movement in castling
	if (m->piece == KING) {
		if (turn == WHITE) {
			// Hard-coded castle positions
			if (m->start == 4) {
				if (m->end == 6) {
					wR.setPos(7, true);
					wR.setPos(5, false);
				} else if (m->end == 2) {
					wR.setPos(0, true);
					wR.setPos(3, false);
				}
			}
		} else {	// Black's turn
			if (m->start == 60) {
				if (m->end == 62) {
					bR.setPos(63, true);
					bR.setPos(61, false);
				} else if (m->end == 58) {
					bR.setPos(56, true);
					bR.setPos(59, false);
				}
			}
		}
	}

	// Update castle permissions
	if (*castlePerms[turn][KING_SIDE] == turnNumber) {
		*castlePerms[turn][KING_SIDE] = true;
		turnLostCastlePerms[turn][KING_SIDE] = -1;
	}
	if (*castlePerms[turn][QUEEN_SIDE] == turnNumber) {
		*castlePerms[turn][QUEEN_SIDE] = true;
		turnLostCastlePerms[turn][QUEEN_SIDE] = -1;
	}

	// Remove reversed move from move list
	moveList.pop_back();

	// Account for en passant
	m = &moveList[moveList.size()-1];
	if (m->piece == PAWN) {
		if (turn == WHITE) {
			if (m->start >= 7 && m->start <= 15 && m->end >=24 && m->end <= 31) {
				enPassant = m->end - 8;
			} else {
				enPassant = -1;
			}
		} else {	// If black's turn
			if (m->start >= 48 && m->start <= 55 && m->end >=32 && m->end <= 39) {
				enPassant = m->end + 8;
			} else {
				enPassant = -1;
			}
		}
	} else {
		enPassant = -1;
	}

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
	vector v = b.getPosVector();
	for (U8 i=0; i<v.size(); ++i) {
		arr[v[i]] = target;
	}
}

void ChessState::show() {
	this->show(true);
}

void ChessState::show(bool show_coords) {
	char board[64];
	U8 i;

	for (i=0; i<64; ++i) {
		board[i] = '.';
	}

	for (i=0; i<6; ++i) {
		this->mapBoardToChar(*pieces[0][i], board, piece_names[0][i]);
		this->mapBoardToChar(*pieces[1][i], board, piece_names[1][i]);
	}

	// TODO: ADD COORDS around board

	cout << "---------------" << endl;
	for (i=0; i<64; ++i) {
		cout << board[Bitboard::SHOW_ORDER[i]] << ' ';

		if ((i+1) % 8 == 0) {
			cout << endl;
		}
	}
	cout << "---------------" << endl;
}
