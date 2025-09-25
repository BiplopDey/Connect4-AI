from __future__ import annotations

from functools import lru_cache

from server.config import get_settings
from server.services.ai import AIMoveSelector
from server.services.engine import EngineClient
from server.services.game import GameService


@lru_cache(maxsize=1)
def get_engine_client() -> EngineClient:
    settings = get_settings()
    return EngineClient(engine_path=settings.engine_path)


@lru_cache(maxsize=1)
def get_ai_selector() -> AIMoveSelector:
    return AIMoveSelector(engine=get_engine_client())


def get_game_service() -> GameService:
    settings = get_settings()
    selector = get_ai_selector()
    return GameService(selector=selector, board_size=settings.board_size)


__all__ = [
    "get_engine_client",
    "get_ai_selector",
    "get_game_service",
]
