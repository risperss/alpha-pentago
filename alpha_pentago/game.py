import time

from pentago import Move, GameResult, Position, Negamax, PositionHistory

position_history = PositionHistory()
negamax = Negamax()

white = True

while position_history.compute_result() == GameResult.UNDECIDED:
    position = position_history.get_last()

    start = time.time()
    nreturn = negamax.best(position, 3)
    end = time.time()

    elapsed = end - start

    if nreturn.move not in (legal_moves := position.legal_moves()):
        s = f"Illegal Move {nreturn.move}"
        raise ValueError(s)

    position_history.push(nreturn.move)

    to_move = "White" if white else "Black"

    print(f"Player:\t{to_move}")
    print(f"Value:\t{nreturn.value:.04}")
    print(f"Move:\t{nreturn.move}")
    print(f"Nodes:\t{negamax.get_nodes_visited():,}")
    print(f"Time:\t{elapsed:.04}s")
    print(f"NPS:\t{int(negamax.get_nodes_visited()/elapsed):,}")
    print(position)

    white = not white

print(position_history.get_last())
print(f"Result:\t{position_history.compute_result().name}")
