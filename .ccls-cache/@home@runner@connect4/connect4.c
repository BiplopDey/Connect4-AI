
#include <stdio.h>
#include <stdlib.h>

#include "connect4.h"
#include "minimax.h"

void printTable(char table[N][N], int level) {

  int i, j;

  for (i = 0; i < N; i++) {
    for (int k = 0; k < level; k++)
      printf("    ");

    for (j = 0; j < N; j++)
      printf("%d ", table[i][j]);

    printf("\n");
  }

  printf("\n");
}

void printTableEsthetic(char table[N][N], int cara) {

  int c1 = 2, c2 = 1;
  int i, j;

  if (cara) {
    c1 = 1;
    c2 = 2;
  }

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++)
      printf("----");

    printf("-\n");
    printf("|");

    for (j = 0; j < N; j++) {
      if (table[i][j] == c1)
        printf(" O |");
      else if (table[i][j] == c2)
        printf(" X |");
      else
        printf("   |");
    }

    printf("\n");
  }

  for (j = 0; j < N; j++)
    printf("----");

  printf("-\n");
  printf("|");

  for (j = 0; j < N; j++)
    printf(" %d |", j);

  printf("\n");
}

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

int tableFull(char table[N][N]) {

  int v[N];
  lastTokenColumn(v, table);

  for (int j = 0; j < N; j++)
    if (v[j] != -1)
      return 0; // 0 = is false, table is not full

  return 1; // true, table full
}

int checkLine(int a1, int a2, int a3, int a4) {

  return ((a1 == 1 && a2 == 1 && a3 == 1 && a4 == 1) ||
          (a1 == 2 && a2 == 2 && a3 == 2 && a4 == 2));
}

int resultTable(
    char table[N][N]) { // win player1 1, win player2 2, draw 3, nothing 4

  int i, j;

  for (j = 0; j < N; j++) {
    for (i = 0; i <= N - 4; i++) {
      if (checkLine(table[i][j], table[i + 1][j], table[i + 2][j],
                    table[i + 3][j])) {

        return table[i][j]; // by columns
      }

      if (checkLine(table[j][i], table[j][i + 1], table[j][i + 2],
                    table[j][i + 3])) {

        return table[j][i]; // by rows
      }
    }
  }

  for (i = 0; i <= N - 4; i++) { // by diagonal tilted down
    for (j = 0; j <= N - 4; j++) {
      if (checkLine(table[i][j], table[i + 1][j + 1], table[i + 2][j + 2],
                    table[i + 3][j + 3])) {

        return table[i][j];
      }
    }
  }

  for (i = 3; i < N; i++) { // by diagonal tilted up
    for (j = 0; j <= N - 4; j++) {
      if (checkLine(table[i][j], table[i - 1][j + 1], table[i - 2][j + 2],
                    table[i - 3][j + 3])) {

        return table[i][j];
      }
    }
  }

  if (tableFull(table))
    return 3;

  return 4;
}

void initializeTable(char table[N][N]) {

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      table[i][j] = 0;
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

int main(void) {

  int column;
  char table[N][N];

  initializeTable(table);

  int alpha = -Inf, beta = Inf;

  printTableEsthetic(table, 0);
  int v[N];
  int player = 1; // player=2 , starts human
                  // player=1 starts machine

  int cara = 0; // if heads=1, human is heads

  char ch1, ch2;
  printf("Do you want to start first? y/n :");

  do {
    scanf("%c", &ch1);
  } while (ch1 != 'y' && ch1 != 'n');

  printf("Do you want to be heads? y/n :");

  do {
    scanf("%c", &ch2);
  } while (ch2 != 'y' && ch2 != 'n');

  if (ch1 == 'y')
    player = 2;
  else
    player = 1;

  if (ch2 == 'y')
    cara = 1;
  else
    cara = 0;

  while (resultTable(table) == 4) {
    lastTokenColumn(v, table);

    if (player % 2 + 1 == 1) { // if player is 1 starts the human
      do {
        printf("Pick a column="); // starts human
        scanf("%d", &column);

        if (column == -1)
          break;
      } while (v[column] == -1 ||
               !(0 <= column && column < N)); // while column is full or
      // column is out of the interval. Continue iterating
      if (column == -1)
        break;

    } else {
      Node *root;
      root = createRoot(table);
      Max_Value(root, alpha, beta, 0);
      column = tossRoot(root);

      for (int i = 0; i < root->n_sons; i++) {
        free(root->sons[i]->sons);
        free(root->sons[i]);
      }

      free(root->sons);
      free(root);
    }

    putToken(table, column, player % 2 + 1);
    printTableEsthetic(table, cara);
    player++;

    switch (resultTable(table)) {
    case 1:
      printf("Connect 4! You have won!\n");
      break;
    case 2:
      printf("Connect 4! You have lost.\n");
      break;
    case 4:
      printf("Continue playing.\n");
      break;
    case 3:
      printf("Draw!\n");
      break;
    }
  }

  return 0;
}
