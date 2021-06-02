
#include "chess_engine.hpp"
#include "chess_state.hpp"

/* Returns true if passed, else false */
bool perftTest() {
	ChessEngine engine;
	ChessState cs;

	// From: https://www.chessprogramming.org/Perft_Results
	cs.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	if (engine.perft(&cs, 5) != 4865609)
		return false;
	cs.loadFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
	if (engine.perft(&cs, 4) != 4085603)
		return false;
	cs.loadFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
	if (engine.perft(&cs, 4) != 422333)
		return false;
	cs.loadFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
	if (engine.perft(&cs, 5) != 674624)
		return false;
	cs.loadFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
	if (engine.perft(&cs, 4) != 2103487)
		return false;
	cs.loadFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
	if (engine.perft(&cs, 4) != 3894594)
		return false;

	// From: http://talkchess.com/forum3/viewtopic.php?t=47318
	// avoid illegal en passant capture:
	cs.loadFEN("8/5bk1/8/2Pp4/8/1K6/8/8 w - d6 0 1");
	if (engine.perft(&cs, 6) != 824064)
		return false;
	cs.loadFEN("8/8/1k6/8/2pP4/8/5BK1/8 b - d3 0 1");
	if (engine.perft(&cs, 6) != 824064)
		return false;

	// en passant capture checks opponent:
	cs.loadFEN("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1");
	if (engine.perft(&cs, 6) != 1440467)
		return false;
	cs.loadFEN("8/5k2/8/2Pp4/2B5/1K6/8/8 w - d6 0 1");
	if (engine.perft(&cs, 6) != 1440467)
		return false;

	// short castling gives check:
	cs.loadFEN("5k2/8/8/8/8/8/8/4K2R w K - 0 1");
	if (engine.perft(&cs, 6) != 661072)
		return false;
	cs.loadFEN("4k2r/8/8/8/8/8/8/5K2 b k - 0 1");
	if (engine.perft(&cs, 6) != 661072)
		return false;

	// long castling gives check:
	cs.loadFEN("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1");
	if (engine.perft(&cs, 6) != 803711)
		return false;
	cs.loadFEN("r3k3/8/8/8/8/8/8/3K4 b q - 0 1");
	if (engine.perft(&cs, 6) != 803711)
		return false;

	// castling (including losing cr due to rook capture):
	cs.loadFEN("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1");
	if (engine.perft(&cs, 4) != 1274206)
		return false;
	cs.loadFEN("r3k2r/7b/8/8/8/8/1B4BQ/R3K2R b KQkq - 0 1");
	if (engine.perft(&cs, 4) != 1274206)
		return false;

	// castling prevented:
	cs.loadFEN("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1");
	if (engine.perft(&cs, 4) != 1720476)
		return false;
	cs.loadFEN("r3k2r/8/5Q2/8/8/3q4/8/R3K2R w KQkq - 0 1");
	if (engine.perft(&cs, 4) != 1720476)
		return false;

	// promote out of check:
	cs.loadFEN("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1");
	if (engine.perft(&cs, 6) != 3821001)
		return false;
	cs.loadFEN("3K4/8/8/8/8/8/4p3/2k2R2 b - - 0 1");
	if (engine.perft(&cs, 6) != 3821001)
		return false;

	// discovered check:
	cs.loadFEN("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1");
	if (engine.perft(&cs, 5) != 1004658)
		return false;
	cs.loadFEN("5K2/8/1Q6/2N5/8/1p2k3/8/8 w - - 0 1");
	if (engine.perft(&cs, 5) != 1004658)
		return false;

	// promote to give check:
	cs.loadFEN("4k3/1P6/8/8/8/8/K7/8 w - - 0 1");
	if (engine.perft(&cs, 6) != 217342)
		return false;
	cs.loadFEN("8/k7/8/8/8/8/1p6/4K3 b - - 0 1");
	if (engine.perft(&cs, 6) != 217342)
		return false;

	// underpromote to check:
	cs.loadFEN("8/P1k5/K7/8/8/8/8/8 w - - 0 1");
	if (engine.perft(&cs, 6) != 92683)
		return false;
	cs.loadFEN("8/8/8/8/8/k7/p1K5/8 b - - 0 1");
	if (engine.perft(&cs, 6) != 92683)
		return false;

	// self stalemate:
	cs.loadFEN("K1k5/8/P7/8/8/8/8/8 w - - 0 1");
	if (engine.perft(&cs, 6) != 2217)
		return false;
	cs.loadFEN("8/8/8/8/8/p7/8/k1K5 b - - 0 1");
	if (engine.perft(&cs, 6) != 2217)
		return false;

	// stalemate/checkmate:
	cs.loadFEN("8/k1P5/8/1K6/8/8/8/8 w - - 0 1");
	if (engine.perft(&cs, 7) != 567584)
		return false;
	cs.loadFEN("8/8/8/8/1k6/8/K1p5/8 b - - 0 1");
	if (engine.perft(&cs, 7) != 567584)
		return false;

	// double check:
	cs.loadFEN("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1");
	if (engine.perft(&cs, 4) != 23527)
		return false;
	cs.loadFEN("8/5k2/8/5N2/5Q2/2K5/8/8 w - - 0 1");
	if (engine.perft(&cs, 4) != 23527)
		return false;

	return true;
}
