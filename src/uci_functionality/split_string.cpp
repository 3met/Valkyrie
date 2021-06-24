
#include "uci.hpp"

// Splits a string into seperate words by using ' ' (space) as a break point
void UCI::splitString(string str, vector<string>* strVec) {
	size_t pos = str.find(' ');
	size_t startPos = 0;

	while (pos != std::string::npos) {
	    strVec->push_back(str.substr(startPos, pos-startPos));
	    startPos = pos + 1;

	    pos = str.find(' ', startPos);
	}
	strVec->push_back(str.substr(startPos, pos-startPos));
}
