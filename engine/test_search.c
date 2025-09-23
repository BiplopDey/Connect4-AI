#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "evaluation.h"
#include "search.h"

static Board make_board(void) {
  Board board;
  board_init(&board);
  return board;
}

static SearchConfig make_config(int depth) {
  SearchConfig config;
  search_config_init(&config);
  config.max_depth = depth;
  config.maximizing_player = PLAYER_TWO;
  return config;
}

static void test_default_search_prefers_center_opening(void) {
  Board board = make_board();
  SearchConfig config = make_config(4);

  int column = search_best_move(&board, PLAYER_TWO, &config);
  assert(column == 3);
}

static void reverse_order(const Board *board, int *columns, int count) {
  (void)board;
  for (int i = 0; i < count / 2; i++) {
    int tmp = columns[i];
    columns[i] = columns[count - 1 - i];
    columns[count - 1 - i] = tmp;
  }
}

static double zero_heuristic(const Board *board, Player maximizer,
                             const EvaluationWeights *weights) {
  (void)board;
  (void)maximizer;
  (void)weights;
  return 0.0;
}

static void test_move_order_override_changes_preference(void) {
  Board board = make_board();
  SearchConfig config = make_config(2);
  config.order_moves = reverse_order;
  config.evaluate = zero_heuristic;

  int column = search_best_move(&board, PLAYER_TWO, &config);
  assert(column == 6);
}

static void test_search_detects_immediate_win(void) {
  Board board = make_board();
  SearchConfig config = make_config(4);

  // Set up a board where PLAYER_TWO can win by playing column 3
  board_drop_token(&board, 0, PLAYER_ONE, NULL);
  board_drop_token(&board, 3, PLAYER_TWO, NULL);
  board_drop_token(&board, 4, PLAYER_ONE, NULL);
  board_drop_token(&board, 3, PLAYER_TWO, NULL);
  board_drop_token(&board, 5, PLAYER_ONE, NULL);
  board_drop_token(&board, 3, PLAYER_TWO, NULL);

  int winning_move = search_best_move(&board, PLAYER_TWO, &config);
  assert(winning_move == 3);
}

int main(void) {
  test_default_search_prefers_center_opening();
  test_move_order_override_changes_preference();
  test_search_detects_immediate_win();
  printf("search tests passed\n");
  return 0;
}
