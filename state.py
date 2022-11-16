from pentago import Board
import random
from timeit import default_timer

class State():
    board: Board
    checks: int

    def __init__(self):
        self.board = Board()
        self.checks = 0

    def serialize(self):
        ...

    def edges(self):
        return self.board.legal_moves()

    def value(self, d: int):
        self.checks = 0
        return self.minimax(d, self.board.current_player, -2, 2)

    def heuristic_value(self):
        # First simple solution: Computing percentage of 16 central squares occupied
        rednodes = 0
        blacknodes = 0

        for i in range(4):
            for j in range(4):
                node = self.board.nodes[(7+6*i)+j]
                if node == True:
                    rednodes += 1
                elif node == False:
                    blacknodes += 1

        return (rednodes - blacknodes) / 16

    def minimax(self, depth, red, alpha, beta):
        outcome = self.board.outcome()
        if depth == 0 or outcome is not None:
            self.checks += 1
            if outcome == Board.Outcome.RED_WIN:
                return 1
            elif outcome == Board.Outcome.BLACK_WIN:
                return -1
            elif outcome == Board.Outcome.DRAW:
                return 0
            else:
                return self.heuristic_value()

        if red:
            max_eval = -2
            for move in self.board.legal_moves():
                self.board.push(move)
                eval_ = self.minimax(depth-1, not red, alpha, beta)
                self.board.pop()
                max_eval = max(max_eval, eval_)
                alpha = max(alpha, eval_)
                if beta <= alpha:
                    break
            return max_eval
        else:
            min_eval = 2
            for move in self.board.legal_moves():
                self.board.push(move)
                eval_ = self.minimax(depth-1, not red, alpha, beta)
                self.board.pop()
                min_eval = min(min_eval, eval_)
                beta = min(beta, eval_)
                if beta <= alpha:
                    break
            return min_eval


if __name__ == "__main__":
    s = State()
    # Can check to see if I am improving the algorithm
    for i in range(1, 5):
        start = default_timer()
        val = s.value(i)
        end = default_timer()
        print(f"d={i}, time={(end-start):.3f}, value={val}, checks={s.checks}")
