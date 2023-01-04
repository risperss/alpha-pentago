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
    grn = to_grn(white_pieces, black_pieces)

    assert grn == expected_grn


@pytest.mark.parametrize(
    "white_pieces,black_pieces,expected_grn",
    [
        (0, 0, 'e' * 36),
        (1, 2, "wb" + 'e' * 34),
    ],
)
def test_to_cpp_grn(white_pieces, black_pieces, expected_grn):
    grn = to_grn(white_pieces, black_pieces)
    cpp_grn = to_cpp_grn(grn)

    assert cpp_grn == expected_grn


def test_create_position_from_grn():
    white_pieces = 1
    black_pieces = 2
