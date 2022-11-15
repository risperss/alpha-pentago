from dataclasses import dataclass
from enum import Enum
import random
import re
from string import ascii_lowercase
from typing import Generator


@dataclass
class Move:
    red: bool
    i: int
    square: int
    clockwise: bool

    def from_string(movestring: str):
        if not re.fullmatch(r"[r|b][0-35]-[0-3][R|L]", movestring):
            raise ValueError("Move string format is invalid")

        red, i, _, square, clockwise = list(movestring)

        red = (red == "r")
        square = int(square)
        clockwise = (clockwise == "R")

        return Move(red, i, square, clockwise)

    def __str__(self):
        colour = "r" if self.red else "b"
        col = ascii_lowercase[self.col]
        rotation = "R" if self.clockwise else "L"
        return f"{colour}{col}{self.row}-{self.square}{rotation}"

class Board:
    nodes: list[bool]
    moves: list[Move]
    num_moves: int

    NODE_STR = {
        None: ".",
        True: "r",
        False: "b",
        ".": None,
        "r": True,
        "b": False,
    }

    class Outcome(Enum):
        RED_WIN = 1
        BLACK_WIN = 2
        DRAW = 3

    def __init__(self):
        self.nodes = [None for _ in range(36)]
        self.moves = []

    @property
    def current_player(self):
        return self.num_moves % 2 == 0 # Red plays first

    def heuristic_value(self):
        # For now just a random float from -1 to 1
        return random.choice([-1, 1]) * random.random()

    def outcome(self):
        full = True
        # Diagonals
        parts = [
            self.nodes[0:29:7],
            self.nodes[7:36:7],
            self.nodes[6:35:7],
            self.nodes[1:29:7],
            self.nodes[5:26:5],
            self.nodes[10:31:5],
            self.nodes[4:25:5],
            self.nodes[11:32:5],
        ]
        for i in range(6):
            hstart = 6*i
            parts += [
                self.nodes[hstart:hstart+6],
                self.nodes[i:i+31:6],
            ]

        outcome = None
        red_win = False
        black_win = False

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

        boardstring = boardstring.replace("/", "")
        board = Board()
        board.num_moves = 36 - boardstring.count(".")
        for i, nodestr in enumerate(boarstring): 
            board.nodes[i] = NODE_STR[nodestr]

    def pop(self) -> Move:
        move = self.moves.pop()
        self.rotate(move.square, not move.clockwise)
        nodes[move.i].red = None
        self.num_moves -= 1
        return move

    def push(self, move: Move):
        if move not in self.legal_moves():
            raise ValueError("Move is not legal")

        nodes[i] = move.red
        self.rotate(move.square, move.clockwise)
        self.moves.append(move)
        self.num_moves += 1

    # TODO: Write rotate methods for 1d board array
    def rotate(self, square: int, clockwise: bool):
        if clockwise:
            self._rotate_clockwise()
        else:
            self._rotate_counterclockwise()

    def _rotate_clockwise(self, square: int):
        self.nodes = [
            [self.nodes[2][0], self.nodes[1][0], self.nodes[0][0]],
            [self.nodes[2][1], self.nodes[1][1], self.nodes[0][1]],
            [self.nodes[2][2], self.nodes[1][2], self.nodes[0][2]],
        ]

    def _rotate_counterclockwise(self, square: int):
        self.nodes = [
            [self.nodes[0][2], self.nodes[1][2], self.nodes[2][2]],
            [self.nodes[0][1], self.nodes[1][1], self.nodes[2][1]],
            [self.nodes[0][0], self.nodes[1][0], self.nodes[2][0]],
        ]


    def legal_moves(self) -> Generator[Move, None, None]:
        for i in range(36):
            if self.nodes[i] == None:
                for j in range(4):
                    yield Move(self.current_player, i, k, True)
                    yield Move(self.current_player, i, k, False)

    def __str__(self):
        s = ""
        for i in range(36):
            s += self.NODE_STR[nodes[i]]
            if i % 6 == 5:
                s += "\n"
        return s

def minimax(board: Board, depth, red, alpha=-2, beta=2):
    outcome = board.outcome()
    if depth == 0 or outcome is not None:
        if outcome == Board.Outcome.RED_WIN:
            return 1
        elif outcome == Board.Outcome.BLACK_WIN:
            return -1
        elif outcome == Board.Outcome.DRAW:
            return 0
        else:
            return board.heuristic_value()

    if red:
        max_eval = -2
        for move in board.legal_moves():
            board.push(move)
            eval = minimax(board, depth-1, alpha, beta, not red)
            board.pop()
            max_eval = max(max_eval, eval)
            alpha = max(alpha, eval)
            if beta <= alpha:
                break
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
        return min_eval

if __name__ == "__main__":
    print("Hello, world!")
