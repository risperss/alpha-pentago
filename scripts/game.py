from alpha_pentago import Game, Move

game = Game()
move = Move("a5-1R")
game.compute_result()
print(move)
