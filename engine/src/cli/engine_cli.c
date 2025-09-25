#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine/board.h"
#include "engine/config.h"
#include "engine/evaluation.h"
#include "engine/search.h"

int main(void) {
  Board board;
  board_init(&board);

  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      int value;
      if (scanf("%d", &value) != 1) {
        fprintf(stderr, "Invalid board input. Expect 49 integers.\n");
        return 2;
      }
      if (value < 0 || value > 2) {
        fprintf(stderr, "Invalid cell value %d at (%d,%d).\n", value, row,
                col);
        return 2;
      }
      board.cells[row][col] = (char)value;
    }
  }

  SearchConfig config;
  search_config_init(&config);
  config.maximizing_player = PLAYER_TWO;

  const char *depth_env = getenv("ENGINE_MAX_DEPTH");
  if (depth_env) {
    char *end = NULL;
    long depth = strtol(depth_env, &end, 10);
    if (end != depth_env && depth > 0 && depth < INT_MAX) {
      config.max_depth = (int)depth;
    }
  }

  if (config.max_depth <= 0)
    config.max_depth = ENGINE_DEFAULT_MAX_DEPTH;

  int column = search_best_move(&board, PLAYER_TWO, &config);
  printf("%d\n", column);
  return column < 0 ? 1 : 0;
}
