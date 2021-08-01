Node *createNode(Node *father, int numSon, int level);
Node *createRoot(char table[N][N]);
void showValue(Node *p, int level);

double Min_Value(Node *p, double alpha, double beta, int level);
double Max_Value(Node *p, double alpha, double beta, int level);

int checkLineHeur(int a1, int a2, int a3, int a4, int player, int lengthConnect);
int resultTableHeur(char table[N][N], int player, int lengthConnect);
int functionHeur(char table[N][N]);
int tossRoot(Node *p);