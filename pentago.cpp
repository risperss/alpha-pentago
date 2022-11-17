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
};


int main(void) {
	std::cout << "Hello, world!" << std::endl;
	return 0;
}
