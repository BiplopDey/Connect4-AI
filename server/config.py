from __future__ import annotations

import os
from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path


@dataclass(frozen=True)
class Settings:
    board_size: int
    static_dir: Path
    engine_path: Path


def _default_board_size() -> int:
    return 7


def _default_static_dir(base_dir: Path) -> Path:
    return base_dir.parent / "web"


def _default_engine_path(base_dir: Path) -> Path:
    fallback = base_dir.parent / "engine" / "engine"
    env_value = os.environ.get("CONNECT4_ENGINE")
    return Path(env_value).expanduser().resolve() if env_value else fallback.resolve()


def _build_settings() -> Settings:
    module_dir = Path(__file__).resolve().parent
    board_size = _default_board_size()
    static_dir = _default_static_dir(module_dir)
    engine_path = _default_engine_path(module_dir)
    return Settings(board_size=board_size, static_dir=static_dir, engine_path=engine_path)


@lru_cache(maxsize=1)
def get_settings() -> Settings:
    return _build_settings()


__all__ = ["Settings", "get_settings"]
