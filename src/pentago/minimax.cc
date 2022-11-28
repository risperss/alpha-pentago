#include <iostream>
#include "stdlib.h"

#include "position.h"

namespace pen {

int minimax(Position position, int depth, int alpha, int beta) {
	GameResult result = GameResult::UNDECIDED;

	if (position.GetMoveCount() >= 9) {
		result = position.ComputeGameResult();
	}

	std::cout << 

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
			int eval = minimax(Position(position, m), depth-1, alpha, beta);

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
			int eval = minimax(Position(position, m), depth-1, alpha, beta);

			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);

			if (beta <= alpha) {
				break;
			}
		}
		return maxEval;
	}
}

}

int main(void) {
	int depth = 3;

	pen::PentagoBoard startingBoard = pen::PentagoBoard();
	pen::Position startPosition = pen::Position(startingBoard);

	std::cout << startPosition.GetMoveCount() << std::endl;

	int value = pen::minimax(startPosition, depth, -2, 2);

	std::cout << value << std::endl;

	return 0;
}