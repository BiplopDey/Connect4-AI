
#define N 7
#define K 6 // K=nivel, profundidad arbol

#define Inf 10000000

typedef struct Nodo{

  	char tablero[N][N];
  	struct Nodo **hijos;
  	int n_hijos;
  	double valor;

} Nodo;

void printTable(char table[N][N], int nivel);
void printTableEstet(char table[N][N], int k);
void ponerFicha(char table[N][N], int column, int jugador);
void ultimoElementoColumn(int v[N], char table[N][N]);
int tableLlena(char table[N][N]);
int compruebaLinea(int a1,int a2, int a3, int a4);
int resultadoTable(char table[N][N]);
void inicializaTable(char table[N][N]);

void capiarTablero(char tablero[N][N], char padre_tablero[N][N]);
int numHijoAColumn(char tablero[N][N], int numHijo);
void aplicarTirada(char tablero[N][N], int numHijo, int nivel);
int numHijos(char tablero[N][N]);
