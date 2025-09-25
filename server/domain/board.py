from __future__ import annotations

from dataclasses import dataclass
from typing import List, Literal, Optional, Sequence

BoardValue = Literal[0, 1, 2]
Player = Literal[1, 2]
ResultCode = Literal[1, 2, 3, 4]
BoardGrid = List[List[int]]
Coordinate = List[int]


@dataclass(frozen=True)
class WinReport:
    result: ResultCode
    line: Optional[List[Coordinate]] = None


class BoardState:
    """Encapsulates a connect-four board and related operations."""

    def __init__(self, cells: BoardGrid, size: int) -> None:
        self._size = size
        self._cells = cells

    @property
    def size(self) -> int:
        return self._size

    @property
    def grid(self) -> BoardGrid:
        return self._cells

    @classmethod
    def empty(cls, size: int) -> "BoardState":
        return cls([[0 for _ in range(size)] for _ in range(size)], size=size)

    @classmethod
    def from_payload(cls, payload: Sequence[Sequence[int]], size: int) -> "BoardState":
        cls.validate(payload, size)
        copied = [[int(value) for value in row] for row in payload]
        return cls(copied, size=size)

    @staticmethod
    def validate(payload: Sequence[Sequence[int]], size: int) -> None:
        if not isinstance(payload, list) or len(payload) != size:
            raise ValueError(f"Board must be a {size}x{size} array")
        for row in payload:
            if not isinstance(row, list) or len(row) != size:
                raise ValueError(f"Board must be a {size}x{size} array")
            for value in row:
                if value not in (0, 1, 2):
                    raise ValueError("Board cells must be 0, 1, or 2")

    def clone(self) -> "BoardState":
        return BoardState([row[:] for row in self._cells], size=self._size)

    def drop_token(self, column: int, player: Player) -> int:
        if column < 0 or column >= self._size:
            return -1
        for row in range(self._size - 1, -1, -1):
            if self._cells[row][column] == 0:
                self._cells[row][column] = player
                return row
        return -1

    def column_full(self, column: int) -> bool:
        return self._cells[0][column] != 0

    def is_full(self) -> bool:
        return all(self.column_full(col) for col in range(self._size))

    def analyze(self) -> WinReport:
        winner = self._find_winning_line()
        if winner:
            player, line = winner
            return WinReport(result=player, line=line)
        if self.is_full():
            return WinReport(result=3, line=None)
        return WinReport(result=4, line=None)

    def find_immediate_win(self, player: Player) -> Optional[int]:
        for column in range(self._size):
            if self.column_full(column):
                continue
            cloned = self.clone()
            cloned.drop_token(column, player)
            if cloned.analyze().result == player:
                return column
        return None

    def to_engine_payload(self) -> str:
        lines = [" ".join(str(value) for value in row) for row in self._cells]
        return "\n".join(lines) + "\n"

    def _find_winning_line(self) -> Optional[tuple[Player, List[Coordinate]]]:
        coords = self._cells
        size = self._size

        # Vertical checks
        for col in range(size):
            for row in range(size - 3):
                player = self._line_winner(
                    coords[row][col],
                    coords[row + 1][col],
                    coords[row + 2][col],
                    coords[row + 3][col],
                )
                if player:
                    return player, [[row + offset, col] for offset in range(4)]

        # Horizontal checks
        for row in range(size):
            for col in range(size - 3):
                player = self._line_winner(
                    coords[row][col],
                    coords[row][col + 1],
                    coords[row][col + 2],
                    coords[row][col + 3],
                )
                if player:
                    return player, [[row, col + offset] for offset in range(4)]

        # Diagonal down-right
        for row in range(size - 3):
            for col in range(size - 3):
                player = self._line_winner(
                    coords[row][col],
                    coords[row + 1][col + 1],
                    coords[row + 2][col + 2],
                    coords[row + 3][col + 3],
                )
                if player:
                    return player, [[row + offset, col + offset] for offset in range(4)]

        # Diagonal up-right
        for row in range(3, size):
            for col in range(size - 3):
                player = self._line_winner(
                    coords[row][col],
                    coords[row - 1][col + 1],
                    coords[row - 2][col + 2],
                    coords[row - 3][col + 3],
                )
                if player:
                    return player, [[row - offset, col + offset] for offset in range(4)]

        return None

    @staticmethod
    def _line_winner(a: int, b: int, c: int, d: int) -> Optional[Player]:
        if a != 0 and a == b == c == d:
            return a  # type: ignore[return-value]
        return None


__all__ = ["BoardState", "WinReport", "ResultCode", "BoardGrid", "Coordinate"]
