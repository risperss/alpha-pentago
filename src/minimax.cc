#include <iostream>
#include <chrono>
#include <unordered_map>
#include "stdlib.h"

#include "position.h"

namespace pen {

std::unordered_map<std::uint64_t, int> lookup;

int minimax(Position position, int depth, int alpha, int beta) {
	GameResult result = GameResult::UNDECIDED;

	if (position.GetMoveCount() >= 9) {
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
			std::cout << "Black to make move #" << position.GetMoveCount()+1 << std::endl;
			std::cout << m.as_string() << std::endl;

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

int value(Position position, int depth = 3) {
	return minimax(position, depth, -2, 2);
}

}

int main(void) {
    pen::PositionHistory history = pen::PositionHistory();
    history.Append(pen::Position(pen::PentagoBoard()));


	return 0;
}