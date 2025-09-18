
#define N 7
#define K 6 // K=level, depth tree

#define Inf 10000000

typedef struct Node {

  char board[N][N];
  struct Node **sons;
  int n_sons;
  double value;

} Node;

void printTable(char table[N][N], int level);
void printTableEsthetic(char table[N][N], int k);
void putToken(char table[N][N], int column, int player);
void lastTokenColumn(int v[N], char table[N][N]);
int tableFull(char table[N][N]);
int checkLine(int a1, int a2, int a3, int a4);
int resultTable(char table[N][N]);
void initializeTable(char table[N][N]);

void copyBoard(char board[N][N], char father_board[N][N]);
int numSonToColumn(char board[N][N], int numSon);
void tokenRoll(char board[N][N], int numSon, int level);
int numSons(char board[N][N]);
