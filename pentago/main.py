from fastapi import FastAPI, HTTPException

from pentago import Move, GameResult, Position, Negamax, PositionHistory

from grn import to_cpp_grn, validate_grn

app = FastAPI()


@app.get("/")
async def root():
    return {"message": "Hello World"}


@app.get("/positions/{grn}")
async def get_best_move(grn: str, depth: int = 4):
    if not validate_grn(grn):
        raise HTTPException(status_code=400, detail="Invalid grn")

    cpp_grn = to_cpp_grn(grn)

    negamax = Negamax()
    position = Position(cpp_grn)

    n_return = negamax.best(position, depth)

    move = str(n_return.move)
    value = n_return.value
    value = round(value, 2)

    return {"move": move, "value": value}
