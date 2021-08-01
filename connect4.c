
#include <stdio.h>
#include <stdlib.h>

#include "connect4.h"
#include "minimax.h"

void printTable(char table[N][N], int level){
    
	int i, j;

    for (i=0; i<N; i++){
    	for( int k=0;k<level;k++)
        	printf("    ");

        for(j=0; j<N; j++)
            printf("%d ",table[i][j]);

        printf("\n");
    }

    printf("\n");
}

void printTableEstet(char table[N][N], int cara){ // Cuestiones de estetica de matriz
	
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
          	if(table[i][j]==c1)
            	printf(" O |");
          	else if(table[i][j]==c2)
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

void ponerFicha(char table[N][N], int column, int player){

  	int i;

  	if(!(0<=column && column<N)){
    	printf("column erronea\n");
    	return;
  	}

  	for(i=N-1;i>=0;i--)
    	if(table[i][column]==0){
        	table[i][column] = player;
          	return;
       }

}

void ultimoElementoColumn(int v[N], char table[N][N]){
  
  	int i;

  	for(int j=0; j<N;j++){
    	v[j]=-1;// column j esta llena hasta que se demuestre lo contrario
    	
		for(i=N-1;i>=0;i--){
      		if(table[i][j]==0){
          		v[j] = i;
          		break;
      		} 
    	}
  	}

}

int tableLlena( char table[N][N]){

  	int v[N];
  	ultimoElementoColumn(v, table);
	
  	for(int j=0;j<N;j++)
		if(v[j]!=-1)
      		return 0; // 0=es falso, la table no esta llena

  return 1; // cierto, table llena

}

int compruebaLinea(int a1,int a2, int a3, int a4){

  return ((a1==1 && a2==1 && a3==1 && a4==1) 
  		|| (a1==2 && a2==2 && a3==2 && a4==2));

}

int resultadoTable(char table[N][N]){ // gana player1 1, gana player2 2, empate 3, nada 4
  
	int i,j;

  	for( j=0;j<N;j++){
    	for( i=0;i<=N-4;i++){
      		if(compruebaLinea(
				  table[i][j],
				  table[i+1][j],
				  table[i+2][j],
				  table[i+3][j])){

        		return table[i][j]; // por columns
      		}

      		if(compruebaLinea(
				  table[j][i],
				  table[j][i+1],
				  table[j][i+2],
				  table[j][i+3])){

        		return table[j][i]; // por filas
      		}
    	}
  	}

  	for( i=0;i<=N-4;i++){// por diagonal inclinado abajo
    	for( j=0;j<=N-4;j++){
    		if(compruebaLinea(
				table[i][j],
				table[i+1][j+1], 
				table[i+2][j+2], 
				table[i+3][j+3])){

    				return table[i][j];
      		}
    	}
  	}

	for(i=3;i<N;i++){ // por diagonal inclinado arriba
   		for( j=0;j<=N-4;j++){
      		if(compruebaLinea(
				  table[i][j],
				  table[i-1][j+1], 
				  table[i-2][j+2], 
				  table[i-3][j+3])){

      			return table[i][j];
     		}
    	}
  	}
 
	if(tableLlena(table))
		return 3;
  
  	return 4;

}

void inicializaTable(char table[N][N]){

  	for (int i =0; i<N; i++)
    	for(int j=0; j<N; j++)
      		table[i][j]=0;

}

void capiarBoard(char board[N][N], char father_board[N][N]){
  
  	int i,j;
  
  	for (i =0; i<N; i++){
    	for(j=0; j<N; j++){
      		board[i][j]=father_board[i][j];
    	}
  	}

}

int numSonAColumn(char board[N][N], int numSon){//sirve para responder a
  // ¿que column del father es el son numero numSon?
  	int v[N];
  	ultimoElementoColumn(v, board);
  	int count=-1;// lo mas importante entender pq
  
  	for(int i=0;i<N;i++){
    	if(v[i]!=-1){// ex v=[-1,1(son num 0),-1,4(son num 1)]
      		count++;
    	}

    	if(count==numSon){
      		return i;
    	}
  	}

  return 0; // para no dar error

}

void aplicarTirada(char board[N][N], int numSon, int level){ // numSon va de 0
  
  	int column = numSonAColumn(board,numSon);
  	ponerFicha(board, column , level%2+1);

}

int numSons(char board[N][N]){

  	int v[N];
  	ultimoElementoColumn(v, board);
  	int count=0;

  	for(int i=0;i<N;i++){
    	if(v[i]!=-1){
      		count++;
	    }
  	}

  return count;

}

int main(void) {

	int column;
	char table[N][N];

	inicializaTable(table);
	Node *raiz;

	int alfa=-Inf,beta=Inf;

	printTableEstet(table,0);
	int v[N];
	int player=1; // player=2 , empieza humano
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
		player=2;
	else 
		player=1;

	if(ch2=='y') 
		cara=1;
	else 
		cara=0;

	while(resultadoTable(table)==4){
		ultimoElementoColumn(v, table);
		
		if(player%2+1==1){ // si player es 1 toca al humano
			do{
			printf("Escoger una column="); // toca al humano
			scanf("%d",&column);
			
			if(column==-1)
				break;
			}while(v[column] ==-1 || !(0<=column && column<N) ); // mientras column este llena o 
		// o column este fuera del intervalo. seguir iterando
			if(column==-1)
				break;
				
		} else{
			raiz=creaRaiz(table);
			Max_Value(raiz,alfa,beta,0);
			column= tiradaRaiz(raiz);

				for(int i=0;i< raiz->n_sons;i++){
					free(raiz->sons[i]->sons);
					free(raiz->sons[i]);
				}
				
			free(raiz->sons);
			free(raiz);
	}

	ponerFicha(table,column,player%2+1);
	printTableEstet(table,cara);
	player++;

	switch (resultadoTable(table)) {
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
