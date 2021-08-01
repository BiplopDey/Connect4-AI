Node *creaNode(Node *father, int numSon, int level);
Node *creaRoot(char table[N][N]);
void mostraValue(Node *p, int level);

double Min_Value(Node *p, double alfa, double beta, int level);
double Max_Value(Node *p, double alfa, double beta, int level);

int compruebaLineaHeur(int a1, int a2, int a3, int a4, int player, int raya);
int resultadoTableHeur(char table[N][N], int player, int raya);
int funcionHeur(char table[N][N]);
int tiradaRoot(Node *p);