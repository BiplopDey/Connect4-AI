import subprocess
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[1]
ENGINE_DIR = REPO_ROOT / "engine"
ENGINE_BINARY = ENGINE_DIR / "engine"


@pytest.fixture(scope="session")
def engine_binary() -> Path:
    subprocess.run(["make", "engine"], cwd=ENGINE_DIR, check=True)
    return ENGINE_BINARY


def render_board(board):
    return "\n".join(" ".join(str(cell) for cell in row) for row in board) + "\n"


def test_engine_cli_returns_winning_column(engine_binary: Path):
    board = [[0 for _ in range(7)] for _ in range(7)]
    for col in range(3):
        board[-1][col] = 2
    payload = render_board(board)

    result = subprocess.run(
        [str(engine_binary)], input=payload.encode(), capture_output=True
    )
    stdout = result.stdout.decode()
    stderr = result.stderr.decode()

    assert result.returncode == 0, stderr
    assert stdout.strip() == "3"
    assert stderr == ""


def test_engine_cli_rejects_out_of_range_value(engine_binary: Path):
    board_lines = []
    for row_idx in range(7):
        if row_idx == 0:
            row = [3] + [0] * 6
        else:
            row = [0] * 7
        board_lines.append(row)

    payload = render_board(board_lines)

    result = subprocess.run(
        [str(engine_binary)], input=payload.encode(), capture_output=True
    )

    assert result.returncode == 2
    stderr = result.stderr.decode()
    assert "Invalid cell value 3" in stderr
