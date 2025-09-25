#include "engine/board.h"

#include <stdbool.h>

static bool column_in_bounds(int column) { return column >= 0 && column < N; }

void board_init(Board *board) {
  if (!board)
    return;
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      board->cells[row][col] = PLAYER_NONE;
    }
  }
}

void board_copy(Board *dest, const Board *src) {
  if (!dest || !src)
    return;
  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      dest->cells[row][col] = src->cells[row][col];
    }
  }
}

bool board_drop_token(Board *board, int column, Player player, int *row_played) {
  if (!board || player == PLAYER_NONE || !column_in_bounds(column))
    return false;

  for (int row = N - 1; row >= 0; row--) {
    if (board->cells[row][column] == PLAYER_NONE) {
      board->cells[row][column] = player;
      if (row_played)
        *row_played = row;
      return true;
    }
  }

  return false;
}

int board_available_rows(const Board *board, int out_rows[N]) {
  int count = 0;
  if (!board)
    return count;

  for (int col = 0; col < N; col++) {
    int row = -1;
    for (int r = N - 1; r >= 0; r--) {
      if (board->cells[r][col] == PLAYER_NONE) {
        row = r;
        break;
      }
    }
    if (out_rows)
      out_rows[col] = row;
    if (row != -1)
      count++;
  }

  return count;
}

int board_legal_moves(const Board *board, int out_columns[N]) {
  if (!board)
    return 0;

  int count = 0;
  for (int col = 0; col < N; col++) {
    if (board->cells[0][col] == PLAYER_NONE) {
      if (out_columns)
        out_columns[count] = col;
      count++;
    }
  }
  return count;
}

Player board_cell(const Board *board, int row, int col) {
  if (!board || row < 0 || row >= N || col < 0 || col >= N)
    return PLAYER_NONE;
  return (Player)board->cells[row][col];
}
