
#include "chess_state.hpp"

// Moves a piece on the board.
// Assumes move is valid.
void ChessState::move(Move m) {

	moveHistory.push(m);

	// Removes potential killed piece from bitboard
	if (m.killed != -1) {
		if (m.end == enPassant) {
			// Remove piece killed by en passant
			if (turn == WHITE) {
				pieces[BLACK][PAWN].setPosOff(m.end-8);
				bh.updatePiece(BLACK, PAWN, m.end-8);
			} else {	// Black's turn
				pieces[WHITE][PAWN].setPosOff(m.end+8);
				bh.updatePiece(WHITE, PAWN, m.end+8);
			}
		} else {
			pieces[!turn][m.killed].setPosOff(m.end);
			bh.updatePiece(!turn, m.killed, m.end);
		}
	}

	// Updates moving piece location on bitboard
	pieces[turn][m.piece].setPosOff(m.start);
	bh.updatePiece(turn, m.piece, m.start);
	if (m.promoted == -1) {
		pieces[turn][m.piece].setPosOn(m.end);
		bh.updatePiece(turn, m.piece, m.end);
	} else {
		pieces[turn][m.promoted].setPosOn(m.end);
		bh.updatePiece(turn, m.promoted, m.end);
	}

	// Account for castling in king movement
	if (m.piece == KING) {
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
		if (m.start == KING_START[turn]) {
			// If king side castled
			if (m.end == KING_START[turn]+2) {
				pieces[turn][ROOK].setPosOff(KING_START[turn]+3);
				pieces[turn][ROOK].setPosOn(KING_START[turn]+1);
				bh.updatePiece(turn, ROOK, KING_START[turn]+3);
				bh.updatePiece(turn, ROOK, KING_START[turn]+1);
			// If queen side castled
			} else if (m.end == KING_START[turn]-2) {
				pieces[turn][ROOK].setPosOff(KING_START[turn]-4);
				pieces[turn][ROOK].setPosOn(KING_START[turn]-1);
				bh.updatePiece(turn, ROOK, KING_START[turn]-4);
				bh.updatePiece(turn, ROOK, KING_START[turn]-1);
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
			moveLostCastlePerms[turn][KING_SIDE] = moveNumber;
			bh.updateCastlePerms(turn, KING_SIDE);
		// Queen's side
		} else if (ROOK_START[turn][KING_SIDE]
			&& castlePerms[turn][QUEEN_SIDE]) {
			
			castlePerms[turn][QUEEN_SIDE] = false;
			moveLostCastlePerms[turn][QUEEN_SIDE] = moveNumber;
			bh.updateCastlePerms(turn, QUEEN_SIDE);
		}
	}

	// Account for castling in rook death
	if (m.killed == ROOK) {
		if (castlePerms[!turn][KING_SIDE] && m.end == ROOK_START[!turn][KING_SIDE]) {
			castlePerms[!turn][KING_SIDE] = false;
			moveLostCastlePerms[!turn][KING_SIDE] = moveNumber;	
			bh.updateCastlePerms(!turn, KING_SIDE);
		} else if (castlePerms[!turn][QUEEN_SIDE] && m.end == ROOK_START[!turn][QUEEN_SIDE]) {
			castlePerms[!turn][QUEEN_SIDE] = false;
			moveLostCastlePerms[!turn][QUEEN_SIDE] = moveNumber;	
			bh.updateCastlePerms(!turn, QUEEN_SIDE);
		}
	}

	// Update enPassant value
	if (m.piece == PAWN) {
		if (turn == WHITE) {
			// If pawn moved two squares forward
			if (Bitboard::RANK[m.start] == 1 && Bitboard::RANK[m.end] == 3) {
				bh.updateEnPassant(enPassant, m.end-8);
				enPassant = m.end-8;
			} else {
				bh.updateEnPassant(enPassant, -1);
				enPassant = -1;
			}
		} else {	// If black's turn
			if (Bitboard::RANK[m.start] == 6 && Bitboard::RANK[m.end] == 4) {
				bh.updateEnPassant(enPassant, m.end+8);
				enPassant = m.end+8;
			} else {
				bh.updateEnPassant(enPassant, -1);
				enPassant = -1;
			}
		}
	} else {
		bh.updateEnPassant(enPassant, -1);
		enPassant = -1;
	}
	enPassantHistory.push(enPassant);

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
	bh.updateTurn();
}

// Reverses a moves.
// Assumes move is valid.
void ChessState::reverseMove() {

	Move m = moveHistory.top();

	turn = !turn;	// Swaps turn
	bh.updateTurn();

	if (turn == BLACK) {
		turnNumber -= 1;
	}
	moveNumber -= 1;

	// Revert en passant value
	S8 oldEnPassant(enPassantHistory.top());
	enPassantHistory.pop();
	// If en passant variable needs to be updated
	if (oldEnPassant != enPassantHistory.top()) {
		enPassant = enPassantHistory.top();
		bh.updateEnPassant(oldEnPassant, enPassant);
	}

	// Updates piece location on bitboard
	if (m.promoted == -1) {
		pieces[turn][m.piece].setPosOff(m.end);
		bh.updatePiece(turn, m.piece, m.end);
	} else {
		pieces[turn][m.promoted].setPosOff(m.end);
		bh.updatePiece(turn, m.promoted, m.end);
	}
	pieces[turn][m.piece].setPosOn(m.start);
	bh.updatePiece(turn, m.piece, m.start);

	// Adds previously killed piece to bitboard
	if (m.killed != -1) {
		if (m.end == enPassant) {
			// Place killed en passant piece
			if (turn == WHITE) {
				pieces[BLACK][PAWN].setPosOn(m.end-8);
				bh.updatePiece(BLACK, PAWN, m.end-8);
			} else {	// Black's turn
				pieces[WHITE][PAWN].setPosOn(m.end+8);
				bh.updatePiece(WHITE, PAWN, m.end+8);
			}
		} else {
			pieces[!turn][m.killed].setPosOn(m.end);
			bh.updatePiece(!turn, m.killed, m.end);
		}
	}

	// Reverse rook movement in castling
	if (m.piece == KING) {
		if (m.start == KING_START[turn]) {
			if (m.end == KING_START[turn]+2) {
				pieces[turn][ROOK].setPosOn(KING_START[turn]+3);
				pieces[turn][ROOK].setPosOff(KING_START[turn]+1);
				bh.updatePiece(turn, ROOK, KING_START[turn]+3);
				bh.updatePiece(turn, ROOK, KING_START[turn]+1);
			} else if (m.end == KING_START[turn]-2) {
				pieces[turn][ROOK].setPosOn(KING_START[turn]-4);
				pieces[turn][ROOK].setPosOff(KING_START[turn]-1);
				bh.updatePiece(turn, ROOK, KING_START[turn]-4);
				bh.updatePiece(turn, ROOK, KING_START[turn]-1);
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

	// Remove reversed move from move list
	moveHistory.pop();

	// Update both universal bitboards
	this->updateAllBitboard(turn);
	this->updateAllBitboard(!turn);
}
