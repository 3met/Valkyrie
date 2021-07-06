
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "chess_engine.hpp"
#include "chess_state.hpp"
#include "size_defs.hpp"

using namespace std;

// Contains a test case for testing engine's perft function.
class PerftTestCase {
public:
	string FEN;
	U8 depth;
	U64 target;
	ChessState* cs;

	PerftTestCase(string _FEN, U8 _depth, U64 _target) {
		FEN = _FEN;
		depth = _depth;
		target = _target;
	};

	void load(ChessState* _cs) {
		cs = _cs;
		cs->loadFEN(this->FEN);
	}

	bool run(ChessEngine* engine, bool verbose=true) {
		if (verbose) {
			cout << "Position: " << FEN << endl;
		}

		U64 result(engine->perft(cs, depth));

		if (verbose) {
			if (result == target) {
				cout << "Status: Passed" << endl;
			} else {
				cout << "Status: Failed" << endl;
				cout << "Depth: " << short(depth) << endl;
				cout << "Target: " << target << endl;
				cout << "Result: " << result << endl;
			}
		}

		if (result == target) {
			return true;
		} else {
			return false;
		}
	}
};

std::vector<PerftTestCase> PERFT_TEST_LIST = {
	// From: http://talkchess.com/forum3/viewtopic.php?t=47318
	// avoid illegal en passant capture:
	PerftTestCase("8/5bk1/8/2Pp4/8/1K6/8/8 w - d6 0 1", 6, 824064),
	PerftTestCase("8/8/1k6/8/2pP4/8/5BK1/8 b - d3 0 1", 6, 824064),
	// en passant capture checks opponent:
	PerftTestCase("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6, 1440467),
	PerftTestCase("8/5k2/8/2Pp4/2B5/1K6/8/8 w - d6 0 1", 6, 1440467),
	// short castling gives check:
	PerftTestCase("5k2/8/8/8/8/8/8/4K2R w K - 0 1", 6, 661072),
	PerftTestCase("4k2r/8/8/8/8/8/8/5K2 b k - 0 1", 6, 661072),
	// long castling gives check:
	PerftTestCase("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", 6, 803711),
	PerftTestCase("r3k3/8/8/8/8/8/8/3K4 b q - 0 1", 6, 803711),
	// castling (including losing cr due to rook capture):
	PerftTestCase("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4, 1274206),
	PerftTestCase("r3k2r/7b/8/8/8/8/1B4BQ/R3K2R b KQkq - 0 1", 4, 1274206),
	// castling prevented:
	PerftTestCase("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 4, 1720476),
	PerftTestCase("r3k2r/8/5Q2/8/8/3q4/8/R3K2R w KQkq - 0 1", 4, 1720476),
	// promote out of check:
	PerftTestCase("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", 6, 3821001),
	PerftTestCase("3K4/8/8/8/8/8/4p3/2k2R2 b - - 0 1", 6, 3821001),
	// discovered check:
	PerftTestCase("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", 5, 1004658),
	PerftTestCase("5K2/8/1Q6/2N5/8/1p2k3/8/8 w - - 0 1", 5, 1004658),
	// promote to give check:
	PerftTestCase("4k3/1P6/8/8/8/8/K7/8 w - - 0 1", 6, 217342),
	PerftTestCase("8/k7/8/8/8/8/1p6/4K3 b - - 0 1", 6, 217342),
	// underpromote to check:
	PerftTestCase("8/P1k5/K7/8/8/8/8/8 w - - 0 1", 6, 92683),
	PerftTestCase("8/8/8/8/8/k7/p1K5/8 b - - 0 1", 6, 92683),
	// self stalemate:
	PerftTestCase("K1k5/8/P7/8/8/8/8/8 w - - 0 1", 6, 2217),
	PerftTestCase("8/8/8/8/8/p7/8/k1K5 b - - 0 1", 6, 2217),
	// stalemate/checkmate:
	PerftTestCase("8/k1P5/8/1K6/8/8/8/8 w - - 0 1", 7, 567584),
	PerftTestCase("8/8/8/8/1k6/8/K1p5/8 b - - 0 1", 7, 567584),
	// double check:
	PerftTestCase("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4, 23527),
	PerftTestCase("8/5k2/8/5N2/5Q2/2K5/8/8 w - - 0 1", 4, 23527),
	// From: https://www.chessprogramming.org/Perft_Results
	PerftTestCase("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5, 4865609),
	PerftTestCase("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 4, 4085603),
	PerftTestCase("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333),
	PerftTestCase("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 5, 674624),
	PerftTestCase("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 2103487),
	PerftTestCase("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 4, 3894594),
};

// Times perft test cases.
// Returns true if passed, else false.
// Good assesment of moves generation speed.
bool perftTest(bool verbose) {
	if (verbose) {
		cout << "------ Begin Perft Test ------" << endl;
	}

	ChessEngine engine;
	ChessState cs;

	bool passed(true);
	U64 totalDuration(0);

	short i(0);
	for ( ; i<PERFT_TEST_LIST.size(); ++i) {
		if (verbose) {
			cout << "Stage: " << (i+1) << "/" << PERFT_TEST_LIST.size() << endl;
		}

		PERFT_TEST_LIST[i].load(&cs);

		chrono::high_resolution_clock::time_point start(chrono::high_resolution_clock::now());

		passed &= PERFT_TEST_LIST[i].run(&engine, verbose);

		chrono::high_resolution_clock::time_point stop(chrono::high_resolution_clock::now());
		U64 duration(chrono::duration_cast<chrono::microseconds>(stop - start).count());
		totalDuration += duration;

		if (verbose) {
			cout << "Duration: " << int(duration/1000) << " ms" << endl;
			cout << "--------------------" << endl;
		}

		if (!passed) {
			break;
		}
	}

	cout << "Total Duration: " << int(totalDuration/1000) << " ms" << endl;
	
	if (passed) {
		cout << "Test Result: Pass" << endl;
	} else {
		cout << "Test Result: Fail" << endl;
		if (!verbose) {	// Already printed by run()
			cout << "Details:" << endl;
			cout << "    FEN: " << PERFT_TEST_LIST[i].FEN << endl;
			cout << "    Depth: " << short(PERFT_TEST_LIST[i].depth) << endl;
			cout << "    Target: " << PERFT_TEST_LIST[i].target << endl;
		}
	}

	if (verbose) {
		cout << "------- End Perft Test -------" << endl;
	}

	if (passed) {
		return true;
	} else {
		return false;
	}
}
