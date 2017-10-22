#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#define SIZE 4
typedef struct TABLE{
	int tile[SIZE][SIZE];
}Table;

enum DIRECTION{
	NONE = 0,
	UP = 1,
	RIGHT = 2,
	DOWN = 3,
	LEFT = 4
}Direction;
/*
	 0  1  2  3

0	01 02 03 04
1	05 06 07 08
2	09 10 11 12
3	13 14 15 00
*/
int HammingDist(Table* tab){
	int dist = 0;
	for (int i = 0; i < SIZE; ++i){
		for (int j = 0; j < SIZE; ++j){
			if(tab->tile[i][j] != ((i*SIZE) + (j+1))%16){
				dist++;
			}
		}
	}
	return dist;
}


int ManhattanDist(Table* tab){
	int dist = 0;
	int aux;
	for (int i = 0; i < SIZE; ++i){
		for (int j = 0; j < SIZE; ++j){
			aux = tab->tile[i][j] - 1;
			if(aux>=0){
				dist += abs(i - (aux / SIZE)) + abs(j - (aux % SIZE)); 
			}else{
				dist += abs(i - 3) + abs(j - 3); 
			}
		}
	}
	return dist;
}

bool TileSwap(Table* tab, int i, int j, int y, int x){
	if(0 > i && i >= SIZE
	&& 0 > j && j >= SIZE
	&& 0 > y && y >= SIZE
	&& 0 > x && x >= SIZE ){return false;}

	int aux = tab->tile[i][j];
	tab->tile[i][j] = tab->tile[y][x];
	tab->tile[y][x] = aux;
	return true;
}

char* A_Estrela(Table* tab, int origin, int step){
	
	return NULL;
}


int main (int argc, char *argv[]) {
	int N_tables = 0;
	scanf("%d", &N_tables);

	Table* table_vector = (Table*)calloc(N_tables, sizeof(Table));

	for (int n = 0; n < N_tables; n++){
		for (int i = 0; i < SIZE; i++){	
			for (int j = 0; j < SIZE; j++){
				scanf("%d", &table_vector[n].tile[i][j]);
			}
		}	
	}
	



	printf("\n\nImprimindo as tables\n");
	for (int n = 0; n < N_tables; n++){
		printf("\nTable[%d] dsitancia :%d\n\t", n, ManhattanDist(&table_vector[n]));
		for (int i = 0; i < SIZE; i++){	
			for (int j = 0; j < SIZE; j++){
				printf("%d ", table_vector[n].tile[i][j]);
			}
			printf("\n\t");
		}	
	}
	return 0;
}