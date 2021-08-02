
#include "board_defs.hpp"
#include "chess_state.hpp"

// Ensures the moving piece stored in passed move is updated
void ChessState::updateMovingPiece(Move* m) {
	if (m->getMovingPiece() == UNKNOWN_PIECE) {
		m->setMovingPiece(getPieceType(turn, m->getStart()));
	}
}

// Ensures the moving piece stored in passed move is updated
void ChessState::updateCapturedPiece(Move* m) {
	if (m->getCapturedPiece() == UNKNOWN_PIECE) {
		m->setCapturedPiece(getPieceType(!turn, m->getEnd()));
	}
}

// Moves a piece on the board.
// Assumes move is valid.
// Updates boardHash with move.
void ChessState::move(Move m) {

	updateMovingPiece(&m);
	
	U8 start = m.getStart();
	U8 end = m.getEnd();
	U8 piece = m.getMovingPiece();

	// Removes potential killed piece from bitboard
	if (m.isCapture()) {

		updateCapturedPiece(&m);
		U8 capturedPiece = m.getCapturedPiece();

		// Check for special en passant capture
		if (m.getFlags() == Move::EP_CAPTURE) {
			// Remove piece killed by en passant
			if (turn == WHITE) {
				pieces[BLACK][PAWN].setPosOff(end-8);
				bh.updatePiece(BLACK, PAWN, end-8);
			} else {	// Black's turn
				pieces[WHITE][PAWN].setPosOff(end+8);
				bh.updatePiece(WHITE, PAWN, end+8);
			}
		// Regular captures
		} else {
			U8 captured = m.getCapturedPiece();

			// Remove captured piece from board
			pieces[!turn][captured].setPosOff(end);
			bh.updatePiece(!turn, captured, end);

			// Account for castling in rook death
			if (captured == ROOK) {
				if (castlePerms[!turn][KING_SIDE] && end == ROOK_START[!turn][KING_SIDE]) {
					castlePerms[!turn][KING_SIDE] = false;
					moveLostCastlePerms[!turn][KING_SIDE] = moveNumber;	
					bh.updateCastlePerms(!turn, KING_SIDE);
				} else if (castlePerms[!turn][QUEEN_SIDE] && end == ROOK_START[!turn][QUEEN_SIDE]) {
					castlePerms[!turn][QUEEN_SIDE] = false;
					moveLostCastlePerms[!turn][QUEEN_SIDE] = moveNumber;	
					bh.updateCastlePerms(!turn, QUEEN_SIDE);
				}
			}
		}

		// Update "all" bitboard
		this->updateAllBitboard(!turn);
	}

	// Updates moving piece location on bitboard
	pieces[turn][piece].setPosOff(start);
	bh.updatePiece(turn, piece, start);
	// Account for changing piece in promotion
	if (m.isPromotion()) {
		switch (m.getFlags()) {
			case Move::QUEEN_PROMO:
			case Move::QUEEN_PROMO_CAPTURE:
				pieces[turn][QUEEN].setPosOn(end);
				bh.updatePiece(turn, QUEEN, end);
				break;
			case Move::KNIGHT_PROMO:
			case Move::KNIGHT_PROMO_CAPTURE:
				pieces[turn][KNIGHT].setPosOn(end);
				bh.updatePiece(turn, KNIGHT, end);
				break;
			case Move::ROOK_PROMO:
			case Move::ROOK_PROMO_CAPTURE:
				pieces[turn][ROOK].setPosOn(end);
				bh.updatePiece(turn, ROOK, end);
				break;
			case Move::BISHOP_PROMO:
			case Move::BISHOP_PROMO_CAPTURE:
				pieces[turn][BISHOP].setPosOn(end);
				bh.updatePiece(turn, BISHOP, end);
				break;
		}
	} else {
		pieces[turn][piece].setPosOn(end);
		bh.updatePiece(turn, piece, end);
	}

	// Update en passant value
	if (piece == PAWN) {
		if (turn == WHITE) {
			// If pawn moved two squares forward
			if (BOARD_RANK[start] == 1 && BOARD_RANK[end] == 3) {
				bh.updateEnPassant(enPassantHistory[moveNumber-1], end-8);
				enPassantHistory[moveNumber] = end-8;
			} else {
				bh.updateEnPassant(enPassantHistory[moveNumber-1], -1);
				enPassantHistory[moveNumber] = -1;
			}
		} else {	// If black's turn
			if (BOARD_RANK[start] == 6 && BOARD_RANK[end] == 4) {
				bh.updateEnPassant(enPassantHistory[moveNumber-1], end+8);
				enPassantHistory[moveNumber] = end+8;
			} else {
				bh.updateEnPassant(enPassantHistory[moveNumber-1], -1);
				enPassantHistory[moveNumber] = -1;
			}
		}
	} else {
		bh.updateEnPassant(enPassantHistory[moveNumber-1], -1);
		enPassantHistory[moveNumber] = -1;

		// Account for castling in rook movement
		// Removing relevant castling perms if rook moves
		if (piece == ROOK) {
			// King's side
			if (start == ROOK_START[turn][KING_SIDE]
				&& castlePerms[turn][KING_SIDE]) {
				
				castlePerms[turn][KING_SIDE] = false;
				moveLostCastlePerms[turn][KING_SIDE] = moveNumber;
				bh.updateCastlePerms(turn, KING_SIDE);
			// Queen's side
			} else if (start == ROOK_START[turn][QUEEN_SIDE]
				&& castlePerms[turn][QUEEN_SIDE]) {
				
				castlePerms[turn][QUEEN_SIDE] = false;
				moveLostCastlePerms[turn][QUEEN_SIDE] = moveNumber;
				bh.updateCastlePerms(turn, QUEEN_SIDE);
			}
		
		// Account for castling in king movement
		} else if (piece == KING) {
			// Update castle permissions
			if (castlePerms[turn][KING_SIDE]) {
				castlePerms[turn][KING_SIDE] = false;
				moveLostCastlePerms[turn][KING_SIDE] = moveNumber;
				bh.updateCastlePerms(turn, KING_SIDE);
			}
			if (castlePerms[turn][QUEEN_SIDE]) {
				castlePerms[turn][QUEEN_SIDE] = false;
				moveLostCastlePerms[turn][QUEEN_SIDE] = moveNumber;
				bh.updateCastlePerms(turn, QUEEN_SIDE);
			}

			// Update rook positions if castled
			if (start == KING_START[turn]) {
				// If king side castled
				if (end == KING_START[turn]+2) {
					pieces[turn][ROOK].setPosOff(KING_START[turn]+3);
					pieces[turn][ROOK].setPosOn(KING_START[turn]+1);
					bh.updatePiece(turn, ROOK, KING_START[turn]+3);
					bh.updatePiece(turn, ROOK, KING_START[turn]+1);
				// If queen side castled
				} else if (end == KING_START[turn]-2) {
					pieces[turn][ROOK].setPosOff(KING_START[turn]-4);
					pieces[turn][ROOK].setPosOn(KING_START[turn]-1);
					bh.updatePiece(turn, ROOK, KING_START[turn]-4);
					bh.updatePiece(turn, ROOK, KING_START[turn]-1);
				}
			}
		}
	}

	// Update "all" bitboard
	this->updateAllBitboard(turn);

	// Updates move count
	if (turn == BLACK) {	// If black completed turn
		turnNumber += 1;
	}
	moveHistory[moveNumber] = m;
	moveNumber += 1;

	// Update turn color
	turn = !turn;
	bh.updateTurn();
}

