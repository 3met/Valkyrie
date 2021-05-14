
#include "bitboard.hpp"
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

/* Check if a ray-movement hit an attacker
   "turn" represents the color of the attacker */
inline bool checkCardinalAttacker(ChessState* cs, bool turn, pair<bool, S8> rayPiece) {
	if (rayPiece.second != -1 && rayPiece.first == turn) {
		S8 pieceType = cs->getPieceType(turn, rayPiece.second);
		if (pieceType == ChessState::ROOK || pieceType == ChessState::QUEEN) {
			return true;
		}
	}
	return false;
}

inline bool checkDiagonalAttacker(ChessState* cs, bool turn, pair<bool, S8> rayPiece) {
	if (rayPiece.second != -1 && rayPiece.first == turn) {
		S8 pieceType = cs->getPieceType(turn, rayPiece.second);
		if (pieceType == ChessState::BISHOP || pieceType == ChessState::QUEEN) {
			return true;
		}
	}
	return false;
}

/* Returns whether a square is being attacked pseudo-legally
   Useful for detecting check and castling ability.
   "turn" represents the attacking color */
bool ChessEngine::isPosAttacked(ChessState* cs, bool turn, U8 pos) {
	// Check if attacked by knight
	if ((NMoveDB[pos].board & cs->pieces[turn][cs->KNIGHT]->board) != 0) {
		return true;
	}
	// Check if attacked by king
	if ((KMoveDB[pos].board & cs->pieces[turn][cs->KING]->board) != 0) {
		return true;
	}

	// Bool is color, S8 is piece position
	pair<bool, S8> rayPiece;

	// Check if attacked cardinally
	rayPiece = nextPieceCardinal(cs, pos, UP);
	if (checkCardinalAttacker(cs, turn, rayPiece)) {
		return true;
	}
	rayPiece = nextPieceCardinal(cs, pos, RIGHT);
	if (checkCardinalAttacker(cs, turn, rayPiece)) {
		return true;
	}
	rayPiece = nextPieceCardinal(cs, pos, DOWN);
	if (checkCardinalAttacker(cs, turn, rayPiece)) {
		return true;
	}
	rayPiece = nextPieceCardinal(cs, pos, LEFT);
	if (checkCardinalAttacker(cs, turn, rayPiece)) {
		return true;
	}

	// Check if attacked diagonally
	rayPiece = nextPieceDiagonal(cs, pos, UP_RIGHT);
	if (checkDiagonalAttacker(cs, turn, rayPiece)) {
		return true;
	}
	rayPiece = nextPieceDiagonal(cs, pos, DOWN_RIGHT);
	if (checkDiagonalAttacker(cs, turn, rayPiece)) {
		return true;
	}
	rayPiece = nextPieceDiagonal(cs, pos, DOWN_LEFT);
	if (checkDiagonalAttacker(cs, turn, rayPiece)) {
		return true;
	}
	rayPiece = nextPieceDiagonal(cs, pos, UP_LEFT);
	if (checkDiagonalAttacker(cs, turn, rayPiece)) {
		return true;
	}

	// Check pawn attacks
	if (turn == cs->WHITE) {
		if (Bitboard::RANK[pos] >= 2) {
			// Right and left pawn attacks
			if (Bitboard::FILE[pos] >= 1 && cs->wP.getPos(pos-9)) {
				return true;
			}
			if (Bitboard::FILE[pos] <= 6 && cs->wP.getPos(pos-7)) {
				return true;
			}
		}
	} else {	// Black's attacking
		if (Bitboard::RANK[pos] <= 5) {
			// Right and left pawn attacks
			if (Bitboard::FILE[pos] >= 1 && cs->bP.getPos(pos+7)) {
				return true;
			}
			if (Bitboard::FILE[pos] <= 6 && cs->bP.getPos(pos+9)) {
				return true;
			}
		}
	}

	return false;
}
