import pytest

from server.app import N
from server.domain.board import BoardState
from server.services.ai import AIMoveSelector


class StubEngine:
    def __init__(self, column: int):
        self.column = column
        self.calls = 0

    def choose_column(self, board: BoardState) -> int:
        self.calls += 1
        return self.column


def make_board_state() -> BoardState:
    return BoardState.empty(N)


def make_board():
    return make_board_state().grid


def test_validate_board_rejects_bad_shape():
    bad_board = [[0 for _ in range(N)] for _ in range(N - 1)]
    with pytest.raises(ValueError):
        BoardState.validate(bad_board, size=N)


def test_put_token_stacks_from_bottom():
    state = make_board_state()
    board = state.grid
    bottom_index = state.drop_token(2, 1)
    assert bottom_index == N - 1
    assert board[N - 1][2] == 1

    second_index = state.drop_token(2, 2)
    assert second_index == N - 2
    assert board[N - 2][2] == 2


def test_result_table_ex_detects_horizontal_win():
    state = make_board_state()
    grid = state.grid
    row = N - 1
    for col in range(4):
        grid[row][col] = 1

    report = state.analyze()
    assert report.result == 1
    assert report.line == [[row, col] for col in range(4)]


def test_ai_best_column_prefers_immediate_win():
    state = make_board_state()
    grid = state.grid
    row = N - 1
    for col in range(3):
        grid[row][col] = 2

    selector = AIMoveSelector(engine=StubEngine(column=5))
    choice = selector.select_move(state)
    assert choice == 3


def test_ai_best_column_calls_engine_when_needed(monkeypatch):
    state = make_board_state()
    engine = StubEngine(column=4)
    selector = AIMoveSelector(engine=engine)

    choice = selector.select_move(state)

    assert choice == 4
    assert engine.calls == 1
