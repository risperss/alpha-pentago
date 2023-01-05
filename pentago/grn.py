"""
Encodes a given position into a fen-like string.
Intended for use in the eventual REST API.
"""


def _empty_count(empty_count: int) -> str:
    grn = ""

    grn += str(empty_count % 6) if empty_count % 6 else ""
    grn += '6' * (empty_count // 6)

    return grn


def to_grn(white_pieces: int, black_pieces: int) -> str:
    """Turns a given bitboard position into a string"""

    grn = ""

    white_pieces = f"{white_pieces:036b}"[::-1]
    black_pieces = f"{black_pieces:036b}"[::-1]

    empty_count = 0

    for w_char, b_char in zip(white_pieces, black_pieces):
        if w_char == '0' and b_char == '0':
            empty_count += 1
        else:
            grn += _empty_count(empty_count)
            empty_count = 0

            if w_char == '1':
                grn += 'w'
            else:
                grn += 'b'

    if empty_count:
        grn += _empty_count(empty_count)

    return grn


# TODO: fix tests
def to_cpp_grn(grn: str) -> str:
    """Solely to allow the code in c++ to be simpler."""
    if not grn:
        grn = to_grn(white_pieces, black_pieces)

    numbers = [str(num) for num in range(1, 37)]
    cpp_grn = ""

    for char in grn:
        if char in numbers:
            cpp_grn += int(char) * 'e'
        else:
            cpp_grn += char

    return cpp_grn


def validate_cpp_grn(grn: str):
    ...
