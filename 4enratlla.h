
#define N 7
#define K 6 //K=nivel, profundidad arbol

#define Inf 10000000

typedef struct node{
  char tauler[N][N];
  struct node **hijos;
  int n_hijos;
  double valor;
}Node;

void printTabla(char tabla[N][N], int nivell);
void printTablaEstet(char tabla[N][N], int k);
void ponerFicha(char tabla[N][N], int columna, int jugador);
void ultimoElementoColumna(int v[N], char tabla[N][N]);
int tablaLlena( char tabla[N][N]);
int compruebaLinea(int a1,int a2, int a3, int a4);
int resultadoTabla(char tabla[N][N]);
void inicializaTabla(char tabla[N][N]);

void capiarTauler(char tauler[N][N],char pare_tauler[N][N]);
int numHijoAColumna(char tauler[N][N],int numHijo);
void aplicarTirada(char tauler[N][N],int numHijo, int nivell);
int numHijos(char tauler[N][N]);
