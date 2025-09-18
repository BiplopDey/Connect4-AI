import pytest
from fastapi import HTTPException

from server.app import (
    N,
    ai_best_column,
    put_token,
    result_table_ex,
    validate_board,
)


def make_board():
    return [[0 for _ in range(N)] for _ in range(N)]


def test_validate_board_rejects_bad_shape():
    bad_board = [[0 for _ in range(N)] for _ in range(N - 1)]
    with pytest.raises(HTTPException):
        validate_board(bad_board)


def test_put_token_stacks_from_bottom():
    board = make_board()
    bottom_index = put_token(board, 2, 1)
    assert bottom_index == N - 1
    assert board[N - 1][2] == 1

    second_index = put_token(board, 2, 2)
    assert second_index == N - 2
    assert board[N - 2][2] == 2


def test_result_table_ex_detects_horizontal_win():
    board = make_board()
    row = N - 1
    for col in range(4):
        board[row][col] = 1

    result, line = result_table_ex(board)
    assert result == 1
    assert line == [[row, col] for col in range(4)]


def test_ai_best_column_prefers_immediate_win():
    board = make_board()
    row = N - 1
    for col in range(3):
        board[row][col] = 2

    choice = ai_best_column(board)
    assert choice == 3


def test_ai_best_column_calls_engine_when_needed(monkeypatch):
    board = make_board()
    call_args = {}

    def fake_exists(path):
        return True

    class DummyCompletedProcess:
        returncode = 0
        stdout = b"4\n"
        stderr = b""

    def fake_run(cmd, input=None, capture_output=None, **kwargs):
        call_args["cmd"] = cmd
        call_args["input"] = input
        return DummyCompletedProcess()

    monkeypatch.setattr("server.app.os.path.exists", fake_exists)
    monkeypatch.setenv("CONNECT4_ENGINE", "/tmp/engine")
    monkeypatch.setattr("server.app.subprocess.run", fake_run)

    choice = ai_best_column(board)

    assert choice == 4
    assert call_args["cmd"] == ["/tmp/engine"]
    assert call_args["input"].decode().strip() != ""
