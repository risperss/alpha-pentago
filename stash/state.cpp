#include "pentago.h"
#include <algorithm>


float minimax(Board* board, int depth, int color, int alpha, int beta) {
	int outcome = -999;
	if (board->enoughMovesPlayed()) {
		outcome = board->outcome();
	}

	if (outcome != -999) {
		return outcome;
	} else if (depth == 0) {
		return board->heuristicValue();
	}

	if (color == 1) {
		int maxEval = -2;

		for (Move move : board->legalMoves()) {
			board->push(move);
			int eval = minimax(board, depth-1, -1, alpha, beta);
			board->pop();

			maxEval = std::max(eval, maxEval);
			alpha = std::max(eval, alpha);

			if (beta <= alpha) {
				break;
			}
		}
		return maxEval;
	} else {
		int minEval = 2;

		for (Move move : board->legalMoves()) {
			board->push(move);
			int eval = minimax(board, depth-1, 1, alpha, beta);
			board->pop();

			minEval = std::min(eval, minEval);
			beta = std::min(eval, beta);

			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	}
}

int main(void) {
	Board b = Board();
	int depth = 10;

	std::cout << depth << " " << minimax(&b, depth, 1, -2, 2) << std::endl;

	return 0;
}
