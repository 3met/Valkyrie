
#ifndef MOVE_HPP
#define MOVE_HPP

class Move {
public:
	Move(char piece, short start, short end);
	Move(char piece, short start, short end, char killed);
	Move(char piece, short start, short end, char killed, char promoted);
	~Move();
	
	short start;
	short end;
	
	char piece;	// Piece being moved
	char killed;	// Piece killed; else ' '
	char promoted;	// Piece promoted to; else ' '
};

#endif
