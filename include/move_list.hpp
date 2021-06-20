
#pragma once
#ifndef MOVE_LIST_HPP
#define MOVE_LIST_HPP

#include <stack>
#include "move.hpp"

class MoveList {
private:
	stack<Move> line;

public:
	MoveList();

	inline void clear() {
		while(!line.empty()) {
			line.pop();
		}
	};

	inline bool empty() {
		return line.empty();
	};

	inline size_t size() {
		return line.size();
	};

	inline Move top() {
		return line.top();
	};

	inline void pop() {
		line.pop();
	};

	inline void push(Move m) {
		line.push(m);
	};

	inline stack<Move> toStack() const {
		return line;
	};

	friend ostream& operator<<(ostream& os, const MoveList& mList);
};

#endif
