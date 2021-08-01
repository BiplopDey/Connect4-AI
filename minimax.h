Node *creaNode(Node *padre, int numSon, int level);
Node *creaRaiz(char table[N][N]);
void mostraValor(Node *p, int level);

double Min_Value(Node *p, double alfa, double beta, int level);
double Max_Value(Node *p, double alfa, double beta, int level);

int compruebaLineaHeur(int a1, int a2, int a3, int a4, int jugador, int raya);
int resultadoTableHeur(char table[N][N], int jugador, int raya);
int funcionHeur(char table[N][N]);
int tiradaRaiz(Node *p);

