#ifndef ENGINE_BOARD_H
#define ENGINE_BOARD_H

#include <stdbool.h>

#define N 7

typedef enum {
  PLAYER_NONE = 0,
  PLAYER_ONE = 1,
  PLAYER_TWO = 2
} Player;

typedef struct {
  char cells[N][N];
} Board;

void board_init(Board *board);
void board_copy(Board *dest, const Board *src);
bool board_drop_token(Board *board, int column, Player player, int *row_played);
int board_available_rows(const Board *board, int out_rows[N]);
int board_legal_moves(const Board *board, int out_columns[N]);
Player board_cell(const Board *board, int row, int col);

#endif // ENGINE_BOARD_H
