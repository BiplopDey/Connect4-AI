Nodo *creaNodo(Nodo *padre, int numHijo, int nivel);
Nodo *creaRaiz(char table[N][N]);
void mostraValor(Nodo *p, int nivel);

double Min_Value(Nodo *p, double alfa, double beta, int nivel);
double Max_Value(Nodo *p, double alfa, double beta, int nivel);

int compruebaLineaHeur(int a1, int a2, int a3, int a4, int jugador, int raya);
int resultadoTableHeur(char table[N][N], int jugador, int raya);
int funcionHeur(char table[N][N]);
int tiradaRaiz(Nodo *p);

