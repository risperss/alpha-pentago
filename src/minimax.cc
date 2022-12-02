#include <iostream>
#include <chrono>
#include <unordered_map>
#include "stdlib.h"

#include "position.h"

namespace pen {

std::unordered_map<std::uint64_t, int> lookup;

int minimax(Position position, int depth, int alpha, int beta) {
	GameResult result = GameResult::UNDECIDED;

	if (position.GetPlyCount() >= 9) {
		result = position.ComputeGameResult();
	}

	if (depth == 0 || result != GameResult::UNDECIDED) {
		if (result == GameResult::WHITE_WON) {
			return 1;
		} else if (result == GameResult::DRAW) {
			return 0;
		} else if (result == GameResult::BLACK_WON) {
			return -1;
		} else {
			// TODO: implement heuristic value function
			return 0;
		}
	}

	if (position.IsBlackToMove()) {
		int minEval = 2;
		for ( Move m : position.GetBoard().GenerateLegalMoves() ) {
			Position p = Position(position, m);
			std::uint64_t hash = p.Hash();
			int eval;

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
		int maxEval = -2;
		for ( Move m : position.GetBoard().GenerateLegalMoves() ) {
			Position p = Position(position, m);
			std::uint64_t hash = p.Hash();
			int eval;

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

int value(Position position, int depth = 6) {
	return minimax(position, depth, -2, 2);
}

}

int main(void) {
     pen::Position p = pen::Position(pen::PentagoBoard());

     using std::chrono::high_resolution_clock;
     using std::chrono::duration_cast;
     using std::chrono::duration;
     using std::chrono::milliseconds;

     for (int depth = 1; depth <= 6; depth++) {
        auto t1 = high_resolution_clock::now();
        int value = pen::value(p, depth);
        auto t2 = high_resolution_clock::now();

        auto ms_int = duration_cast<milliseconds>(t2 - t1);

     	std::cout << "Depth: " << depth << " Value: " << value << " Duration(ms): " << ms_int.count() << std::endl;

     	pen::lookup.clear();
     }

	return 0;
}