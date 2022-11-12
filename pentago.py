NODE_FORMAT = {
    None: ".",
    True: "r",
    False: "b",
}

class Square:
    nodes: list[list[bool]]

    def __init__(self):
        self.nodes = [
            [None, None, None],
            [None, None, None],
            [None, None, None],
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

    def __str__(self):
        s = ""
        for row in self.nodes:
            for node in row:
                s += NODE_FORMAT[node] + " "
            s += "\n"
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
            s += "\n"
        return s
