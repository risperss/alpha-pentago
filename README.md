# alpha-pentago

## Zero Knowledge Pentago Engine

- Establish the search tree
- Use the neural net to prune the search tree

## Definition: Value Network
- ``` V = f(Board) ```

## State(Board):

### Pieces(3):
- Blank
- Red
- Black

-- 6x6x3 = 108 bits (vector of 0 or 1)

## Rules:
### Starting:
> Start with an empty board  and decide who goes first and who's playing what colour.
- In our game red will always go first
### Object:
> The object is to get five marbles in a row, in any direction, anywhere on the board, before your opponent does,
### Playing:
> Each turn consists of placing one marble anywhere on the board and twisting any of the game blocks 90 degrees, in either direction. You can place your marble on one game block and twist any other game block.
### Winning:
> First to five in a row wins!

