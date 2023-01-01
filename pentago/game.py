import re

from pentago import Move, GameResult, Position, Negamax

EMPTY = "....../....../....../....../....../......"

position = Position(EMPTY)
negamax = Negamax()

print(position)

nreturn = negamax.best(position, 5, -1)

print(nreturn.value)
print(nreturn.move)
print(negamax.get_nodes_visited())
