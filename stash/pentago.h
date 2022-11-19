#include <iostream>
#include <vector>
#include <iterator>
#include <string>


struct Move {
	int color;
	int i;
	int square;
	bool clockwise;
};


class Board {

private:

	int *nodes_;
	std::vector<Move> moves_;
	int numMoves_;

public:

	Board() {
		nodes_ = new int[36];
		numMoves_ = 0;
	}

	int currentPlayer() {
		if (numMoves_ % 2 == 0) {
			return 1;
		} else {
			return -1;
		}
	}

	bool enoughMovesPlayed() {
		return numMoves_ >= 9;
	}

	int outcome() {
		bool redWin = false;
		bool blackWin = false;

		for (int i = 0; i < 6; i++) {
			int r1 = rowOutcome(i*6);
			int r2 = rowOutcome(i*6+1);
			int c1 = colOutcome(i);
			int c2 = colOutcome(i+6);

			if (!redWin && (r1 == 1 || r2 == 1 || c1 == 1 || c2 == 1)) {
				redWin = true;
			}
			if (!blackWin && (r1 == -1 || r2 == -1 || c1 == -1 || c2 == -1)) {
				blackWin = true;
			}
			if (redWin && blackWin) {
				return 0;
			}
		}

		int d0 = diagOutcome(0, true);
		int d1 = diagOutcome(1, true);
		int d2 = diagOutcome(6, true);
		int d3 = diagOutcome(8, true);
		int d4 = diagOutcome(13, true);
		int d5 = diagOutcome(5, false);
		int d6 = diagOutcome(4, false);
		int d7 = diagOutcome(11, false);
		int d8 = diagOutcome(9, false);
		int d9 = diagOutcome(16, false);

		if (!redWin && (d0 == 1 || d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1 || d6 == 1 || d7 == 1 || d8 == 1 || d9 == 1)) {
			redWin = true;
		}
		if (!blackWin && (d0 == -1 || d1 == -1 || d2 == -1 || d3 == -1 || d4 == -1 || d5 == -1 || d6 == -1 || d7 == -1 || d8 == -1 || d9 == -1)) {
			blackWin = true;
		}

		if (redWin && blackWin) {
			return 0;
		} else if (redWin) {
			return 1;
		} else if (blackWin) {
			return -1;
		} else if (full()) {
			return 0;
		} else {
			return -999;
		}
	}

	float heuristicValue() {
		float balance = 0.0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				balance += (float)nodes_[(7+6*i)+j];
			}
		}
		return balance / 16.0;
	}

	void push(Move move) {
		nodes_[move.i] = move.color;
		rotate(move.square, move.clockwise);
		moves_.push_back(move);
		numMoves_++;
	}

	Move pop() {
		Move move = moves_.back();
		moves_.pop_back();
		rotate(move.square, !move.clockwise);
		nodes_[move.i] = 0;
		numMoves_--;
		return move;
	}

	void rotate(int square, bool clockwise) {
		int refIndex;
		if (square == 0) {
			refIndex = 0;
		} else if (square == 1) {
			refIndex = 3;
		} else if (square == 2) {
			refIndex = 18;
		} else {
			refIndex = 21;
		}

		if (clockwise) {
			rotateClockwise(refIndex);
		} else {
			rotateCounterclockwise(refIndex);
		}
	}

	std::vector<Move> legalMoves() {
		std::vector<Move> legalMoves;
		int player = currentPlayer();

		for (int i = 0; i < 36; i++) {
			if (nodes_[i] == 0) {
				for (int j = 0; j < 4; j++) {
					Move m1 = {player, i, j, true};
					Move m2 = {player, i, j, false};
					legalMoves.push_back(m1);
					legalMoves.push_back(m2);
				}
			}
		}

		return legalMoves;
	}

	void print() {
		std::string s = "";

		for (int i = 0; i < 36; i++) {
			if (nodes_[i] == 1) {
				s += "r ";
			} else if (nodes_[i] == -1) {
				s += "b ";
			} else {
				s += ". ";
			}

			if (i % 6 == 5) {
				s += "\n";
			}
		}

		std::cout << s << std::endl;
	}

private:

	void rotateClockwise(int refIndex) {
		int *nodes = new int[36];

		for (int i = 0; i < 36; i++) {
			nodes[i] = nodes_[i];
		}

		nodes[refIndex] = nodes_[refIndex+12];
        nodes[refIndex+1] = nodes_[refIndex+6];
        nodes[refIndex+2] = nodes_[refIndex];
        nodes[refIndex+6] = nodes_[refIndex+13];
        nodes[refIndex+8] = nodes_[refIndex+1];
        nodes[refIndex+12] = nodes_[refIndex+14];
        nodes[refIndex+13] = nodes_[refIndex+8];
        nodes[refIndex+14] = nodes_[refIndex+2];

        delete[] nodes_;
        nodes_ = nodes;
	}

	void rotateCounterclockwise(int refIndex) {
		int *nodes = new int[36];

		for (int i = 0; i < 36; i++) {
			nodes[i] = nodes_[i];
		}

		nodes[refIndex] = nodes_[refIndex+2];
        nodes[refIndex+1] = nodes_[refIndex+8];
        nodes[refIndex+2] = nodes_[refIndex+14];
        nodes[refIndex+6] = nodes_[refIndex+1];
        nodes[refIndex+8] = nodes_[refIndex+13];
        nodes[refIndex+12] = nodes_[refIndex];
        nodes[refIndex+13] = nodes_[refIndex+6];
        nodes[refIndex+14] = nodes_[refIndex+12];

        delete[] nodes_;
        nodes_ = nodes;
	}

	int rowOutcome(int startIndex) {
		int firstNode = nodes_[startIndex];

		if (firstNode == 0) {
			return -999;
		}

		for (int i = 1; i <= 4; i++) {
			if (nodes_[startIndex+i] != firstNode) {
				return -999;
			}
		}

		return firstNode;
	}

	int colOutcome(int startIndex) {
		int firstNode = nodes_[startIndex];

		if (firstNode == 0) {
			return -999;
		}

		for (int i = 1; i <= 4; i++) {
			if (nodes_[startIndex+6*i] != firstNode) {
				return -999;
			}
		}

		return firstNode;
	}

	int diagOutcome(int startIndex, bool fromTopLeft) {
		int firstNode = nodes_[startIndex];

		if (firstNode == 0) {
			return -999;
		}

		int factor = 5;
		if (fromTopLeft) {
			factor = 7;
		}

		for (int i = 1; i <= 4; i++) {
			if (nodes_[startIndex+factor*i] != firstNode) {
				return -999;
			}
		}

		return firstNode;
	}

	bool full() {
		for (int i = 0; i < 36; i++) {
			if (nodes_[i] == 0) {
				return false;
			}
		}
		return true;
	}
};
