import re

from alpha_pentago import Game, Move, GameResult

MAX_SEARCH_DEPTH = 4

game = Game()
game.set_max_search_depth(MAX_SEARCH_DEPTH)
computer_to_play = False

human_game = False

while game.compute_result() == GameResult.UNDECIDED:
    print(game)

    if human_game:
        if computer_to_play:
            move = game.best_move()
        else:
            while True:
                while True:
                    user_input = input("Enter your move: ")

                    if re.fullmatch(r"[a-f][1-6]-[1-4][L|R]", user_input):
                        break
                    else:
                        print("Move string is improperly formatted")

                move = Move(user_input)

                if move in game.legal_moves():
                    break
                else:
                    print("Move is not legal")
    else:
        move = game.best_move()

    game.push(move)
    computer_to_play = not computer_to_play

print(game)
print(game.compute_result().name)
