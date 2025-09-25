#include <assert.h>
#include <stdio.h>

#include "engine/board.h"
#include "engine/evaluation.h"

static Board make_board(void) {
  Board board;
  board_init(&board);
  return board;
}

static void test_evaluation_default_empty_board_returns_zero(void) {
  Board board = make_board();
  double score = evaluation_default(&board, PLAYER_ONE, NULL);
  assert(score == 0.0);
}

static void test_evaluation_default_detects_maximizer_connect_four(void) {
  Board board = make_board();
  for (int i = 0; i < 4; i++) {
    assert(board_drop_token(&board, 0, PLAYER_ONE, NULL));
  }
  double score = evaluation_default(&board, PLAYER_ONE, NULL);
  assert(score == (double)EVALUATION_DEFAULT_WEIGHTS.win_score);
}

static void test_evaluation_default_detects_opponent_connect_four(void) {
  Board board = make_board();
  for (int col = 0; col < 4; col++) {
    assert(board_drop_token(&board, col, PLAYER_TWO, NULL));
  }
  double score = evaluation_default(&board, PLAYER_ONE, NULL);
  assert(score == (double)EVALUATION_DEFAULT_WEIGHTS.opponent_win_score);
}

static void test_evaluation_uses_custom_weights_for_two_in_row(void) {
  Board board = make_board();
  EvaluationWeights weights = {
      .win_score = 1000,
      .opponent_win_score = -1000,
      .three_in_row_score = 0,
      .opponent_three_in_row_score = 0,
      .two_in_row_score = 5,
      .opponent_two_in_row_score = -8,
  };

  assert(board_drop_token(&board, 0, PLAYER_ONE, NULL));
  assert(board_drop_token(&board, 0, PLAYER_ONE, NULL));

  double score = evaluation_default(&board, PLAYER_ONE, &weights);
  assert(score == 5.0);
}

static void test_evaluation_penalizes_opponent_threats(void) {
  Board board = make_board();
  EvaluationWeights weights = {
      .win_score = 0,
      .opponent_win_score = 0,
      .three_in_row_score = 0,
      .opponent_three_in_row_score = -42,
      .two_in_row_score = 0,
      .opponent_two_in_row_score = 0,
  };

  assert(board_drop_token(&board, 0, PLAYER_TWO, NULL));
  assert(board_drop_token(&board, 1, PLAYER_TWO, NULL));
  assert(board_drop_token(&board, 2, PLAYER_TWO, NULL));

  double score = evaluation_default(&board, PLAYER_ONE, &weights);
  assert(score == -42.0);
}

static void test_evaluation_handles_null_board_pointer(void) {
  double score = evaluation_default(NULL, PLAYER_ONE, NULL);
  assert(score == 0.0);
}

int main(void) {
  test_evaluation_default_empty_board_returns_zero();
  test_evaluation_default_detects_maximizer_connect_four();
  test_evaluation_default_detects_opponent_connect_four();
  test_evaluation_uses_custom_weights_for_two_in_row();
  test_evaluation_penalizes_opponent_threats();
  test_evaluation_handles_null_board_pointer();
  printf("evaluation tests passed\n");
  return 0;
}
