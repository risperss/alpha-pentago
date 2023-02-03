from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware

from grn import to_cpp_grn, validate_grn

from pentago import Move, GameResult, Position, Negamax, PositionHistory


app = FastAPI()


origins = [
    "*",
]


app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


@app.get("/")
async def root():
    return {"message": "Hello World"}


@app.get("/positions/{grn}")
async def get_best_move(grn: str):
    if not validate_grn(grn):
        raise HTTPException(status_code=400, detail="Invalid grn")

    cpp_grn = to_cpp_grn(grn)

    negamax = Negamax()
    position = Position(cpp_grn)

    n_return = negamax.best(position, 3)

    move = str(n_return.move)
    value = round(n_return.value, 2)

    return {"move": move, "value": value}
