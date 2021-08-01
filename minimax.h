Node *createNode(Node *father, int numSon, int level);
Node *createRoot(char table[N][N]);
void showValue(Node *p, int level);

double Min_Value(Node *p, double alfa, double beta, int level);
double Max_Value(Node *p, double alfa, double beta, int level);

int checkLineHeur(int a1, int a2, int a3, int a4, int player, int raya);
int resultTableHeur(char table[N][N], int player, int raya);
int funcionHeur(char table[N][N]);
int tiradaRoot(Node *p);