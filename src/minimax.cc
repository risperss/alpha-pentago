#include <iostream>
#include <chrono>
#include <unordered_map>
#include "stdlib.h"

#include "position.h"

namespace pen {

std::unordered_map<std::uint64_t, int> lookup;

double heuristic_value(Position position) {
    return 0.0;
}

double minimax(Position position, int depth, double alpha, double beta) {
	GameResult result = GameResult::UNDECIDED;

	if (position.GetPlyCount() >= 9) {
		result = position.ComputeGameResult();
	}

	if (depth == 0 || result != GameResult::UNDECIDED) {
		if (result == GameResult::WHITE_WON) {
			return 1.0;
		} else if (result == GameResult::DRAW) {
			return 0.0;
		} else if (result == GameResult::BLACK_WON) {
			return -1.0;
		} else {
			return heuristic_value(position);
		}
	}

	if (position.IsBlackToMove()) {
		double minEval = 2.0;
		for ( Move m : position.GetBoard().GenerateLegalMoves() ) {
			Position p = Position(position, m);
			std::uint64_t hash = p.Hash();
			double eval;

			if (lookup.find(hash) != lookup.end()) {
				eval = lookup.find(hash)->second;
			} else {
				eval = minimax(p, depth-1, alpha, beta);
				 lookup[hash] = eval;
			}

			minEval = std::min(minEval, eval);
			beta = std::min(beta, eval);

			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	} else {
		double maxEval = -2.0;
		for ( Move m : position.GetBoard().GenerateLegalMoves() ) {
			Position p = Position(position, m);
			std::uint64_t hash = p.Hash();
			double eval;

			if (lookup.find(hash) != lookup.end()) {
				eval = lookup.find(hash)->second;
			} else {
				eval = minimax(p, depth-1, alpha, beta);
				 lookup[hash] = eval;
			}

			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);

			if (beta <= alpha) {
				break;
			}
		}
		return maxEval;
	}
}

double value(Position position, int depth = 6) {
	return minimax(position, depth, -2, 2);
}

}

int main(void) {
     pen::Position p = pen::Position(pen::PentagoBoard());

     using std::chrono::high_resolution_clock;
     using std::chrono::duration_cast;
     using std::chrono::duration;
     using std::chrono::milliseconds;

     for (int depth = 1; depth <= 9; depth++) {
        auto t1 = high_resolution_clock::now();
        double value = pen::value(p, depth);
        auto t2 = high_resolution_clock::now();

        auto ms_int = duration_cast<milliseconds>(t2 - t1);

     	std::cout << "Depth: " << depth << " Value: " << value << " Duration(ms): " << ms_int.count() << std::endl;

     	pen::lookup.clear();
     }

	return 0;
}