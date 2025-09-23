from fastapi.testclient import TestClient

from server.app import app


client = TestClient(app)


def test_index_markup_includes_tailwind_and_board():
    response = client.get("/")
    assert response.status_code == 200

    html = response.text
    assert "<!doctype html>" in html.lower()
    assert "cdn.tailwindcss.com" in html
    assert "id=\"board\"" in html
    assert "script src=\"/static/app.js\"" in html
    assert "bg-token-human" in html


def test_static_app_js_served():
    response = client.get("/static/app.js")
    assert response.status_code == 200
    body = response.text
    assert "const N = 7" in body
    assert "className = BASE_CELL_CLASSES" in body
