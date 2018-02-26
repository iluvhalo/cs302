// Matt Matto
// Lab 8 - city_map.cpp
//
// takes in Intersection data on standard input and finds the best path from one corner to the opposite using Dijktra's algorithm
// It can also calculate the worst possible time and the average time based on the red light times of the intersections

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include "city_map.h"

using namespace std;

typedef enum {	EAST, NORTH, WEST, SOUTH } Direction;

double Find_Distance (Intersection *point1, Intersection *point2);								// returns the length of the road between the two Intersections
int Is_2Way (Intersection *point1, Intersection *point2);											// checks if the edge between the 2 intesections is bidirectional
int Find_Road_Number (Intersection *point1, Intersection *point2);								// returns what road the two intersections both share
Road_Type Find_Road_Type (Intersection *point1, Intersection *point2);							// returns what type of road is between the two points
Direction Find_1Way_Direction (Intersection *point1, Intersection *point2);					// Finds which way the one way road goes.
int Find_Road_Segment(Intersection *point1, Intersection *point2);								// finds if point2 is in point1's adj list 
void Create_Road(vector < vector  <Intersection *> > &map, Intersection *point1, Intersection *point2);	// joins two intersections according to the rules of road segments
double Max_Red_Time(Intersection *n, Road_Segment *backedge);										// calculates the red light time depending on the direction traveling
double Avg_Red_Time (Intersection *n, Road_Segment *backedge);										// calculates the average red light time

City_Map::City_Map () {

	int street, avenue;											// used for filling in the data for each Intersection, then used as iterators for my for loops
	double x, y, greenStreet, greenAvenue;					// used for filling in the data for each Intersection
	Intersection *iPtr;											// pointer to my current Intersection
	Intersection *nPtr, *sPtr, *ePtr, *wPtr;				// pointers to the Intersections to the NORTH, SOUTH, EAST, and WEST of iPtr
	list <Intersection *>::iterator lit;					// iterator for all
	vector < vector < Intersection *> > map;				// temporary 2-D vector to hold my Intersections

	// while getting input, makes new Intersections and pushes them onto all
	while (cin >> street >> avenue >> x >> y >> greenStreet >> greenAvenue) {
		iPtr = new Intersection;

		iPtr->street = street;
		iPtr->avenue = avenue;
		iPtr->x = x;
		iPtr->y = y;
		iPtr->green[STREET] = greenStreet;
		iPtr->green[AVENUE] = greenAvenue;
	
		if (street <= map.size()) map.resize(street + 1);
		if (avenue <= map[street].size()) map[street].resize(avenue + 1);
		map[street][avenue] = iPtr;

		all.push_back(iPtr);
	}

	// assigns first and last to map[0][0] and map[max][max]
	first = map[0][0];
	last = map[map.size() - 1][map[map.size() - 1].size() - 1];
	
	// iterates through my map vector and builds the roads between each intersection, if it is supposed to exist
	for (street = 0; street <= last->street; street++) {
		for (avenue = 0; avenue <= last->avenue; avenue++) {
			iPtr = map[street][avenue];

			nPtr = NULL;
			sPtr = NULL;
			ePtr = NULL;
			wPtr = NULL;

			// setting the north, south, east, and west pointers
			if (street + 1 <= last->street)		nPtr = map[street + 1][avenue];
			if (street - 1 >= 0)						sPtr = map[street - 1][avenue];
			if (avenue - 1 >= 0)						ePtr = map[street][avenue - 1];
			if (avenue + 1 <= last->avenue)		wPtr = map[street][avenue + 1];

			// creating valid roads
			if (nPtr != NULL) Create_Road(map, iPtr, nPtr); 
			if (sPtr != NULL) Create_Road(map, iPtr, sPtr);
			if (ePtr != NULL) Create_Road(map, iPtr, ePtr);
			if (wPtr != NULL) Create_Road(map, iPtr, wPtr);
		}
	}
}

