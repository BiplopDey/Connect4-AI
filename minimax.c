#include <stdio.h>
#include <stdlib.h>

#include "connect4.h"
#include "minimax.h"


double Max_Value(Node *p,double alfa, double beta,int level){
	if(p->n_sons==0) 
  		return(functionHeur(p->board));
  			p->value=-Inf;

  	double v1;
	for(int i=0;i<p->n_sons;i++){

		p->sons[i]=createNode(p,i,level+1);
    	v1=Min_Value(p->sons[i],alfa,beta,level+1);

    	if(v1>p->value) 
			p->value=v1;

    	if(v1>=beta){
      		double v=p->value;
      		free(p->sons[i]->sons);
      		free(p->sons[i]);
      		return v;
    	}

    	if(v1>alfa)  
			alfa=v1;

    	if(level!=0){// no matar a los sons de la root, porque se necesita luego
      		free(p->sons[i]->sons);
      		free(p->sons[i]);
    	}
  	}

  	return p->value;    

}

double Min_Value(Node *p,double alfa, double beta,int level){

	if(p->n_sons==0) 
		return(functionHeur(p->board));

  	p->value=+Inf;
  	double v1;
	
  	for(int i=0;i<p->n_sons;i++){
    	p->sons[i]=createNode(p,i,level+1);
    	v1=Max_Value(p->sons[i],alfa,beta,level+1);
    	
		if(v1< p->value) 
			p->value=v1;
    	
		if(v1<= alfa){
      		double v=p->value;
      		free(p->sons[i]->sons);
      		free(p->sons[i]);
      		return v;
      }

    if(v1< beta)  
		beta=v1;

    free(p->sons[i]->sons);
    free(p->sons[i]);

  }

  return p->value;

}

Node *createNode(Node *father, int numSon, int level){

  Node *p=malloc(sizeof(Node));
  copyBoard(p->board,father->board);
  aplicarTirada(p->board,numSon,level);//numSon es el numero del son, que va de 0 a ...

  if(level<K){
    p->n_sons=numSons(p->board);
    p->sons=malloc(p->n_sons*sizeof(Node*));
  }
  else{
    p->n_sons=0;
    p->sons= NULL;//malloc(0);
  }

  return p;

}

Node *createRoot(char table[N][N]){

  Node *p=malloc(sizeof(Node));
  copyBoard(p->board,table);
  p->n_sons=numSons(p->board);
  p->sons=malloc(p->n_sons*sizeof(Node*));
  
  return p;

}

void showValue(Node *p,int level) {

    for(int i=0;i<level;i++)
        printf("\t");

   printf("%f\n",p->value);

}

//inicion function heur
int checkLineHeur(int a1, int a2, int a3, int a4, int player, int lengthConnect){
    
	switch (lengthConnect) {
    	case 2: // escribir mas cosas para penalizar 2 en raya y un vacio al lado
            return (a1==player && a2==player && a3==0) 
				+ (a2==player && a3==player && a4==0)
				+ (a1==0 && a2==player && a3==player) 
				+ (a2==0 && a3==player && a4==player);
        case 3:
            return (a1==player && a2==player && a3==player && a4==0) 
				+ (a1==0 && a2==player && a3==player && a4==player);
        case 4:
            return (a1==player && a2==player && a3==player && a4==player);
        }

    return 0;//pos si a caso

}


int resultTableHeur(char table[N][N], int player, int lengthConnect){// player 1 1, player 2 2, empate 3, nada 4
  int i, j;
  int p=0; 

  for(j=0;j<N;j++)
    for(i=0;i<=N-4;i++){
        p+=checkLineHeur(// por columns
			table[i][j],
			table[i+1][j],
			table[i+2][j],
			table[i+3][j],
			player,
			lengthConnect);
        p+=checkLineHeur(// por filas
			table[j][i],
			table[j][i+1],
			table[j][i+2],
			table[j][i+3],
			player,
			lengthConnect);
      }

  	for( i=0;i<=N-4;i++)// por diagonal inclinado abajo
    	for( j=0;j<=N-4;j++)
        	p+=checkLineHeur(
				table[i][j],
				table[i+1][j+1], 
				table[i+2][j+2], 
				table[i+3][j+3],
				player,
				lengthConnect);

  	for(i=3;i<N;i++) // por diagonal inclinado arriba
   		for( j=0;j<=N-4;j++)
       		p+=checkLineHeur(
				   table[i][j],
				   table[i-1][j+1], 
				   table[i-2][j+2], 
				   table[i-3][j+3],
				   player,
				   lengthConnect);

 	return p;

}

int functionHeur(char table[N][N]){
  
	if(K==2){
    	if(resultTableHeur(table,2,4)) 
			return 100000;
    	else if(resultTableHeur(table,1,4)) 
			return -100000*resultTableHeur(table,1,4);
  	}
  
  	if(resultTableHeur(table,1,4)) //si el humano hace 4 en raya
  		return -100000*resultTableHeur(table,1,4);
  	else if(resultTableHeur(table,2,4)) // si el ordenador hace 4 en raya
  		return 100000;

  	int p=0;
    p+=resultTableHeur(table,2,3)*10;
    //penalizar si el humano tien 3 en raya
    p+=resultTableHeur(table,1,3)*(-10);
    p+=resultTableHeur(table,2,2)*2;
    //penalizar si el humano tien 2 en raya
    p+=resultTableHeur(table,1,2)*(-2);
 
 	return p;

}
//fin function heur

int tossRoot(Node *p){// que column tirar despues del minimax
  
  	double m;
  	int j=0;
  	m=p->sons[0]->value;

  	for(int i=1;i<p->n_sons;i++){
		if(p->sons[i]->value>m){
      		m=p->sons[i]->value;
      		j=i;
    	}
  	}
  	// el son numero j que column corresponde de p->board?
  	return numSonToColumn(p->board, j);

}
