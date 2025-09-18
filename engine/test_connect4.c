#include <assert.h>
#include <stdio.h>

#include "connect4.h"

static void reset_board(char board[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      board[i][j] = 0;
    }
  }
}

static void fill_column(char board[N][N], int column, char value) {
  for (int row = 0; row < N; row++) {
    board[row][column] = value;
  }
}

static void test_putToken_stacks_from_bottom(void) {
  char board[N][N];
  reset_board(board);

  putToken(board, 3, 1);
  assert(board[N - 1][3] == 1);

  putToken(board, 3, 2);
  assert(board[N - 2][3] == 2);
}

static void test_lastTokenColumn_tracks_available_slots(void) {
  char board[N][N];
  reset_board(board);

  int slots[N];
  lastTokenColumn(slots, board);
  for (int col = 0; col < N; col++) {
    assert(slots[col] == N - 1);
  }

  putToken(board, 0, 1);
  lastTokenColumn(slots, board);
  assert(slots[0] == N - 2);

  fill_column(board, 1, 1);
  lastTokenColumn(slots, board);
  assert(slots[1] == -1);
}

static void test_numSons_counts_open_columns(void) {
  char board[N][N];
  reset_board(board);
  assert(numSons(board) == N);

  fill_column(board, 0, 1);
  fill_column(board, 3, 2);
  assert(numSons(board) == N - 2);
}

static void test_numSonToColumn_skips_full_columns(void) {
  char board[N][N];
  reset_board(board);

  fill_column(board, 0, 1);
  fill_column(board, 3, 1);

  assert(numSonToColumn(board, 0) == 1);
  assert(numSonToColumn(board, 1) == 2);
  assert(numSonToColumn(board, 2) == 4);
}

static void test_tokenRoll_places_tokens_for_current_player(void) {
  char board[N][N];
  reset_board(board);

  tokenRoll(board, 0, 0);
  assert(board[N - 1][0] == 1);

  tokenRoll(board, 0, 1);
  assert(board[N - 2][0] == 2);
}

static void test_copyBoard_duplicates_board_state(void) {
  char src[N][N];
  reset_board(src);
  src[0][0] = 1;
  src[N - 1][6] = 2;

  char dst[N][N];
  reset_board(dst);
  copyBoard(dst, src);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      assert(dst[i][j] == src[i][j]);
    }
  }
}

int main(void) {
  test_putToken_stacks_from_bottom();
  test_lastTokenColumn_tracks_available_slots();
  test_numSons_counts_open_columns();
  test_numSonToColumn_skips_full_columns();
  test_tokenRoll_places_tokens_for_current_player();
  test_copyBoard_duplicates_board_state();
  printf("connect4 helper tests passed\n");
  return 0;
}
