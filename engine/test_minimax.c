#include <assert.h>
#include <stdio.h>

#include "connect4.h"
#include "minimax.h"

static void reset_board(char board[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      board[i][j] = 0;
    }
  }
}

static void test_checkLineHeur_cases(void) {
  assert(checkLineHeur(2, 2, 0, 0, 2, 2) == 1);
  assert(checkLineHeur(0, 2, 2, 0, 2, 2) == 2);
  assert(checkLineHeur(2, 2, 2, 0, 2, 3) == 1);
  assert(checkLineHeur(0, 1, 1, 1, 1, 3) == 1);
  assert(checkLineHeur(1, 1, 1, 1, 1, 4) == 1);
  assert(checkLineHeur(0, 0, 0, 0, 2, 4) == 0);
}

static void test_resultTableHeur_detects_sequences(void) {
  char board[N][N];
  reset_board(board);

  // Horizontal window for player 2
  board[0][0] = 2;
  board[0][1] = 2;
  board[0][2] = 0;
  board[0][3] = 0;
  assert(resultTableHeur(board, 2, 2) == 1);

  reset_board(board);
  board[0][0] = 2;
  board[0][1] = 2;
  board[0][2] = 2;
  board[0][3] = 0;
  assert(resultTableHeur(board, 2, 3) == 1);

  reset_board(board);
  board[0][0] = 2;
  board[0][1] = 2;
  board[0][2] = 2;
  board[0][3] = 2;
  assert(resultTableHeur(board, 2, 4) == 1);
}

static void test_functionHeur_scoring(void) {
  char board[N][N];
  reset_board(board);

  board[0][0] = 1;
  board[0][1] = 1;
  board[0][2] = 1;
  board[0][3] = 1;
  assert(functionHeur(board) == -100000);

  reset_board(board);
  board[0][0] = 2;
  board[0][1] = 2;
  board[0][2] = 2;
  board[0][3] = 2;
  assert(functionHeur(board) == 100000);

  reset_board(board);
  board[0][0] = 2;
  board[0][1] = 2;
  board[0][2] = 2;
  assert(functionHeur(board) == 14);

  reset_board(board);
  board[0][0] = 1;
  board[0][1] = 1;
  board[0][2] = 1;
  assert(functionHeur(board) == -14);
}

int main(void) {
  test_checkLineHeur_cases();
  test_resultTableHeur_detects_sequences();
  test_functionHeur_scoring();
  printf("minimax heuristics tests passed\n");
  return 0;
}
