
#include <string>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "U8.hpp"

using namespace std;

class DepthTestCase {
public:
	string FEN;
	U8 depth;

	DepthTestCase(string _FEN, U8 _depth) {
		FEN = _FEN;
		depth = _depth;
	};

	void run(ChessEngine* engine, ChessState* cs, bool verbose=true) {
		if (verbose) {
			cout << "Position: " << FEN << endl;
			cout << "Depth: " << short(depth) << endl;
		}

		cs->loadFEN(FEN);

		engine->searchDepth(*cs, depth);
	}
};

std::vector<DepthTestCase> DEPTH_TEST_LIST = {
	DepthTestCase("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6),
	DepthTestCase("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 6),
	DepthTestCase("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6),
	DepthTestCase("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 6),
	DepthTestCase("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 6),
	DepthTestCase("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 6),
	DepthTestCase("1k1r4/pp1b1R2/3q2pp/4p3/2B5/4Q3/PPP2B2/2K5 b - -", 6),
	DepthTestCase("3r1k2/4npp1/1ppr3p/p6P/P2PPPP1/1NR5/5K2/2R5 w - -", 6),
	DepthTestCase("2q1rr1k/3bbnnp/p2p1pp1/2pPp3/PpP1P1P1/1P2BNNP/2BQ1PRK/7R b - -", 6),
};

/* Returns true if passed, else false */
void depthTest(bool verbose) {
	if (verbose) {
		cout << "------ Begin Depth Test ------" << endl;
	}

	ChessEngine engine;
	ChessState cs;

	U64 totalDuration = 0;

	int i=0;
	for (i=0; i<DEPTH_TEST_LIST.size(); ++i) {
		if (verbose) {
			cout << "Stage: " << (i+1) << "/" << DEPTH_TEST_LIST.size() << endl;
		}

		engine.clear();

		auto start = chrono::high_resolution_clock::now();
		
		DEPTH_TEST_LIST[i].run(&engine, &cs, verbose);

		auto stop = chrono::high_resolution_clock::now();
		U64 duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
		totalDuration += duration;

		if (verbose) {
			cout << "Duration: " << int(duration/1000) << " ms" << endl;
			cout << "--------------------" << endl;
		}
	}

	cout << "Total Duration: " << int(totalDuration/1000) << " ms" << endl;
	
	if (verbose) {
		cout << "------- End Depth Test -------" << endl;
	}
}
