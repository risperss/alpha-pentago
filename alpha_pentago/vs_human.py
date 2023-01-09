import re

from pentago import Move, GameResult, Position, Negamax, PositionHistory


position_history = PositionHistory()
negamax = Negamax()


def get_human_move():
  while True:
    move = input("Enter a move: ")

    if not re.fullmatch(r"[a-f][1-6]-[1-4][L|R]", move):
        continue

    move = Move(move)

    if move not in position_history.get_last().legal_moves():
        print(f"Illegal Move: {move}")
        continue

    return move


def get_cpu_move():
  nreturn = negamax.best(position_history.get_last(), 4)

  if nreturn.move not in position_history.get_last().legal_moves():
        s = f"Illegal CPU Move {nreturn.move}"
        raise ValueError(s)

  return nreturn.move


def human_vs_cpu_game():
    human = True

    while position_history.compute_result() == GameResult.UNDECIDED:
        print(position_history.get_last())

        move = get_human_move() if human else get_cpu_move()
        print(move)
        position_history.push(move)

        human = not human

    print(position_history.get_last())
    print(f"Result:\t{position_history.compute_result().name}")

if __name__ == "__main__":
    human_vs_cpu_game()
