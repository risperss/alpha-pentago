from pentago import Board
import random


class State():
    board: Board

    def __init__(self):
        self.board = Board()

    def serialize(self):
        ...

    def edges(self):
        return self.board.legal_moves()

    def value(self):
        return self.minimax(36, self.board.current_player)

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

    def minimax(self, depth, red, alpha=-2, beta=2):
        outcome = self.board.outcome()
        if depth == 0 or outcome is not None:
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
            for move in self.edges():
                self.board.push(move)
                eval = self.minimax(depth-1, alpha, beta, not red)
                self.board.pop()
                max_eval = max(max_eval, eval)
                alpha = max(alpha, eval)
                if beta <= alpha:
                    break
            return max_eval
        else:
            min_eval = 2
            for move in self.edges():
                self.board.push(move)
                eval = self.minimax(depth-1, alpha, beta, not red)
                self.board.pop()
                min_eval = min(min_eval, eval)
                beta = min(beta, eval)
                if beta <= alpha:
                    break
            return min_eval


if __name__ == "__main__":
    s = State()
    print(s.value())
