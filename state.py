import pentago

class State():
    board: pentago.Board

    def __init__(self):
        self.board = pentago.Board()

    def serialize(self):
        ...

    def edges(self):
        return list(self.board.legal_moves())

    def value(self):
        # TODO: add neural net here
        return 0    # communists will be happy, all board positions are equal!

if __name__ == "__main__":
    s = State()
    print(len(s.edges()))
