from alpha_pentago import Game, Move, GameResult

game = Game()
game.set_max_search_depth(2)

while game.compute_result() == GameResult.UNDECIDED:
    print(game)

    move = game.best_move()
    game.push(move)

print(game)
print(game.compute_result().name)
