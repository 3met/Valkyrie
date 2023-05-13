
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

// Contains a test case for testing the Effective Branching Factor (EBF)
class EBFTestCase {
public:
	string FEN;
	U8 depth;
	ChessState* cs;

	EBFTestCase(string _FEN, U8 _depth) {
		FEN = _FEN;
		if (_depth < 3) {
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
			printf("EBF: %.3f\n", double(EBF));
		}

		return EBF;
	}
};

std::vector<EBFTestCase> EBF_TEST_LIST = {
	// Misc Positions
	EBFTestCase("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 8),
	EBFTestCase("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 8),
	EBFTestCase("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -", 7),
	EBFTestCase("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 8),
	EBFTestCase("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -", 8),
	EBFTestCase("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -", 8),
	EBFTestCase("1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -", 8),
	EBFTestCase("3r1k2/4npp1/1ppr3p/p6P/P2PPPP1/1NR5/5K2/2R5 w - -", 8),
	EBFTestCase("2q1rr1k/3bbnnp/p2p1pp1/2pPp3/PpP1P1P1/1P2BNNP/2BQ1PRK/7R b - -", 8),
	// Mid Game Positions
	// Taken From: https://www.talkchess.com/forum3/viewtopic.php?t=58359&start=2
	EBFTestCase("1kr5/3n4/q3p2p/p2n2p1/PppB1P2/5BP1/1P2Q2P/3R2K1 w - -", 8),
	EBFTestCase("1n5k/3q3p/pp1p2pB/5r2/1PP1Qp2/P6P/6P1/2R3K1 w - -", 9),
	EBFTestCase("1n6/4bk1r/1p2rp2/pP2pN1p/K1P1N2P/8/P5R1/3R4 w - -", 9),
	EBFTestCase("1nr5/1k5r/p3pqp1/3p4/1P1P1PP1/R4N2/3Q1PK1/R7 w - -", 7),
	EBFTestCase("1q4k1/5p1p/p1rprnp1/3R4/N1P1P3/1P6/P5PP/3Q1R1K w - -", 8),
	EBFTestCase("1qr1k2r/1p2bp2/pBn1p3/P2pPbpp/5P2/2P1QBPP/1P1N3R/R4K2 b k -", 8),
	EBFTestCase("1r1b2k1/2r2ppp/p1qp4/3R1NPP/1pn1PQB1/8/PPP3R1/1K6 w - -", 7),
	EBFTestCase("1r1qk1nr/p3ppbp/3p2p1/1pp5/2bPP3/4B1P1/2PQNPBP/R2R2K1 w k -", 8),
	EBFTestCase("1r2n1rk/pP2q2p/P2p4/4pQ2/2P2p2/5B1P/3R1P1K/3R4 w - -", 8),
	EBFTestCase("1r4k1/1rq2pp1/3b1nn1/pBpPp3/P1N4p/2PP1Q1P/6PB/2R2RK1 w - -", 8),
	EBFTestCase("1r4k1/p1rqbp1p/b1p1p1p1/NpP1P3/3PB3/3Q2P1/P4P1P/3RR1K1 w - -", 8),
	EBFTestCase("1r6/R1nk1p2/1p4pp/pP1p1P2/P2P3P/5PN1/5K2/8 w - -", 10),
	EBFTestCase("1rr3k1/1q3pp1/pnbQp2p/1p2P3/3B1P2/2PB4/P1P2RPP/R5K1 w - -", 7),
	EBFTestCase("2b1r1k1/1p6/pQ1p1q1p/P2P3P/2P1pPpN/6P1/4R1K1/8 w - -", 8),
	EBFTestCase("2bq2k1/1pr3bp/1Qpr2p1/P2pNp2/3P1P1P/6P1/5PB1/1RR3K1 w - -", 8),
	EBFTestCase("2kr4/ppqnbp1r/2n1p1p1/P2pP3/3P2P1/3BBN2/1P1Q1PP1/R4RK1 w - -", 8),
	EBFTestCase("2r1kb1r/1bqn1pp1/p3p3/1p2P1P1/3Np3/P1N1B3/1PP1Q2P/R4RK1 w k -", 8),
};

// Times a list of test cases.
void EBF_Test(bool verbose) {
	if (verbose) {
		printf("------ Begin EBF Test ------\n");
	}

	ChessEngine engine;
	ChessState cs;

	float meanEBF(0);

	for (size_t i(0); i<EBF_TEST_LIST.size(); ++i) {
		if (verbose) {
			printf("Stage: %d/%d\n", short(i)+1, short(EBF_TEST_LIST.size()));
		}

		engine.clear();
		EBF_TEST_LIST[i].load(&cs);
		
		meanEBF += EBF_TEST_LIST[i].run(&engine, verbose);


		if (verbose) {
			printf("--------------------\n");
		}
	}

	meanEBF /= EBF_TEST_LIST.size();
	printf("Mean EBF: %.3f\n", double(meanEBF));
	
	if (verbose) {
		printf("------- End EBF Test -------\n");
	}
}
