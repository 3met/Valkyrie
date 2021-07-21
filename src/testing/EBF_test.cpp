
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

using namespace std;

// Contains a test case for testing the effective branching factor
class EBFTestCase {
public:
	string FEN;
	U8 depth;
	ChessState* cs;

	EBFTestCase(string _FEN, U8 _depth) {
		FEN = _FEN;
		if (depth < 3) {
			depth = 3;
		} else {
			depth = _depth;
		}
	};

	void load(ChessState* _cs) {
		cs = _cs;
		cs->loadFEN(this->FEN);
	}

	float run(ChessEngine* engine, bool verbose=true) {
		if (verbose) {
			cout << "Position: " << FEN << '\n';
			printf("Depth: %d\n", short(depth));
		}

		engine->searchDepth(*cs, depth);
		U64 N = engine->nodesTotal;

		engine->clear();
		
		engine->searchDepth(*cs, depth-2);
		U64 N2 = engine->nodesTotal;

		float EBF = sqrt(N / N2);

		if (verbose) {
			printf("EBF: %.3f\n", EBF);
		}

		return EBF;
	}
};

std::vector<EBFTestCase> EBF_TEST_LIST = {
	EBFTestCase("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 7),
	EBFTestCase("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 7),
	EBFTestCase("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -", 7),
	EBFTestCase("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 7),
	EBFTestCase("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -", 7),
	// EBFTestCase("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -", 7),
	// EBFTestCase("1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -", 6),
	// EBFTestCase("3r1k2/4npp1/1ppr3p/p6P/P2PPPP1/1NR5/5K2/2R5 w - -", 6),
	// EBFTestCase("2q1rr1k/3bbnnp/p2p1pp1/2pPp3/PpP1P1P1/1P2BNNP/2BQ1PRK/7R b - -", 6),
};

// Times a list of test cases.
void EBF_Test(bool verbose) {
	if (verbose) {
		printf("------ Begin EBF Test ------\n");
	}

	ChessEngine engine;
	ChessState cs;

	float meanEBF(0);

	for (short i(0); i<EBF_TEST_LIST.size(); ++i) {
		if (verbose) {
			printf("Stage: %d/%d\n", i+1, short(EBF_TEST_LIST.size()));
		}

		engine.clear();
		EBF_TEST_LIST[i].load(&cs);
		
		meanEBF += EBF_TEST_LIST[i].run(&engine, verbose);


		if (verbose) {
			printf("--------------------\n");
		}
	}

	meanEBF /= EBF_TEST_LIST.size();
	printf("Mean EBF: %.3f\n", meanEBF);
	
	if (verbose) {
		printf("------- End EBF Test -------\n");
	}
}
