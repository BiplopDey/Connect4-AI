Node *creaNode(Node *pare, int numFill, int nivell);
Node *creaRaiz(char tabla[N][N]);
void mostraValor(Node *p,int nivell);

double Min_Value(Node *p,double alfa, double beta,int nivel);
double Max_Value(Node *p,double alfa, double beta,int nivel);

int compruebaLineaHeur(int a1,int a2,int a3,int a4,int jugador,int raya);
int resultadoTablaHeur(char tabla[N][N], int jugador, int raya);
int funcionHeur(char tabla[N][N]);
int tiradaRaiz(Node *p);

