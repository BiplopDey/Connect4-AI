#include <stdio.h>
#include <stdlib.h>

#include "connect4.h"
#include "minimax.h"

// Simple CLI that reads a 7x7 board of ints (0,1,2) from stdin
// row-major, whitespace-separated (7 lines of 7 numbers is fine),
// computes the AI (player 2) best column and prints it.
int main(void) {
  char table[N][N];

  // Read 49 integers (0/1/2) row-major
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int v;
      if (scanf("%d", &v) != 1) {
        fprintf(stderr, "Invalid board input. Expect 49 integers.\n");
        return 2;
      }
      if (v < 0 || v > 2) {
        fprintf(stderr, "Invalid cell value %d at (%d,%d).\n", v, i, j);
        return 2;
      }
      table[i][j] = (char)v;
    }
  }

  Node *root = createRoot(table);
  double alpha = -Inf, beta = Inf;
  Max_Value(root, alpha, beta, 0);
  int column = tossRoot(root);

  // free root children (Max_Value kept root->sons for tossRoot)
  for (int i = 0; i < root->n_sons; i++) {
    if (root->sons[i]) {
      free(root->sons[i]->sons);
      free(root->sons[i]->columns);
      free(root->sons[i]);
    }
  }
  free(root->sons);
  free(root->columns);
  free(root);

  printf("%d\n", column);
  return 0;
}
