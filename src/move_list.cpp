
#include "move_list.hpp"

using namespace std;

MoveList::MoveList() {}

/* Creates a copy of the stack to print */
ostream& operator<<(ostream &os, const MoveList& mList) { 
	
	stack<Move> temp = mList.toStack();

	while(!temp.empty()) {
		os << temp.top();
		temp.pop();
	}

	return os;
}
