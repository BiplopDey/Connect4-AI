
#define N 7
#define K 6 //K=nivel, profundidad arbol

#define Inf 10000000

typedef struct Nodo{
  char tablero[N][N];
  struct Nodo **hijos;
  int n_hijos;
  double valor;
}Nodo;

void printTabla(char tabla[N][N], int nivell);
void printTablaEstet(char tabla[N][N], int k);
void ponerFicha(char tabla[N][N], int columna, int jugador);
void ultimoElementoColumna(int v[N], char tabla[N][N]);
int tablaLlena( char tabla[N][N]);
int compruebaLinea(int a1,int a2, int a3, int a4);
int resultadoTabla(char tabla[N][N]);
void inicializaTabla(char tabla[N][N]);

void capiarTablero(char tablero[N][N],char padre_tablero[N][N]);
int numHijoAColumna(char tablero[N][N],int numHijo);
void aplicarTirada(char tablero[N][N],int numHijo, int nivell);
int numHijos(char tablero[N][N]);
