
#define N 7
#define K 10 // K=level, depth tree

#define Inf 10000000

typedef struct Node {

  char board[N][N];
  struct Node **sons;
  int *columns;
  int n_sons;
  double value;

} Node;

void putToken(char table[N][N], int column, int player);
void lastTokenColumn(int v[N], char table[N][N]);

void copyBoard(char board[N][N], char father_board[N][N]);
int numSonToColumn(char board[N][N], int numSon);
void tokenRoll(char board[N][N], int numSon, int level);
int numSons(char board[N][N]);