double City_Map::Dijkstra (int avg_best_worst) {

	list <Intersection *>::iterator lit;
	list <Road_Segment *>::iterator rit;
	double f;										// holds the best_time of the Intersection pulled off the queue
	double time;									// used for calculating the best_time of a visited Intersection
	Intersection *n, *t;							// pointers to the Intersection pulled off the queue(n) and the Intersection the Road_Segment points to(t)

	// iterates through all and sets each backedge to NULL and best_time to -1
	for (lit = all.begin(); lit != all.end(); lit++) {
		(*lit)->backedge = NULL;
		(*lit)->best_time = -1;
	}

	// initializes the starting Intersection and puts it on the queue
	first->best_time = 0.0;
	first->bfsq_ptr = bfsq.insert(make_pair(first->best_time, first));

	// pull things off the queue
	while (!bfsq.empty()) {

		// erase the first element
		f = bfsq.begin()->first;
		n = bfsq.begin()->second;
		bfsq.erase(bfsq.begin());

		// if we have reached the last intersection
		if (n == last) {
			while (n != first) {
				path.push_front(n->backedge);
				n = n->backedge->from;
			}

			return f;
		}

		// iterate through n's adj list
		for (rit = n->adj.begin(); rit != n->adj.end(); rit++) {
			t = (*rit)->to;

			// calculate time based on command line argument
			time = f + ((*rit)->distance / 30.0) * 3600;
			if (avg_best_worst == 'W') {
				time += Max_Red_Time(t, (*rit));
			} else if (avg_best_worst == 'A') {
				time += Avg_Red_Time(t, (*rit));
			}

			// if t is unvisited or the path found is faster 
			// than the path currently on the queue
			// I add it to the queue and remove the old one if it exists
			if ((t->best_time == -1) or (t->best_time > time)) {
				if (t->best_time > time) bfsq.erase(t->bfsq_ptr);
				t->best_time = time;
				t->backedge = (*rit);
				t->bfsq_ptr = bfsq.insert(make_pair(time, t));
			}
		}
	}

	return 0;
}

// textbook distance formula
double Find_Distance (Intersection *point1, Intersection *point2) {
	double deltaX, deltaY;

	deltaX = point1->x - point2->x;
	deltaY = point1->y - point2->y;

	return sqrt(pow(deltaX, 2) + pow(deltaY, 2));
}

// checks if a road is supposed to be a 2way road
// used in Create_Road()
int Is_2Way (Intersection *point1, Intersection *point2) {

	if (point1->street == point2->street) {
		if (point1->street % 5 == 0) {
			return 1;
		}
	} 	else if (point1->avenue == point2->avenue) {
		if (point1->avenue % 5 == 0) {
			return 1;
		}
	}

	return 0;
}

// finds the number of the road segment between the two points
// used in Create_Road()
int Find_Road_Number (Intersection *point1, Intersection *point2) {
	if (point1->street == point2->street) {
		return point1->street;
	} else if (point1->avenue == point2->avenue) {
		return point1->avenue;
	}
}

// returns what road type is the road between the two points
// used in Create_Road()
Road_Type Find_Road_Type (Intersection *point1, Intersection *point2) {
	if (point1->street == point2->street) {
		return STREET;
	} else {
		return AVENUE;
	}
}

// determines which way the one way road is supposed to go
// used in Create_Road()
Direction Find_1Way_Direction (Intersection *point1, Intersection *point2) {

	if (Find_Road_Type(point1, point2) == STREET) {
		if ((Find_Road_Number(point1, point2) % 2) == 0) {
			return WEST;
		} else return EAST;
	} else {
		if ((Find_Road_Number(point1, point2) % 2) == 0) {
			return NORTH;
		} else return SOUTH;
	}

}

// finds if there is a Road_Segment to point2 in point1's adj list
int Find_Road_Segment(Intersection *point1, Intersection *point2) { 
	list <Road_Segment *>::iterator lit;

	for (lit = point1->adj.begin(); lit != point1->adj.end(); lit++) {
		if ((*lit)->to == point2) return 1;
	}

	return 0;
}

