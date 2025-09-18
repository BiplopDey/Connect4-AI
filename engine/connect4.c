
#include <stdio.h>
#include <stdlib.h>

#include "connect4.h"
#include "minimax.h"

void putToken(char table[N][N], int column, int player) {

  int i;

  if (!(0 <= column && column < N)) {
    printf("column erronea\n");
    return;
  }

  for (i = N - 1; i >= 0; i--)
    if (table[i][column] == 0) {
      table[i][column] = player;
      return;
    }
}

void lastTokenColumn(int v[N], char table[N][N]) {

  int i;

  for (int j = 0; j < N; j++) {
    v[j] = -1; // column j is full until the contrary proven

    for (i = N - 1; i >= 0; i--) {
      if (table[i][j] == 0) {
        v[j] = i;
        break;
      }
    }
  }
}

void copyBoard(char board[N][N], char father_board[N][N]) {

  int i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      board[i][j] = father_board[i][j];
    }
  }
}

int numSonToColumn(char board[N][N], int numSon) { // to respond
  // what father's column is son's number numSon?
  int v[N];
  lastTokenColumn(v, board);
  int count = -1; // is important to undertant the why

  for (int i = 0; i < N; i++) {
    if (v[i] != -1) { // ex v=[-1,1(son num 0),-1,4(son num 1)]
      count++;
    }

    if (count == numSon) {
      return i;
    }
  }

  return 0; // to avoid error
}

void tokenRoll(char board[N][N], int numSon,
               int level) { // numSon starts with 0

  int column = numSonToColumn(board, numSon);
  putToken(board, column, level % 2 + 1);
}

int numSons(char board[N][N]) {

  int v[N];
  lastTokenColumn(v, board);
  int count = 0;

  for (int i = 0; i < N; i++) {
    if (v[i] != -1) {
      count++;
    }
  }

  return count;
}
