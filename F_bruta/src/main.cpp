#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>

#define SIZE 4

struct Table{
	int tile[SIZE][SIZE];
	int empty_x, empty_y;
};

enum Direction{
	NONE = 0,
	UP = 1,
	RIGHT = 2,
	DOWN = 3,
	LEFT = 4
};

//picks return values of scanf. This variable only exists to supress compiler warnings.
int garbage;

using namespace std;

/*
	 0  1  2  3
0	01 02 03 04
1	05 06 07 08
2	09 10 11 12
3	13 14 15 16
*/

/* Calculates Hamming Distance*/
int HammingDist(Table tab){
	int dist = 0;
	for (int i = 0; i < SIZE; ++i){
		for (int j = 0; j < SIZE; ++j){
			if(tab.tile[i][j] != ((i*SIZE) + (j+1))%16){
				dist++;
			}
		}
	}
	return dist;
}


/*Checks if sliding the empty tile to a 
	direction 'd' is valid on table 't'*/
bool valid(Table t, Direction d){
	switch (d){
		case UP:
			return (t.empty_y - 1 >= 0);
		case DOWN:
			return (t.empty_y + 1 < SIZE);
		case LEFT:
			return (t.empty_x - 1 >= 0);
		case RIGHT:
			return (t.empty_x + 1 < SIZE);
		default:
			return false;
	}
}

/* Slides the empty tile in game table 't'
	to a direction 'd'
	NOTE: This function does not checks if move is valid
	Please check using valid(t,d) before calling this*/
Table slide(Table t, Direction d){

	int aux;
	switch (d){

		case UP:
			//saves the value above the empty tile
			aux = t.tile[t.empty_y-1][t.empty_x];
			//moves empty tile up
			t.tile[t.empty_y-1][t.empty_x] = 0;
			//moves saved value to where the empty tile was
			t.tile[t.empty_y][t.empty_x] = aux;

			//updates t to have the new position of the empty tile
			t.empty_y = t.empty_y-1;
			break;

		case DOWN:
			//saves the value below the empty tile
			aux = t.tile[t.empty_y+1][t.empty_x];
			//moves empty tile down
			t.tile[t.empty_y+1][t.empty_x] = 0;
			//moves saved value to where the empty tile was
			t.tile[t.empty_y][t.empty_x] = aux;

			//updates t to have the new position of the empty tile
			t.empty_y = t.empty_y+1;
			break;

		case LEFT:
			//saves the value on right of the empty tile
			aux = t.tile[t.empty_y][t.empty_x-1];
			//moves empty tile down
			t.tile[t.empty_y][t.empty_x-1] = 0;
			//moves saved value to where the empty tile was
			t.tile[t.empty_y][t.empty_x] = aux;

			//updates t to have the new position of the empty tile
			t.empty_x = t.empty_x-1;
			break;

		case RIGHT:
			//saves the value on right of the empty tile
			aux = t.tile[t.empty_y][t.empty_x+1];
			//moves empty tile down
			t.tile[t.empty_y][t.empty_x+1] = 0;
			//moves saved value to where the empty tile was
			t.tile[t.empty_y][t.empty_x] = aux;

			//updates t to have the new position of the empty tile
			t.empty_x = t.empty_x+1;
			break;

		default:
			break;

	}
	return t;
}

/* Prints a Table t*/
void printTable(Table t){
	//testing
	for (int i = 0; i < SIZE; i++){	
		for (int j = 0; j < SIZE; j++){
			printf("%d ", t.tile[i][j]);
		}
		printf("\n");
	} 	
}

/* Reads a table from STDIN and return it*/
Table readTable(){
	Table t;
	for (int i = 0; i < SIZE; i++){	
		for (int j = 0; j < SIZE; j++){

			//reads Table tiles 
			garbage = scanf("%d", &t.tile[i][j]);

			//marks position of empty tile
			if(t.tile[i][j] == 0){
				t.empty_y = i;
				t.empty_x = j;
			}
		}
	}

	return t;
}

bool solves(Table t, string& sol, int steps, char last){
	// printf("Currently on step %d\n",steps);
	//checks if we found the solution
	if(HammingDist(t) == 0){
		printTable(t);
		return true;
	}

	//checks if we searched for too long and didn't found the answer
	if(steps <= 0) return false;

	//didn't returned yet, so let's search more
	if(valid(t,DOWN) && last != 'U'){
		if(solves(slide(t,DOWN),sol,steps-1,'D')){
			sol += 'D';
			return true;
		}
	}
	if(valid(t,UP) && last != 'D'){
		if(solves(slide(t,UP),sol,steps-1,'U')){
			sol += 'U';
			return true;
		}
	}
	if(valid(t,RIGHT) && last != 'L'){
		if(solves(slide(t,RIGHT),sol,steps-1,'R')){
			sol += 'R';
			return true;
		}
	}
	if(valid(t,LEFT) && last != 'R'){
		if(solves(slide(t,LEFT),sol,steps-1,'L')){
			sol += 'L';
			return true;
		}
	}
	return false;
}

int main (int argc, char *argv[]) {
	int n_games = 0;
	int max_steps;

	if(argc <= 1){
		max_steps = 50;
	} else {
		max_steps = atoi(argv[1]);
	}
	//creating a table
	Table t;

	//Reads the number of games available
	garbage = scanf("%d", &n_games);
	for (int n = 0; n < n_games; n++){
		/*For each game, reads the game table,
			tries to solve it and prints the result*/

		//reading game table
		t = readTable();
		printTable(t);

		//problem solution is in this string
		string sol = "";

		solves(t,sol, max_steps, ' ');

		//prints result
		reverse(sol.begin(),sol.end());
		cout << (sol.size() > 0 ? sol: "This puzzle is not solvable.") << endl;
	}

	return 0;
}