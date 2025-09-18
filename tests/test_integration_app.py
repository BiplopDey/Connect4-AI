from fastapi.testclient import TestClient

from server.app import N, app


client = TestClient(app)


def make_board():
    return [[0 for _ in range(N)] for _ in range(N)]


def test_new_game_defaults():
    response = client.post("/api/new")
    assert response.status_code == 200
    payload = response.json()
    assert payload["next"] == "human"
    assert len(payload["board"]) == N
    assert all(len(row) == N for row in payload["board"])


def test_new_game_ai_starts():
    response = client.post("/api/new", params={"human_starts": False})
    assert response.status_code == 200
    assert response.json()["next"] == "ai"


def test_human_move_triggers_ai(monkeypatch):
    monkeypatch.setattr("server.app.ai_best_column", lambda board: 1)
    board = make_board()

    response = client.post("/api/human-move", json={"board": board, "column": 0})

    assert response.status_code == 200
    data = response.json()
    assert data["humanRow"] == N - 1
    assert data["aiColumn"] == 1
    assert data["aiRow"] == N - 1
    assert data["board"][N - 1][0] == 1
    assert data["board"][N - 1][1] == 2


def test_human_move_rejects_invalid_column():
    board = make_board()
    response = client.post("/api/human-move", json={"board": board, "column": 9})
    assert response.status_code == 400


def test_human_move_returns_finished_game():
    board = make_board()
    row = N - 1
    for col in range(4):
        board[row][col] = 1

    response = client.post("/api/human-move", json={"board": board, "column": 0})
    assert response.status_code == 200
    data = response.json()
    assert data["result"] == 1
    assert "aiColumn" not in data
