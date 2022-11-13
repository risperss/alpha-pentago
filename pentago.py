from string import ascii_lowercase
from typing import Generator


class Square:
    '''
    Square representing 3x3 nodes
    Can be rotated 90 degrees in either direction
    '''
    nodes: list[list[str]]

    def __init__(self):
        self.nodes = [
            [".", ".", "."],
            [".", ".", "."],
            [".", ".", "."],
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
            [self.nodes[0][2], self.nodes[0][1], self.nodes[0][0]],
            [self.nodes[1][2], self.nodes[1][1], self.nodes[1][0]],
            [self.nodes[2][2], self.nodes[2][1], self.nodes[2][0]],
        ]

class Move:
    '''
    colour: r, b
    col: a-f
    row: 0-5
    square: 1-4
    rotation: R, L (clockwise, counterclockwise)
    Move: (colour)(col)(row)-(square)(direction)

    Example encoding: ra0-3L
    '''
    red: bool
    row: int
    col: int
    square: int
    clockwise: bool

    def __init__(
        self,
        red: bool,
        row: int,
        col: int,
        square: int,
        clockwise: bool,
        movestring: str = None,
    ):
        if movestring:
            self.init_from_string(movestring)
        else:
            self.red = red
            self.row = row
            self.col = col
            self.square = square
            self.clockwise = clockwise

    def init_from_string(self, movestring: str):
        ...

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

    def __init__(self):
        self.squares = [
            [Square(), Square()],
            [Square(), Square()],
        ]

    def make_move(self, move: Move):
        ...

    def legal_moves(self, red: bool) -> Generator[Move, None, None]:
        for i, row in enumerate(self.nodes()):
            for j, node in enumerate(row):
                if node == ".":
                    for k in range(4):
                        yield Move(red, i, j, k, True)
                        yield Move(red, i, j, k, False)

    def nodes(self) -> Generator[list[str], None, None]:
        for i in range(2):
                for j in range(3):
                    yield [*self.squares[i][0].nodes[j], *self.squares[i][1].nodes[j]]

    def __str__(self):
        return "\n".join(" ".join(row) for row in self.nodes())

if __name__ == "__main__":
    board = Board()
    print(board)
    for move in board.legal_moves(True):
        print(move)
