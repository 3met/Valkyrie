	
#include "uci.hpp"

// Clears the console.
void UCI::inputClear() {
	#ifdef WINDOWS
		system("cls");
	#elif defined LINUX
		system("clear");
	#endif
}
