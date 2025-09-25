from __future__ import annotations

import subprocess
from dataclasses import dataclass
from pathlib import Path

from server.domain.board import BoardState


class EngineError(RuntimeError):
    """Raised when the native engine fails or misbehaves."""


@dataclass(frozen=True)
class EngineClient:
    engine_path: Path

    def choose_column(self, board: BoardState) -> int:
        if not self.engine_path.exists():
            raise EngineError(f"engine binary not found: {self.engine_path}")

        payload = board.to_engine_payload().encode()
        completed = subprocess.run(
            [str(self.engine_path)],
            input=payload,
            capture_output=True,
        )

        if completed.returncode != 0:
            stderr = completed.stderr.decode(errors="ignore")
            raise EngineError(f"engine error: {stderr}")

        output = completed.stdout.decode().strip()
        try:
            return int(output)
        except ValueError as exc:  # pragma: no cover - defensive
            raise EngineError(f"invalid engine output: {output}") from exc


__all__ = ["EngineClient", "EngineError"]
