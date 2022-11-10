from pentago.pentago import Node, Square, Board


def test_encoding():
    board = Board()
    assert str(board) == "x"*36
