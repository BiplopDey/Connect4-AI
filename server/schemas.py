from __future__ import annotations

from typing import List, Literal, Optional

from pydantic import BaseModel

from server.services.game import MoveOutcome, NewGameState

ResultLiteral = Literal[1, 2, 3, 4]


class MoveRequest(BaseModel):
    board: List[List[int]]
    column: int


class NewGameResponse(BaseModel):
    board: List[List[int]]
    next: Literal["human", "ai"]

    @classmethod
    def from_state(cls, state: NewGameState) -> "NewGameResponse":
        return cls(board=state.board, next=state.next_player)


class MoveResponse(BaseModel):
    board: List[List[int]]
    result: ResultLiteral
    winLine: Optional[List[List[int]]]
    humanRow: Optional[int]
    aiColumn: Optional[int]
    aiRow: Optional[int]

    @classmethod
    def from_outcome(cls, outcome: MoveOutcome) -> "MoveResponse":
        return cls(
            board=outcome.board,
            result=outcome.result,
            winLine=outcome.win_line,
            humanRow=outcome.human_row,
            aiColumn=outcome.ai_column,
            aiRow=outcome.ai_row,
        )


__all__ = [
    "MoveRequest",
    "NewGameResponse",
    "MoveResponse",
]
