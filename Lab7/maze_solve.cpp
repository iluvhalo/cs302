// Matt Matto
// Lab 7 -- maze_solve.cpp
// takes on std input the dimensions and walls of a maze to build a graph
// then it solves the maze using Depth First Search

#include <cstdio>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

// Node class that I use to represent each box in the maze grid
class Node {
	public:
		int id;
		set <int> edges;
		int visited;
};

// graph that holds the collection of Nodes to build and solve a maze
class Graph {
	public:
		vector <Node *> nodes;
		void makeGraph(int r, int c);
		void makeMaze(int r, int c);
		int DFS(int index, vector <int> &v, int end);
};


int main () {
	int rows, cols;
	int i;
	string garbage;
	Graph g;
	Node *n;
	vector <int> path;

	cin >> garbage;
	cin >> rows;
	cin >> garbage;
	cin >> cols;																

	// prints the output header
	printf("ROWS %d COLS %d\n", rows, cols);

	if (rows * cols == 1) {
		printf("PATH 0\n");
		return 0;
	}

	// makes a fully connected grid of vertices
	g.makeGraph(rows, cols);

	// breaks the edges between vertices where there is a wall
	g.makeMaze(rows, cols);

	// does a Depth First Search to solve the maze
	g.DFS(0, path, rows*cols - 1);

	// if a solution is found, prints the path to the end
	if (path.size() > 0) {
		printf("PATH 0\n");
		for (i = path.size() - 1; i >= 0; i--) {
			printf("PATH %d\n", path[i]);
		}
	}

	return 0;
}

void Graph::makeGraph (int r, int c) {
	int i;
	Node *nodeP;
	Node *n;								

	// creates an unconnected graph of size r*c
	for (i = 0; i < r * c; i++) {
		nodeP = new Node;
		nodeP->id = i;
		nodeP->visited = 0;

		nodes.push_back(nodeP);
	}																		

	// connects the vertices of the graph to make a grid of edges between them
	// this works vary similarly to superball
	for (i = 0; i < r * c - 1; i++) {
		nodeP = nodes[i];							

		// adds an edge between the current vertex and the vertex to the right as long it does
		// not extend beyond the end of the nodes vector and it does not belong in the next row
		if ((i + 1 <= (r*c - 1)) and ((i + 1) % c != 0)) {	
			n = nodes[i + 1];
			nodeP->edges.insert(i + 1);
			n->edges.insert(i);
		}

		// adds and edge between the current vertex and the vertex below it as long as
		// the one below it does not point beyond the end of the vector
		if (i + c <= (r*c - 1)) {								
			n = nodes[i + c];
			nodeP->edges.insert(i + c);
			n->edges.insert(i);
		}
	}

	return;		
}

void Graph::makeMaze (int r, int c) {
	string command;
	Node *n1;
	Node *n2;
	int index1;
	int index2;
	set <int>::iterator sit;

	// disconnects vertices where there is a wall
	while (cin >> command >> index1 >> index2) {
		printf("%s %d %d\n", command.c_str(), index1, index2);

		n1 = nodes[index1];
		n2 = nodes[index2];

		sit = n1->edges.find(index2);
		n1->edges.erase(sit);

		sit = n2->edges.find(index1);
		n2->edges.erase(sit);
	}

	return;
}


// does a depth first search on the maze until it finds a path to the end
int Graph::DFS (int index, vector <int> &v, int end) {
	Node *n;
	set <int>::iterator sit;
	int i;
	int retval;

	n = nodes[index];

	// base cases
	if (n->visited == 1) return 0;
	if (index == end) return 1;

	n->visited = 1;

	// iterate though the edges of n until we find the end
	// after finding the end, I build a path vector to return to main
	for (sit = n->edges.begin(); sit != n->edges.end(); sit++) {
		retval = DFS(*sit, v, end);
		if (retval == 1) {
			v.push_back(*sit);
			return 1;
		}
	}

	return 0;	
}
