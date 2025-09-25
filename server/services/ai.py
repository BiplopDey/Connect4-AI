from __future__ import annotations

from dataclasses import dataclass

from server.domain.board import BoardState
from server.services.engine import EngineClient

AI_PLAYER = 2


@dataclass
class AIMoveSelector:
    engine: EngineClient

    def select_move(self, board: BoardState) -> int:
        immediate = board.find_immediate_win(AI_PLAYER)
        if immediate is not None:
            return immediate
        return self.engine.choose_column(board)


__all__ = ["AIMoveSelector", "AI_PLAYER"]
