
#define N 7
#define K 6 // K=level, profundidad arbol

#define Inf 10000000

typedef struct Node{

  	char tablero[N][N];
  	struct Node **sons;
  	int n_sons;
  	double valor;

} Node;

void printTable(char table[N][N], int level);
void printTableEstet(char table[N][N], int k);
void ponerFicha(char table[N][N], int column, int jugador);
void ultimoElementoColumn(int v[N], char table[N][N]);
int tableLlena(char table[N][N]);
int compruebaLinea(int a1,int a2, int a3, int a4);
int resultadoTable(char table[N][N]);
void inicializaTable(char table[N][N]);

void capiarTablero(char tablero[N][N], char padre_tablero[N][N]);
int numSonAColumn(char tablero[N][N], int numSon);
void aplicarTirada(char tablero[N][N], int numSon, int level);
int numSons(char tablero[N][N]);
