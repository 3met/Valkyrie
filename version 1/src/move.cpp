
#include <iostream>
#include "../include/move.hpp"

Move::Move(char p, short s, short e) {
	piece = p;
	start = s;
	end = e;
};

Move::~Move() {
	//std::cout << "DELETED" << std::endl;	
};