// creates a road given the two intersections
// first it checks if the road segment should be two, as that is the easiest to test for
// then is checks if it is supposed to be one-way and what direction it is supposed to go
void Create_Road(vector < vector <Intersection *> > &map, Intersection *point1, Intersection *point2) {
	Road_Segment *rPtr;

	// if the road is a 2-way road
	if (((Find_Road_Type(point1, point2) == AVENUE) and (Find_Road_Number(point1, point2) == map[map.size() - 1].size() - 1)) or (Is_2Way(point1, point2))) {
		if (!(Find_Road_Segment(point1, point2))) {
			rPtr = new Road_Segment;

			rPtr->type = Find_Road_Type(point1, point2);
			rPtr->number = Find_Road_Number(point1, point2);
			rPtr->distance = Find_Distance(point1, point2);
			rPtr->from = point1;
			rPtr->to = point2;

			point1->adj.push_back(rPtr);
		}

		if (!(Find_Road_Segment(point2, point1))) {
			rPtr = new Road_Segment;

			rPtr->type = Find_Road_Type(point1, point2);
			rPtr->number = Find_Road_Number(point1, point2);
			rPtr->distance = Find_Distance(point1, point2);
			rPtr->from = point2;
			rPtr->to = point1;

			point2->adj.push_back(rPtr);
		}

		return;
	}

	// checks if there is supposed to be a NORTH road segment from point1 to point2
	if (Find_1Way_Direction(point1, point2) == NORTH) {
		if (point1->street < point2->street) {
			if (!(Find_Road_Segment(point1, point2))) {
				rPtr = new Road_Segment;

				rPtr->type = AVENUE;
				rPtr->number = Find_Road_Number(point1, point2);
				rPtr->distance = Find_Distance(point1, point2);
				rPtr->from = point1;
				rPtr->to = point2;

				point1->adj.push_back(rPtr);
			}
		}

		// checks if there is supposed to be a SOUTH road segment from point1 to point2
	} else if (Find_1Way_Direction(point1, point2) == SOUTH) {
		if (point1->street > point2->street) {
			if (!(Find_Road_Segment(point1, point2))) {
				rPtr = new Road_Segment;

				rPtr->type = AVENUE;
				rPtr->number = Find_Road_Number(point1, point2);
				rPtr->distance = Find_Distance(point1, point2);
				rPtr->from = point1;
				rPtr->to = point2;

				point1->adj.push_back(rPtr);
			}
		}

		// checks if there is supposed to be an EAST road segment from point1 to point2
	} else if (Find_1Way_Direction(point1, point2) == EAST) {
		if (point1->avenue > point2->avenue) {
			if (!(Find_Road_Segment(point1, point2))) {
				rPtr = new Road_Segment;

				rPtr->type = STREET;
				rPtr->number = Find_Road_Number(point1, point2);
				rPtr->distance = Find_Distance(point1, point2);
				rPtr->from = point1;
				rPtr->to = point2;

				point1->adj.push_back(rPtr);
			}
		}

		// and finally, checks if there is supposed to be a WEST road segment from point1 to point2
	} else if (Find_1Way_Direction(point1, point2) == WEST) {
		if (point1->avenue < point2->avenue) {
			if (!(Find_Road_Segment(point1, point2))) {
				rPtr = new Road_Segment;

				rPtr->type = STREET;
				rPtr->number = Find_Road_Number(point1, point2);
				rPtr->distance = Find_Distance(point1, point2);
				rPtr->from = point1;
				rPtr->to = point2;

				point1->adj.push_back(rPtr);
			}
		}
	}

	return;
}

// returns the red light time for the worst case calculation
double Max_Red_Time(Intersection *n, Road_Segment *backedge) {

	if (backedge->type == STREET) {
		return n->green[AVENUE];
	} else {
		return n->green[STREET];
	}

}

// returns the average red light time using Plank's formula: (R^2 / (2(R + G)))
double Avg_Red_Time (Intersection *n, Road_Segment *backedge) {
	if (backedge->type == STREET) {
		return (pow(n->green[AVENUE], 2) / ( 2 * (n->green[AVENUE] + n->green[STREET])));
	} else {
		return (pow(n->green[STREET], 2) / ( 2 * (n->green[AVENUE] + n->green[STREET])));
	}
}
