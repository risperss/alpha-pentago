from dataclasses import dataclass
from enum import Enum
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
        self.num_moves = 0

    @property
    def current_player(self):
        return self.num_moves % 2 == 0 # Red plays first

    def outcome(self):
        full = True
        # Diagonals
        parts = [
            self.nodes[0:29:7],
            self.nodes[7:36:7],
            self.nodes[6:35:7],
            self.nodes[1:30:7],
            self.nodes[5:26:5],
            self.nodes[10:31:5],
            self.nodes[4:25:5],
            self.nodes[11:32:5],
        ]
        for i in range(6):
            hstart = 6*i
            parts += [
                self.nodes[hstart:hstart+5],
                self.nodes[hstart+1:hstart+6],
                self.nodes[i:i+30:6],
                self.nodes[i+6:i+31:6],
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
        self.nodes[move.i] = None
        self.num_moves -= 1
        return move

    def push(self, move: Move, human_move=False):
        if human_move and self.nodes[move.i] is not None:
            raise ValueError("Move is not legal")
            
        self.nodes[move.i] = move.red
        self.rotate(move.square, move.clockwise)
        self.moves.append(move)
        self.num_moves += 1

    # TODO: Write rotate methods for 1d board array
    def rotate(self, square: int, clockwise: bool):
        if square == 0:
            refnode = 0
        elif square == 1:
            refnode = 3
        elif square == 2:
            refnode = 18
        else:
            refnode = 21

        if clockwise:
            self._rotate_clockwise(refnode)
        else:
            self._rotate_counterclockwise(refnode)

    def _rotate_clockwise(self, refnode: int):
        copynodes = self.nodes.copy()

        copynodes[refnode] = self.nodes[refnode+12]
        copynodes[refnode+1] = self.nodes[refnode+6]
        copynodes[refnode+2] = self.nodes[refnode]
        copynodes[refnode+6] = self.nodes[refnode+13]
        copynodes[refnode+8] = self.nodes[refnode+1]
        copynodes[refnode+12] = self.nodes[refnode+14]
        copynodes[refnode+13] = self.nodes[refnode+8]
        copynodes[refnode+14] = self.nodes[refnode+2]

        self.nodes = copynodes

    def _rotate_counterclockwise(self, refnode: int):
        copynodes = self.nodes.copy()

        # TODO: there is a clear pattern here that I'm too lazy to implement with an algo
        copynodes[refnode] = self.nodes[refnode+2]
        copynodes[refnode+1] = self.nodes[refnode+8]
        copynodes[refnode+2] = self.nodes[refnode+14]
        copynodes[refnode+6] = self.nodes[refnode+1]
        copynodes[refnode+8] = self.nodes[refnode+13]
        copynodes[refnode+12] = self.nodes[refnode]
        copynodes[refnode+13] = self.nodes[refnode+6]
        copynodes[refnode+14] = self.nodes[refnode+12]

        self.nodes = copynodes

    def legal_moves(self) -> Generator[Move, None, None]:
        for i in range(36):
            if self.nodes[i] == None:
                for j in range(4):
                    yield Move(self.current_player, i, j, True)
                    yield Move(self.current_player, i, j, False)

    def __str__(self):
        s = ""
        for i in range(36):
            s += self.NODE_STR[self.nodes[i]] + " "
            if i % 6 == 5:
                s += "\n"
        return s

if __name__ == "__main__":
    board = Board()
    board.push(Move(True, 21, 2, True))
    board.push(Move(False, 21, 2, True))
    board.push(Move(True, 21, 2, False))
    print(board)
