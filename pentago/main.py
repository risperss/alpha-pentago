from fastapi import FastAPI

from pentago import Move, GameResult, Position, Negamax, PositionHistory

from grn import to_cpp_grn

app = FastAPI()


@app.get("/")
async def root():
    return {"message": "Hello World"}


@app.get("/positions/{grn}")
async def get_best_move(grn: str, depth: int = 4):
    cpp_grn = to_cpp_grn(grn)

    negamax = Negamax()
    position = Position(cpp_grn)

    n_return = negamax.best(position, depth)

    move = str(n_return.move)
    value = n_return.value

    return {"move": move, "value": value}
