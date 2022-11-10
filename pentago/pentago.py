from dataclasses import dataclass


@dataclass
class Node:
    white: bool = None

    def __str__(self):
        if self.white is None:
            return "x"
        elif self.white:
            return "w"
        else:
            return "b"


class Square:
    nodes: list[list[Node]]

    def __init__(self):
        self.nodes = [
            [Node(), Node(), Node()],
            [Node(), Node(), Node()],
            [Node(), Node(), Node()],
        ]

    def rotate_clockwise(self):
        self.nodes = [
            [self.nodes[2][0], self.nodes[1][0], self.nodes[0][0]],
            [self.nodes[2][1], self.nodes[1][1], self.nodes[0][1]],
            [self.nodes[2][2], self.nodes[1][2], self.nodes[0][2]],
        ]

    def rotate_counterclockwise(self):
        self.nodes = [
            [self.nodes[0][2], self.nodes[0][1], self.nodes[0][0]],
            [self.nodes[1][2], self.nodes[1][1], self.nodes[1][0]],
            [self.nodes[2][2], self.nodes[2][1], self.nodes[2][0]],
        ]

    def __str__(self):
        s = ""
        for row in self.nodes:
            for node in row:
                s += str(node)
        return s


class Board:
    squares: list[list[Square]]

    def __init__(self):
        self.squares = [
            [Square(), Square()],
            [Square(), Square()],
        ]

    def __str__(self):
        s = ""
        for row in self.squares:
            for square in row:
                s += str(square)
        return s
