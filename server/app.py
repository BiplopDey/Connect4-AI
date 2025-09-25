from __future__ import annotations

from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

from server.api.routes import router
from server.config import get_settings

settings = get_settings()

app = FastAPI(title="Connect4 AI Server")
app.include_router(router)
app.mount("/static", StaticFiles(directory=settings.static_dir), name="static")
app.extra["static_dir"] = settings.static_dir

N = settings.board_size

__all__ = ["app", "N"]
