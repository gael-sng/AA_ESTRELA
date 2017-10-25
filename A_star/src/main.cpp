#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>
#include <queue>
#include <time.h>

using namespace std;


#define SIZE 4

typedef unsigned char u_char;

struct Table{
 	u_char tile[SIZE][SIZE];
};

#define NONE 'N'
#define UP 'U'
#define RIGHT 'R'
#define DOWN 'D'
#define LEFT 'L'


struct Node{
	Table t;
	//total cost of this state
	u_char cost;
	
	//number of steps to reach this state
	string steps;

	bool operator<(const Node& rhs) const
	{
		return(cost > rhs.cost);
	}

	Node(Table tab, int cst, string stps){
		t = tab;
		cost = cst;
		steps = stps;
	}
};

//picks return values of scanf. This variable only exists to supress compiler warnings.
int garbage;


/*
	 0  1  2  3
0	01 02 03 04
1	05 06 07 08
2	09 10 11 12
3	13 14 15 16 
*/



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

int ManhattanDist(Table tab){
	int dist = 0;
	int aux;
	for (int i = 0; i < SIZE; ++i){
		for (int j = 0; j < SIZE; ++j){
			aux = tab.tile[i][j] - 1;
			if(aux>=0){
				dist += abs(i - (aux / SIZE)) + abs(j - (aux % SIZE)); 
			}else{
				dist += abs(i - 3) + abs(j - 3); 
			}
		}
	}
	return dist;
}

bool FindZero(Table t, int* y, int* x){
	for (int i = 0; i < SIZE; ++i){
		for (int j = 0; j < SIZE; ++j){
			if (t.tile[i][j] == 0){
				*y = i;
				*x = j;
				return true;
			}
		}
	}
	return false;
}

/*Checks if sliding the empty tile to a 
	direction 'd' is valid on table 't'*/
bool valid(Table t, char d){
	int x, y;
	if(!FindZero(t, &y, &x))return false;

	switch (d){
		case UP:
			return (y - 1 >= 0);
		case DOWN:
			return (y + 1 < SIZE);
		case LEFT:
			return (x - 1 >= 0);
		case RIGHT:
			return (x + 1 < SIZE);
		default:
			return false;
	}
}

/* Slides the empty tile in game table 't'
	to a direction 'd'
	NOTE: This function does not checks if move is valid
	Please check using valid(t,d) before calling this*/
Table slide(Table t, char d){
	int empty_x, empty_y;
	if(!FindZero(t, &empty_y, &empty_x)){
		printf("ERRO na função slide, Table passada não possui 0 nela\n");
		return t;
	}

	int aux;
	switch (d){

		case UP:
			//saves the value above the empty tile
			aux = t.tile[empty_y-1][empty_x];
			//moves empty tile up
			t.tile[empty_y-1][empty_x] = 0;
			//moves saved value to where the empty tile was
			t.tile[empty_y][empty_x] = aux;

			//updates t to have the new position of the empty tile
			empty_y = empty_y-1;
			break;

		case DOWN:
			//saves the value below the empty tile
			aux = t.tile[empty_y+1][empty_x];
			//moves empty tile down
			t.tile[empty_y+1][empty_x] = 0;
			//moves saved value to where the empty tile was
			t.tile[empty_y][empty_x] = aux;

			//updates t to have the new position of the empty tile
			empty_y = empty_y+1;
			break;

		case LEFT:
			//saves the value on right of the empty tile
			aux = t.tile[empty_y][empty_x-1];
			//moves empty tile down
			t.tile[empty_y][empty_x-1] = 0;
			//moves saved value to where the empty tile was
			t.tile[empty_y][empty_x] = aux;

			//updates t to have the new position of the empty tile
			empty_x = empty_x-1;
			break;

		case RIGHT:
			//saves the value on right of the empty tile
			aux = t.tile[empty_y][empty_x+1];
			//moves empty tile down
			t.tile[empty_y][empty_x+1] = 0;
			//moves saved value to where the empty tile was
			t.tile[empty_y][empty_x] = aux;

			//updates t to have the new position of the empty tile
			empty_x = empty_x+1;
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
			printf("%-2d ", t.tile[i][j]);
		}
		printf("\n");
	} 	
}

/* Reads a table from STDIN and return it*/
Table readTable(){
	Table t;
	int aux;
	for (int i = 0; i < SIZE; i++){	
		for (int j = 0; j < SIZE; j++){

			//reads Table tiles 
			garbage = scanf("%d", &aux);
			t.tile[i][j] = (u_char)aux;
		}
	}

	return t;
}

int cost(Node n, char d/*, int limit*/){
	return (ManhattanDist(slide(n.t,d))/2) * (n.steps.size()+1);
}

string astar(Table t, unsigned  int Max_steps){

	//creates a priority queue for storing the possibilities
	priority_queue<Node, vector<Node>> q;

	//pushing the first Node
	q.push( Node(t,ManhattanDist(t),"") );
	
	//limit for our heuristic
	// limit = ManhattanDist(t);

	while(!q.empty()){

		//removing lowest cost path
		Node n = q.top();
		q.pop();

		//checking if we found the solution
		if(ManhattanDist(n.t) == 0){
			//printing here means we found the solution
			return n.steps;
		};

		/*I should stop searching when my cost possibilites are higher than
		the allowed number of steps */
		if(n.steps.size() > Max_steps) continue;

        //didnt found, lets keep searching         //for each direction
		if(valid(n.t,UP) && n.steps.back() != DOWN )             
			q.push( Node(slide(n.t, UP), cost(n, UP), n.steps + UP) );
		 
		if(valid(n.t,DOWN) && n.steps.back() != UP )
			q.push( Node(slide(n.t, DOWN), cost(n, DOWN), n.steps + DOWN) );
		
		if(valid(n.t,LEFT) && n.steps.back() != RIGHT )
			q.push( Node(slide(n.t, LEFT), cost(n, LEFT), n.steps + LEFT) );
		
		if(valid(n.t,RIGHT) && n.steps.back() != LEFT )
			q.push( Node(slide(n.t, RIGHT), cost(n, RIGHT), n.steps + RIGHT) );
		


		/*	The cost f(x) of a state is given by f(x) = g(x) + h(x)
			g(x) = Manhattan distance from the correct game table for that state
			h(x) = Number of steps to reach this state from the initial state*/
	}

	return "";
}

// This function returns true if given
// instance of N*N - 1 puzzle is solvable
bool isSolvable(Table t){
	int count = 0;
	for (int i = 0; i < (SIZE*SIZE); i++){
		for (int x = i +1; x < (SIZE*SIZE); x++){
			if(t.tile[x/SIZE][x%SIZE] && t.tile[i/SIZE][i%SIZE] && t.tile[i/SIZE][i%SIZE] > t.tile[x/SIZE][x%SIZE]){
				count++;
			}
		}
	}
	int line = 0;
	FindZero(t, &line, &garbage);
    if (line & 1)
        return !(count & 1);
    return count & 1;
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
		// printTable(t);

		//problem solution is in this string
		string sol = "";
		
		clock_t c_inicial = clock();
		if(isSolvable(t))sol = astar(t,max_steps);
		clock_t c_final = clock();

		if(sol.size() > 0){
			cout << " \nTempo de execução: ";
			cout << ((c_final - c_inicial)/(double)CLOCKS_PER_SEC) << endl;
			cout << sol << endl;

		}else{
			cout << "This puzzle is not solvable." << endl;
			
		}
	}

	return 0;
}