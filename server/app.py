from fastapi import FastAPI, HTTPException
from fastapi.responses import JSONResponse, FileResponse
from fastapi.staticfiles import StaticFiles
import subprocess
import os
from typing import List, Literal, Optional, Tuple

N = 7  # must match engine/board.h

app = FastAPI(title="Connect4 AI Server")

static_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "web"))
app.mount("/static", StaticFiles(directory=static_dir), name="static")


def resolve_engine_path() -> str:
    """Return absolute path to the compiled engine binary."""
    engine_path = os.environ.get(
        "CONNECT4_ENGINE",
        os.path.join(os.path.dirname(__file__), "..", "engine", "engine"),
    )
    return os.path.abspath(engine_path)


def validate_board(board: List[List[int]]):
    if not isinstance(board, list) or len(board) != N:
        raise HTTPException(status_code=400, detail="Board must be a 7x7 array")
    for row in board:
        if not isinstance(row, list) or len(row) != N:
            raise HTTPException(status_code=400, detail="Board must be a 7x7 array")
        for v in row:
            if v not in (0, 1, 2):
                raise HTTPException(status_code=400, detail="Board cells must be 0,1,2")


def put_token(board: List[List[int]], column: int, player: int) -> int:
    """Place a token for player in column. Returns row index or -1 if invalid/full."""
    if column < 0 or column >= N:
        return -1
    for i in range(N - 1, -1, -1):
        if board[i][column] == 0:
            board[i][column] = player
            return i
    return -1


def check_line(a1, a2, a3, a4):
    return (a1 == 1 and a2 == 1 and a3 == 1 and a4 == 1) or (
        a1 == 2 and a2 == 2 and a3 == 2 and a4 == 2
    )


def table_full(board: List[List[int]]):
    for j in range(N):
        if board[0][j] == 0:
            return False
    return True


def result_table_ex(board: List[List[int]]) -> Tuple[Literal[1,2,3,4], Optional[List[List[int]]]]:
    # returns (result, winning_line) where winning_line is 4 [row,col] pairs when result in {1,2}
    for j in range(N):
        for i in range(N - 3):
            # vertical
            if check_line(board[i][j], board[i + 1][j], board[i + 2][j], board[i + 3][j]):
                return board[i][j], [[i + k, j] for k in range(4)]
            # horizontal
            if check_line(board[j][i], board[j][i + 1], board[j][i + 2], board[j][i + 3]):
                return board[j][i], [[j, i + k] for k in range(4)]
    # diag down-right
    for i in range(N - 3):
        for j in range(N - 3):
            if check_line(board[i][j], board[i + 1][j + 1], board[i + 2][j + 2], board[i + 3][j + 3]):
                return board[i][j], [[i + k, j + k] for k in range(4)]
    # diag up-right
    for i in range(3, N):
        for j in range(N - 3):
            if check_line(board[i][j], board[i - 1][j + 1], board[i - 2][j + 2], board[i - 3][j + 3]):
                return board[i][j], [[i - k, j + k] for k in range(4)]
    if table_full(board):
        return 3, None
    return 4, None


def result_table(board: List[List[int]]) -> Literal[1, 2, 3, 4]:
    r, _ = result_table_ex(board)
    return r


def ai_best_column(board: List[List[int]]) -> int:
    """Return best AI column (player=2).

    First, do a fast tactical check: if any legal column wins immediately for AI,
    play it. Otherwise, defer to the compiled engine (minimax) for selection.
    """
    # 1) Immediate winning move check
    for col in range(N):
        if board[0][col] != 0:
            continue  # column full
        b = [row[:] for row in board]
        if put_token(b, col, 2) == -1:
            continue
        res, _ = result_table_ex(b)
        if res == 2:
            return col

    # 2) Fall back to engine
    engine_path = resolve_engine_path()
    if not os.path.exists(engine_path):
        raise RuntimeError("engine binary not found: %s" % engine_path)
    # Prepare stdin payload: 7 lines of 7 ints
    lines = [" ".join(str(v) for v in row) for row in board]
    payload = "\n".join(lines) + "\n"
    res = subprocess.run([engine_path], input=payload.encode(), capture_output=True)
    if res.returncode != 0:
        raise RuntimeError(f"engine error: {res.stderr.decode(errors='ignore')}")
    out = res.stdout.decode().strip()
    try:
        return int(out)
    except ValueError:
        raise RuntimeError(f"invalid engine output: {out}")


@app.post("/api/new")
def new_game(human_starts: bool = True):
    board = [[0 for _ in range(N)] for _ in range(N)]
    next_player = "human" if human_starts else "ai"
    return JSONResponse({"board": board, "next": next_player})


@app.post("/api/human-move")
def human_move(payload: dict):
    board = payload.get("board")
    column = payload.get("column")
    validate_board(board)
    if not isinstance(column, int):
        raise HTTPException(status_code=400, detail="column must be int")

    # If game is already finished, just return final state
    r0, line0 = result_table_ex(board)
    if r0 != 4:
        return {"board": board, "result": r0, "winLine": line0}

    # Apply human move
    human_row = put_token(board, column, 1)
    if human_row == -1:
        raise HTTPException(status_code=400, detail="Invalid or full column")

    r, line = result_table_ex(board)
    if r != 4:
        return {"board": board, "result": r, "winLine": line, "humanRow": human_row}

    # AI move
    ai_col = ai_best_column(board)
    ai_row = put_token(board, ai_col, 2)
    r2, line2 = result_table_ex(board)
    return {
        "board": board,
        "humanRow": human_row,
        "aiColumn": ai_col,
        "aiRow": ai_row,
        "result": r2,
        "winLine": line2,
    }


@app.get("/health")
def health():
    engine_path = resolve_engine_path()
    if not os.path.exists(engine_path):
        raise HTTPException(status_code=503, detail="engine binary not found")
    return {"status": "ok", "enginePath": engine_path}


@app.get("/")
def index():
    index_path = os.path.join(static_dir, "index.html")
    return FileResponse(index_path)
