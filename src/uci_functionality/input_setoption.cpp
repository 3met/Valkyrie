	
#include <string>
#include "uci.hpp"

// Sets UCI/engine option
void UCI::inputSetOption(string input) {
	// Break down input into words
	std::vector<std::string> inVec;
	UCI::splitString(input, &inVec);

	// Fetch option name
	string name = "";
	short pos(2);
	while (pos != inVec.size() && inVec[pos] != "value") {
		if (name != "") {
			name += ' ';
		}
		name += inVec[pos];
		
		pos += 1;
	}

	pos += 1;

	// Fetch value if given
	string value = "";
	if (pos != inVec.size()) {
		for ( ; pos<inVec.size(); ++pos) {
			value += inVec[pos];
		}
	}

	// Set option based on value
	if (name == "Hash") {
		engine.transTable->resize(stoi(value) * 1000000);
	} else if (name == "Clear Hash") {
		engine.transTable->clear();
	} else if (name == "Move Overhead") {
		this->moveOverhead = stoi(value);
	} else if (name == "Threads") {
		// TODO: add thread options
	} else if (name == "OwnBook") {
		if (value == "true" || value == "True" || value == "TRUE") {
			engine.useOwnBook = true;
		} else {
			engine.useOwnBook = false;
		}
	} else if (name == "UCI_EngineAbout") {

	} else {
		cout << "Unable to set option \"" << name << "\"" << '\n';
	}
}
