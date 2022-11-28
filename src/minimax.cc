#include <iostream>
#include "stdlib.h"

#include "position.h"

namespace pen {

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
	pen::PositionHistory history = pen::PositionHistory();
	history.Append(pen::Position(pen::PentagoBoard()));

	while (true) {
		pen::MoveList legalMoves = history.Last().GetBoard().GenerateLegalMoves();

		int i = rand() % legalMoves.size();
		pen::Move move = legalMoves[i];

		history.Append(move);

		if (history.Last().ComputeGameResult() != pen::GameResult::UNDECIDED) {
			break;
		}
	}

	int len = history.GetLength();

	for (int i = 0; i < len; i++) {
		std::cout << history.GetPositionAt(i).DebugString() << std::endl;
	}

	return 0;
}