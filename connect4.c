
#include <stdio.h>
#include <stdlib.h>

#include "connect4.h"
#include "minimax.h"

void printTabla(char tabla[N][N], int nivel){
    
	int i, j;

    for (i=0; i<N; i++){
    	for( int k=0;k<nivel;k++)
        	printf("    ");

        for(j=0; j<N; j++)
            printf("%d ",tabla[i][j]);

        printf("\n");
    }

    printf("\n");
}

void printTablaEstet(char tabla[N][N], int cara){ // Cuestiones de estetica de matriz
	
	int c1=2, c2=1;
	int i,j;

  	if(cara){
    	c1=1;
    	c2=2;
  	}
  
    for (i =0; i<N; i++){
		for(j=0; j<N; j++)
        	printf("----");
        
		printf("-\n");
        printf("|");

        for(j=0; j<N; j++){
          	if(tabla[i][j]==c1)
            	printf(" O |");
          	else if(tabla[i][j]==c2)
            	printf(" X |");
          	else
            	printf("   |");
        }

        printf("\n");
    }

    for(j=0; j<N; j++)
        printf("----");

    printf("-\n");
    printf("|");

    for(j=0; j<N; j++)
        printf(" %d |",j);

    printf("\n");

}

void ponerFicha(char tabla[N][N], int columna, int jugador){

  	int i;

  	if(!(0<=columna && columna<N)){
    	printf("columna erronea\n");
    	return;
  	}

  	for(i=N-1;i>=0;i--)
    	if(tabla[i][columna]==0){
        	tabla[i][columna] = jugador;
          	return;
       }

}

void ultimoElementoColumna(int v[N], char tabla[N][N]){
  
  	int i;

  	for(int j=0; j<N;j++){
    	v[j]=-1;// columna j esta llena hasta que se demuestre lo contrario
    	
		for(i=N-1;i>=0;i--){
      		if(tabla[i][j]==0){
          		v[j] = i;
          		break;
      		} 
    	}
  	}

}

int tablaLlena( char tabla[N][N]){

  	int v[N];
  	ultimoElementoColumna(v, tabla);
	
  	for(int j=0;j<N;j++)
		if(v[j]!=-1)
      		return 0; // 0=es falso, la tabla no esta llena

  return 1; // cierto, tabla llena

}

int compruebaLinea(int a1,int a2, int a3, int a4){

  return ((a1==1 && a2==1 && a3==1 && a4==1) 
  		|| (a1==2 && a2==2 && a3==2 && a4==2));

}

int resultadoTabla(char tabla[N][N]){ // gana player1 1, gana player2 2, empate 3, nada 4
  
	int i,j;

  	for( j=0;j<N;j++){
    	for( i=0;i<=N-4;i++){
      		if(compruebaLinea(
				  tabla[i][j],
				  tabla[i+1][j],
				  tabla[i+2][j],
				  tabla[i+3][j])){

        		return tabla[i][j]; // por columnas
      		}

      		if(compruebaLinea(
				  tabla[j][i],
				  tabla[j][i+1],
				  tabla[j][i+2],
				  tabla[j][i+3])){

        		return tabla[j][i]; // por filas
      		}
    	}
  	}

  	for( i=0;i<=N-4;i++){// por diagonal inclinado abajo
    	for( j=0;j<=N-4;j++){
    		if(compruebaLinea(
				tabla[i][j],
				tabla[i+1][j+1], 
				tabla[i+2][j+2], 
				tabla[i+3][j+3])){

    				return tabla[i][j];
      		}
    	}
  	}

	for(i=3;i<N;i++){ // por diagonal inclinado arriba
   		for( j=0;j<=N-4;j++){
      		if(compruebaLinea(
				  tabla[i][j],
				  tabla[i-1][j+1], 
				  tabla[i-2][j+2], 
				  tabla[i-3][j+3])){

      			return tabla[i][j];
     		}
    	}
  	}
 
	if(tablaLlena(tabla))
		return 3;
  
  	return 4;

}

void inicializaTabla(char tabla[N][N]){

  	for (int i =0; i<N; i++)
    	for(int j=0; j<N; j++)
      		tabla[i][j]=0;

}

void capiarTablero(char tablero[N][N],char padre_tablero[N][N]){
  
  	int i,j;
  
  	for (i =0; i<N; i++){
    	for(j=0; j<N; j++){
      		tablero[i][j]=padre_tablero[i][j];
    	}
  	}

}

int numHijoAColumna(char tablero[N][N],int numHijo){//sirve para responder a
  // ¿que columna del padre es el hijo numero numHijo?
  	int v[N];
  	ultimoElementoColumna(v, tablero);
  	int count=-1;// lo mas importante entender pq
  
  	for(int i=0;i<N;i++){
    	if(v[i]!=-1){// ex v=[-1,1(hijo num 0),-1,4(hijo num 1)]
      		count++;
    	}

    	if(count==numHijo){
      		return i;
    	}
  	}

  return 0; // para no dar error

}

void aplicarTirada(char tablero[N][N], int numHijo, int nivel){ // numHijo va de 0
  
  	int columna = numHijoAColumna(tablero,numHijo);
  	ponerFicha(tablero, columna , nivel%2+1);

}

int numHijos(char tablero[N][N]){

  	int v[N];
  	ultimoElementoColumna(v, tablero);
  	int count=0;

  	for(int i=0;i<N;i++){
    	if(v[i]!=-1){
      		count++;
	    }
  	}

  return count;

}

int main(void) {

	int columna;
	char tabla[N][N];

	inicializaTabla(tabla);
	Nodo *raiz;

	int alfa=-Inf,beta=Inf;

	printTablaEstet(tabla,0);
	int v[N];
	int jugador=1; // jugador=2 , empieza humano
               // juagador=1 empezar el ordenador

	int cara=0; // si cara=1, humano es cara 

	char ch1,ch2;
	printf("Quieres empezar primero? y/n :");

	do{
	scanf("%c",&ch1);
	}while(ch1 !='y' && ch1 !='n');

	printf("Quieres ser cara? y/n :");

	do{
		scanf("%c",&ch2);
	}while(ch2 !='y' && ch2 !='n');

	if(ch1=='y') 
		jugador=2;
	else 
		jugador=1;

	if(ch2=='y') 
		cara=1;
	else 
		cara=0;

	while(resultadoTabla(tabla)==4){
		ultimoElementoColumna(v, tabla);
		
		if(jugador%2+1==1){ // si jugador es 1 toca al humano
			do{
			printf("Escoger una columna="); // toca al humano
			scanf("%d",&columna);
			
			if(columna==-1)
				break;
			}while(v[columna] ==-1 || !(0<=columna && columna<N) ); // mientras columna este llena o 
		// o columna este fuera del intervalo. seguir iterando
			if(columna==-1)
				break;
				
		} else{
			raiz=creaRaiz(tabla);
			Max_Value(raiz,alfa,beta,0);
			columna= tiradaRaiz(raiz);

				for(int i=0;i< raiz->n_hijos;i++){
					free(raiz->hijos[i]->hijos);
					free(raiz->hijos[i]);
				}
				
			free(raiz->hijos);
			free(raiz);
	}

	ponerFicha(tabla,columna,jugador%2+1);
	printTablaEstet(tabla,cara);
	jugador++;

	switch (resultadoTabla(tabla)) {
		case 1:
			printf("4 en raya! Has ganado!\n");
			break;
		case 2:
			printf("4 en raya! Has perdido\n");
			break;
		case 4:
			printf("Sigue la partida\n");
			break;
		case 3:
			printf("Empate\n");
			break;
		}
	}

	return 0;

}
