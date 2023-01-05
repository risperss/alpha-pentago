import pytest

from pentago import Move, GameResult, Position, Negamax, PositionHistory

from grn import to_grn, to_cpp_grn


def test_legal_moves():
    position_history = PositionHistory()

    for i in range(36):
        legal_moves = position_history.get_last().legal_moves()

        assert len(legal_moves) == 288 - 8 * i

        position_history.push(legal_moves[0])


@pytest.mark.parametrize(
    "white_pieces,black_pieces,expected_grn",
    [
        (0, 0, "666666"),
        (1, 2, "wb466666"),
    ],
)
def test_to_grn(white_pieces, black_pieces, expected_grn):
    assert to_grn(white_pieces, black_pieces) == expected_grn


@pytest.mark.parametrize(
    "white_pieces,black_pieces,expected_grn",
    [
        (0, 0, "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
        (1, 2, "wbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"),
    ],
)
def test_to_cpp_grn(white_pieces, black_pieces, expected_grn):
    assert to_cpp_grn(white_pieces, black_pieces) == expected_grn


def test_create_position_from_grn():
    white_pieces = 1
    black_pieces = 2

    cpp_grn = to_cpp_grn(white_pieces, black_pieces)

    position = Position(cpp_grn)

    assert position.white_pieces() == white_pieces
    assert position.black_pieces() == black_pieces
