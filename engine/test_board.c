#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "board.h"

static Board make_board(void) {
  Board board;
  board_init(&board);
  return board;
}

static void test_board_drop_token_stacks_from_bottom(void) {
  Board board = make_board();
  int row;
  assert(board_drop_token(&board, 3, PLAYER_ONE, &row) == true);
  assert(row == N - 1);
  assert(board_cell(&board, N - 1, 3) == PLAYER_ONE);

  assert(board_drop_token(&board, 3, PLAYER_TWO, &row) == true);
  assert(row == N - 2);
  assert(board_cell(&board, N - 2, 3) == PLAYER_TWO);
}

static void test_board_drop_token_rejects_invalid_moves(void) {
  Board board = make_board();
  assert(board_drop_token(&board, -1, PLAYER_ONE, NULL) == false);
  assert(board_drop_token(&board, N, PLAYER_ONE, NULL) == false);
  assert(board_drop_token(&board, 0, PLAYER_NONE, NULL) == false);
}

static void test_board_available_rows_tracks_slots(void) {
  Board board = make_board();
  int slots[N];
  board_available_rows(&board, slots);
  for (int col = 0; col < N; col++) {
    assert(slots[col] == N - 1);
  }

  board_drop_token(&board, 0, PLAYER_ONE, NULL);
  board_available_rows(&board, slots);
  assert(slots[0] == N - 2);

  for (int i = 0; i < N; i++) {
    board_drop_token(&board, 1, PLAYER_ONE, NULL);
  }
  board_available_rows(&board, slots);
  assert(slots[1] == -1);
}

static void test_board_legal_moves_counts_open_columns(void) {
  Board board = make_board();
  int moves[N];
  assert(board_legal_moves(&board, moves) == N);

  for (int i = 0; i < N; i++) {
    board_drop_token(&board, 0, (i % 2) ? PLAYER_TWO : PLAYER_ONE, NULL);
  }

  assert(board_legal_moves(&board, moves) == N - 1);
}

static void test_board_copy_duplicates_state(void) {
  Board src = make_board();
  board_drop_token(&src, 0, PLAYER_ONE, NULL);
  board_drop_token(&src, 6, PLAYER_TWO, NULL);

  Board dst;
  board_init(&dst);
  board_copy(&dst, &src);

  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {
      assert(board_cell(&dst, row, col) == board_cell(&src, row, col));
    }
  }
}

int main(void) {
  test_board_drop_token_stacks_from_bottom();
  test_board_drop_token_rejects_invalid_moves();
  test_board_available_rows_tracks_slots();
  test_board_legal_moves_counts_open_columns();
  test_board_copy_duplicates_state();
  printf("board tests passed\n");
  return 0;
}
