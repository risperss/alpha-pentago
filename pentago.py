from dataclasses import dataclass
from enum import Enum
import random
import re
from string import ascii_lowercase
from typing import Generator


@dataclass
class Node:
    red: bool = None

    def update(self, nodestring: str):
        if nodestring not in ["r", "b", "."]:
            raise ValueError("Node string format is invalid")

        if nodestring == ".":
            self.red = None
        elif nodestring == "r":
            self.red = True
        else:
            self.red = False

    def __str__(self):
        if self.red is None:
            return "."
        elif self.red:
            return "r"
        else:
            return "b"

class Square:
    '''
    Square representing 3x3 nodes.
    Can be rotated 90 degrees in either direction.
    '''
    nodes: list[list[Node]]

    def __init__(self):
        self.nodes = [
            [Node(), Node(), Node()],
            [Node(), Node(), Node()],
            [Node(), Node(), Node()],
        ]

    def rotate(self, clockwise: bool):
        if clockwise:
            self._rotate_clockwise()
        else:
            self._rotate_counterclockwise()

    def _rotate_clockwise(self):
        self.nodes = [
            [self.nodes[2][0], self.nodes[1][0], self.nodes[0][0]],
            [self.nodes[2][1], self.nodes[1][1], self.nodes[0][1]],
            [self.nodes[2][2], self.nodes[1][2], self.nodes[0][2]],
        ]

    def _rotate_counterclockwise(self):
        self.nodes = [
            [self.nodes[0][2], self.nodes[1][2], self.nodes[2][2]],
            [self.nodes[0][1], self.nodes[1][1], self.nodes[2][1]],
            [self.nodes[0][0], self.nodes[1][0], self.nodes[2][0]],
        ]

@dataclass
class Move:
    '''
    colour: r, b
    col: a-f
    row: 0-5
    square: 0-3
    rotation: R, L (clockwise, counterclockwise)
    Move: (colour)(col)(row)-(square)(direction)

    Example encoding: ra0-3L
    '''
    red: bool
    row: int
    col: int
    square: int
    clockwise: bool

    def from_string(movestring: str):
        if not re.fullmatch(r"[r|b][a-f][0-5]-[0-3][R|L]", movestring):
            raise ValueError("Move string format is invalid")

        red, row, col, _, square, clockwise = list(movestring)
        red = (red == "r")
        row = ascii_lowercase.index(row)
        col = int(col)
        square = int(square)
        clockwise = (clockwise == "R")

        return Move(red, row, col, square, clockwise)

    def __str__(self):
        colour = "r" if self.red else "b"
        col = ascii_lowercase[self.col]
        rotation = "R" if self.clockwise else "L"
        return f"{colour}{col}{self.row}-{self.square}{rotation}"

class Board:
    '''
    Board has 6x6 nodes, and 2x2 Squares

       a b c d e f
    0  . . . . . .
    1  . 0 . . 1 .
    2  . . . . . .
    3  . . . . . .
    4  . 2 . . 3 .
    5  . . . . . .
    '''
    squares: list[list[Square]]
    moves: list[Move]

    class Outcome(Enum):
        RED_WIN = 1
        BLACK_WIN = 2
        DRAW = 3

    def __init__(self):
        self.squares = [
            [Square(), Square()],
            [Square(), Square()],
        ]
        self.moves = []

    @property
    def current_player(self):
        return len(self.moves) % 2 == 0 # Red starts

    def outcome(self):
        nodes = [[node.red for node in row] for row in self.nodes()]
        full = True
        # Diagonals
        parts = [
            # Top Left -> Bottom Right
            [nodes[i][i] for i in range(5)],
            [nodes[i][i] for i in range(1, 6)],
            [nodes[i+1][i] for i in range(5)],
            [nodes[i][i+1] for i in range(5)],
            # Bottom Left -> Top Right
            [nodes[5-i][i] for i in range(5)],
            [nodes[5-i][i] for i in range(1, 6)],
            [nodes[4-i][i] for i in range(5)],
            [nodes[5-i][i+1] for i in range(5)],
        ]
        # Horizontal + Vertical
        for i in range(6):
            parts += [
                nodes[i][0:5],
                nodes[i][1:6],
                [nodes[j][i] for j in range(5)],
                [nodes[j][i] for j in range(1, 6)]
            ]

        outcome = None
        red_win = False
        black_win = False
        # Checking for win
        for part in parts:
            if None not in part:
                if all(part):
                    red_win = True
                    outcome = Board.Outcome.RED_WIN
                elif not any(part):
                    black_win = True
                    outcome = Board.Outcome.BLACK_WIN
            else:
                full = False

        if full or (red_win and black_win):
            return Board.Outcome.DRAW
        else:
            return outcome

    def from_string(boardstring: str):
        if not re.fullmatch(r"([r|b|.]{6}\/){5}[r|b|.]{6}", boardstring):
            raise ValueError("Board string format is invalid")

        boardstring = [list(x) for x in boardstring.split("/")]
        board = Board()
        for i, row in enumerate(board.nodes()):
            for j, node in enumerate(row):
                node.update(boardstring[i][j])

        return board

    def pop(self) -> Move:
        move = self.moves.pop()
        square_row = move.square // 2
        square_col = move.square % 2
        self.squares[square_row][square_col].rotate(not move.clockwise)
        nodes = list(self.nodes())
        nodes[move.row][move.col].red = None

        return move

    def push(self, move: Move):
        if move not in self.legal_moves():
            raise ValueError("Move is not legal")

        self.moves.append(move)
        square_row = move.square // 2
        square_col = move.square % 2
        nodes = list(self.nodes())
        nodes[move.row][move.col].red = move.red
        self.squares[square_row][square_col].rotate(move.clockwise)

    def legal_moves(self) -> Generator[Move, None, None]:
        red = self.current_player
        for i, row in enumerate(self.nodes()):
            for j, node in enumerate(row):
                if node.red is None:
                    for k in range(4):
                        yield Move(red, i, j, k, True)
                        yield Move(red, i, j, k, False)

    def nodes(self) -> Generator[list[Node], None, None]:
        for i in range(2):
                for j in range(3):
                    yield [*self.squares[i][0].nodes[j], *self.squares[i][1].nodes[j]]

    def __str__(self):
        return "\n".join(" ".join([str(node) for node in row]) for row in self.nodes())

@dataclass
class Foo:
    NODES: int = 0

foo = Foo()

def minimax(board, depth, alpha, beta, red):
    outcome = board.outcome()
    if depth == 0 or outcome is not None:
        foo.NODES += 1
        print(foo.NODES)
        if outcome == Board.Outcome.RED_WIN:
            return 1
        elif outcome == Board.Outcome.BLACK_WIN:
            return -1
        else:
            return 0

    if red: # We'll pick red as the maximixing player
        max_eval = -2
        for move in board.legal_moves():
            board.push(move)
            eval = minimax(board, depth-1, alpha, beta, not red)
            board.pop()
            max_eval = max(max_eval, eval)
            alpha = max(alpha, eval)
            if beta <= alpha:
                break
        foo.NODES += 1
        print(foo.NODES)
        return max_eval
    else:
        min_eval = 2
        for move in board.legal_moves():
            board.push(move)
            eval = minimax(board, depth-1, alpha, beta, not red)
            board.pop()
            min_eval = min(min_eval, eval)
            beta = min(beta, eval)
            if beta <= alpha:
                break
        foo.NODES += 1
        print(foo.NODES)
        return min_eval

if __name__ == "__main__":
    board = Board.from_string("rrrr../bbbb../....../....../....../......")
    print(minimax(board, 3, -2, 2, board.current_player))