// Reverses a moves.
// Assumes move is valid.
void ChessState::reverseMove() {

	turn = !turn;	// Swaps turn
	bh.updateTurn();

	if (turn == BLACK) {
		turnNumber -= 1;
	}
	moveNumber -= 1;

	Move& m = moveHistory[moveNumber];

	U8 start = m.getStart();
	U8 end = m.getEnd();
	U8 piece = m.getMovingPiece();

	// If en passant variable needs to be updated
	if (enPassantHistory[moveNumber] != enPassantHistory[moveNumber-1]) {
		bh.updateEnPassant(enPassantHistory[moveNumber], enPassantHistory[moveNumber-1]);
	}

	// Updates moving piece location on bitboard
	// Reverse piece change if there was a promotion
	if (m.isPromotion()) {
		U8 promotionPiece = m.promotionPiece();
		pieces[turn][promotionPiece].setPosOff(end);
		bh.updatePiece(turn, promotionPiece, end);
	} else {
		pieces[turn][piece].setPosOff(end);
		bh.updatePiece(turn, piece, end);
	}
	// Add piece to start location
	pieces[turn][piece].setPosOn(start);
	bh.updatePiece(turn, piece, start);

	// Adds previously killed piece to bitboard
	if (m.isCapture()) {
		if (end == enPassantHistory[moveNumber-1]) {
			// Place killed en passant piece
			if (turn == WHITE) {
				pieces[BLACK][PAWN].setPosOn(end-8);
				bh.updatePiece(BLACK, PAWN, end-8);
			} else {	// Black's turn
				pieces[WHITE][PAWN].setPosOn(end+8);
				bh.updatePiece(WHITE, PAWN, end+8);
			}
		} else {

			U8 captured = m.getCapturedPiece();

			pieces[!turn][captured].setPosOn(end);
			bh.updatePiece(!turn, captured, end);
		}

		this->updateAllBitboard(!turn);
	} else {

		// Reverse rook movement in castling
		if (piece == KING) {
			if (start == KING_START[turn]) {
				if (end == KING_START[turn]+2) {
					pieces[turn][ROOK].setPosOn(KING_START[turn]+3);
					pieces[turn][ROOK].setPosOff(KING_START[turn]+1);
					bh.updatePiece(turn, ROOK, KING_START[turn]+3);
					bh.updatePiece(turn, ROOK, KING_START[turn]+1);
				} else if (end == KING_START[turn]-2) {
					pieces[turn][ROOK].setPosOn(KING_START[turn]-4);
					pieces[turn][ROOK].setPosOff(KING_START[turn]-1);
					bh.updatePiece(turn, ROOK, KING_START[turn]-4);
					bh.updatePiece(turn, ROOK, KING_START[turn]-1);
				}
			}
		}
	}

	// Update castle permissions
	if (moveLostCastlePerms[turn][KING_SIDE] == moveNumber) {
		castlePerms[turn][KING_SIDE] = true;
		moveLostCastlePerms[turn][KING_SIDE] = -1;
		bh.updateCastlePerms(turn, KING_SIDE);
	}
	if (moveLostCastlePerms[turn][QUEEN_SIDE] == moveNumber) {
		castlePerms[turn][QUEEN_SIDE] = true;
		moveLostCastlePerms[turn][QUEEN_SIDE] = -1;
		bh.updateCastlePerms(turn, QUEEN_SIDE);
	}
	// One can lose castle perms regardless of turn
	if (moveLostCastlePerms[!turn][KING_SIDE] == moveNumber) {
		castlePerms[!turn][KING_SIDE] = true;
		moveLostCastlePerms[!turn][KING_SIDE] = -1;
		bh.updateCastlePerms(!turn, KING_SIDE);
	}
	if (moveLostCastlePerms[!turn][QUEEN_SIDE] == moveNumber) {
		castlePerms[!turn][QUEEN_SIDE] = true;
		moveLostCastlePerms[!turn][QUEEN_SIDE] = -1;
		bh.updateCastlePerms(!turn, QUEEN_SIDE);
	}

	// Update both universal bitboards
	this->updateAllBitboard(turn);
}

// Does a null move.
// Updates boardHash with move.
void ChessState::moveNull() {

	moveHistory[moveNumber] = Move::NULL_MOVE;

	// Update en passant value
	if (enPassantHistory[moveNumber-1] != -1) {
		bh.updateEnPassant(enPassantHistory[moveNumber-1], -1);
	}
	enPassantHistory[moveNumber] = -1;

	// Updates move count
	if (turn == BLACK) {	// If black completed turn
		turnNumber += 1;
	}
	moveNumber += 1;

	// Update turn color
	turn = !turn;
	bh.updateTurn();
}

// Reverses a null move.
// Updates boardHash with move.
void ChessState::reverseNullMove() {

	turn = !turn;	// Swaps turn
	bh.updateTurn();

	if (turn == BLACK) {
		turnNumber -= 1;
	}
	moveNumber -= 1;

	Move m = moveHistory[moveNumber];

	// If en passant variable needs to be updated
	if (enPassantHistory[moveNumber] != enPassantHistory[moveNumber-1]) {
		bh.updateEnPassant(enPassantHistory[moveNumber], enPassantHistory[moveNumber-1]);
	}
}
