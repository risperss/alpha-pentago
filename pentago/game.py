from pentago import Move, GameResult, Position, Negamax, PositionHistory

# TODO: Initializing position from string does not work
EMPTY = "....../....../....../....../....../......"

position_history = PositionHistory()
negamax = Negamax()

color = 1

while position_history.compute_result() == GameResult.UNDECIDED:
    position = position_history.get_last()
    print(position)

    nreturn = negamax.best(position, 3, color)

    assert nreturn.move in position.legal_moves()

    position_history.push(nreturn.move)

    print(f"Value:\t{color * nreturn.value:.04}")
    print(f"Move:\t{nreturn.move}")
    print(f"Nodes:\t{negamax.get_nodes_visited():,}")

    color = -color

print(position_history.get_last())
print(f"Result:\t{position_history.compute_result().name}")
