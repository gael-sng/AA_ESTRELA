#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>
#include <queue>

#define SIZE 4

struct Table{
	char tile[SIZE][SIZE];
	short empty_x, empty_y;
};

enum Direction{
	NONE = 0,
	UP = 1,
	RIGHT = 2,
	DOWN = 3,
	LEFT = 4
};

struct Node{
	Table t;
	//total cost of this state
	int cost;
	//number of steps to reach this state
	int steps;
	//last movement that led to this state
	Direction last;

	bool operator<(const Node& rhs) const
	{
		return(cost > rhs.cost);
	}

	Node(Table tab, int cst, int stps, Direction dir){
		t = tab;
		cost = cst;
		steps = stps;
		last = dir;
	}
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

//No longer a recursive function, bois
string solves(Table t, int steps){

	//creates a priority queue for storing the possibilities
	priority_queue<Node, vector<Node>> q;

	//pushing the first Node
	Node n(t,ManhattanDist(t),0,NONE);
	q.push(n);
	
	while(!q.empty()){

		//removing lowest cost path
		n = q.top();
		q.pop();

		if(n.cost >= steps) continue;

		//checking if we found the solution
		if(ManhattanDist(n.t) == 0){
			printTable(n.t);
			break;
		};

		//didnt found, lets keep searching
		//for each direction
		if(valid(n.t,UP) && n.last != DOWN){
			Node new_node(slide(n.t, UP), 
				ManhattanDist(new_node.t) + n.steps+1,
				 n.steps+1,
				  UP);
			q.push(new_node);
		}
		if(valid(n.t,DOWN) && n.last != UP){
			Node new_node(slide(n.t, DOWN),
			 ManhattanDist(new_node.t) + n.steps+1,
			  n.steps+1,
			   DOWN);
			q.push(new_node);
		}
		if(valid(n.t,LEFT) && n.last != RIGHT){
			Node new_node(slide(n.t, LEFT),
			 ManhattanDist(new_node.t) + n.steps+1,
			  n.steps+1,
			   LEFT);
			q.push(new_node);
		}
		if(valid(n.t,RIGHT) && n.last != LEFT){
			Node new_node(slide(n.t, RIGHT),
			 ManhattanDist(new_node.t) + n.steps+1,
			  n.steps+1,
			   RIGHT);
			q.push(new_node);
		}


		/*	The cost f(x) of a state is given by f(x) = g(x) + h(x)
			g(x) = Manhattan distance from the correct game table for that state
			h(x) = Number of steps to reach this state from the initial state*/
	}

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

		sol = solves(t,max_steps);
	}

	return 0;
}