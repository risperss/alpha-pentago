from fastapi import FastAPI, HTTPException

from grn import to_cpp_grn, validate_grn

from pentago import Move, GameResult, Position, Negamax, PositionHistory


app = FastAPI()


@app.get("/")
async def root():
    return {"message": "Hello World"}


@app.get("/positions/{grn}")
async def get_best_move(grn: str, depth: int = 4):
    if not validate_grn(grn):
        raise HTTPException(status_code=400, detail="Invalid grn")
    if depth > 6:
        raise HTTPException(status_code=400, detail="Max allowed search depth is 6")

    cpp_grn = to_cpp_grn(grn)

    negamax = Negamax()
    position = Position(cpp_grn)

    n_return = negamax.best(position, depth)

    move = str(n_return.move)
    value = round(n_return.value, 2)

    return {"move": move, "value": value}
