from __future__ import annotations

from dataclasses import dataclass
from typing import Optional

from fastapi import HTTPException

from server.domain.board import BoardGrid, BoardState, ResultCode, WinReport
from server.services.ai import AIMoveSelector, AI_PLAYER

HUMAN_PLAYER = 1


@dataclass
class NewGameState:
    board: BoardGrid
    next_player: str


@dataclass
class MoveOutcome:
    board: BoardGrid
    result: ResultCode
    win_line: Optional[list[list[int]]] = None
    human_row: Optional[int] = None
    ai_column: Optional[int] = None
    ai_row: Optional[int] = None


class InvalidMoveError(HTTPException):
    def __init__(self, detail: str) -> None:
        super().__init__(status_code=400, detail=detail)


class GameService:
    def __init__(self, selector: AIMoveSelector, board_size: int) -> None:
        self._selector = selector
        self._board_size = board_size

    def new_game(self, human_starts: bool) -> NewGameState:
        board = BoardState.empty(self._board_size)
        next_player = "human" if human_starts else "ai"
        return NewGameState(board=board.grid, next_player=next_player)

    def human_move(self, board_payload: BoardGrid, column: int) -> MoveOutcome:
        board = self._parse_board(board_payload)
        if not isinstance(column, int):
            raise InvalidMoveError("column must be int")

        initial = board.analyze()
        if initial.result != 4:
            return self._build_outcome(board, initial, human_row=None, ai_column=None, ai_row=None)

        human_row = board.drop_token(column, HUMAN_PLAYER)
        if human_row == -1:
            raise InvalidMoveError("Invalid or full column")

        after_human = board.analyze()
        if after_human.result != 4:
            return self._build_outcome(board, after_human, human_row=human_row, ai_column=None, ai_row=None)

        ai_column = self._selector.select_move(board)
        ai_row = board.drop_token(ai_column, AI_PLAYER)
        after_ai = board.analyze()
        return self._build_outcome(
            board,
            after_ai,
            human_row=human_row,
            ai_column=ai_column,
            ai_row=ai_row,
        )

    def _parse_board(self, payload: BoardGrid) -> BoardState:
        try:
            return BoardState.from_payload(payload, size=self._board_size)
        except ValueError as exc:
            raise InvalidMoveError(str(exc)) from exc

    @staticmethod
    def _build_outcome(
        board: BoardState,
        report: WinReport,
        *,
        human_row: Optional[int],
        ai_column: Optional[int],
        ai_row: Optional[int],
    ) -> MoveOutcome:
        return MoveOutcome(
            board=board.grid,
            result=report.result,
            win_line=report.line,
            human_row=human_row,
            ai_column=ai_column,
            ai_row=ai_row,
        )


__all__ = [
    "GameService",
    "InvalidMoveError",
    "MoveOutcome",
    "NewGameState",
]
