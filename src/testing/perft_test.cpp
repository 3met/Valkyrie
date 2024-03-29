
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
			cout << "Position: " << FEN << '\n';
		}

		U64 result(engine->perft(cs, depth));

		if (verbose) {
			if (result == target) {
				printf("Status: Passed\n");
			} else {
				printf("Status: Failed\n");
				printf("Depth: %d\n", short(depth));
				cout << "Target: " << target << '\n';
				cout << "Result: " << result << '\n';
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
	PerftTestCase("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 5, 193690690),
	PerftTestCase("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 5, 674624),
	PerftTestCase("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5, 15833292),
	PerftTestCase("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5, 89941194),
	PerftTestCase("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 5, 164075551),
	// Mid Game Positions
	// Taken From: https://www.talkchess.com/forum3/viewtopic.php?t=58359&start=2
	PerftTestCase("1kr5/3n4/q3p2p/p2n2p1/PppB1P2/5BP1/1P2Q2P/3R2K1 w - -", 5, 93431288),
	PerftTestCase("1n5k/3q3p/pp1p2pB/5r2/1PP1Qp2/P6P/6P1/2R3K1 w - -", 5, 38942132),
	PerftTestCase("1n6/4bk1r/1p2rp2/pP2pN1p/K1P1N2P/8/P5R1/3R4 w - -", 6, 328845658),
	PerftTestCase("1nr5/1k5r/p3pqp1/3p4/1P1P1PP1/R4N2/3Q1PK1/R7 w - -", 5, 103996088),
	PerftTestCase("1q4k1/5p1p/p1rprnp1/3R4/N1P1P3/1P6/P5PP/3Q1R1K w - -", 5, 88571028),
	PerftTestCase("1qr1k2r/1p2bp2/pBn1p3/P2pPbpp/5P2/2P1QBPP/1P1N3R/R4K2 b k -", 5, 127495691),
	PerftTestCase("1r1b2k1/2r2ppp/p1qp4/3R1NPP/1pn1PQB1/8/PPP3R1/1K6 w - -", 5, 112492381),
	PerftTestCase("1r1qk1nr/p3ppbp/3p2p1/1pp5/2bPP3/4B1P1/2PQNPBP/R2R2K1 w k -", 5, 80254241),
};

// Times perft test cases.
// Returns true if passed, else false.
// Good assesment of moves generation speed.
bool perftTest(bool verbose) {
	if (verbose) {
		printf("------ Begin Perft Test ------\n");
	}

	ChessEngine engine;
	ChessState cs;

	bool passed(true);
	U64 totalDuration(0);

	size_t i(0);
	for ( ; i<PERFT_TEST_LIST.size(); ++i) {
		if (verbose) {
			printf("Stage: %d/%d\n", short(i)+1, short(PERFT_TEST_LIST.size()));
		}

		PERFT_TEST_LIST[i].load(&cs);

		chrono::high_resolution_clock::time_point start(chrono::high_resolution_clock::now());

		passed &= PERFT_TEST_LIST[i].run(&engine, verbose);

		chrono::high_resolution_clock::time_point stop(chrono::high_resolution_clock::now());
		U64 duration(chrono::duration_cast<chrono::microseconds>(stop - start).count());
		totalDuration += duration;

		if (verbose) {
			printf("Duration: %d ms\n", int(duration/1000));
			printf("--------------------\n");
		}

		if (!passed) {
			break;
		}
	}

	printf("Total Duration: %d ms\n", int(totalDuration/1000));
	
	if (passed) {
		printf("Test Result: Pass\n");
	} else {
		printf("Test Result: Fail\n");
		if (!verbose) {	// Already printed by run()
			printf("Details:\n");
			cout << "    FEN: " << PERFT_TEST_LIST[i].FEN << '\n';
			printf("    Depth: %d\n", short(PERFT_TEST_LIST[i].depth));
			cout << "    Target: " << PERFT_TEST_LIST[i].target << '\n';
		}
	}

	if (verbose) {
		printf("------- End Perft Test -------\n");
	}

	if (passed) {
		return true;
	} else {
		return false;
	}
}
