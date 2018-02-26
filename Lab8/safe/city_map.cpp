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
int Find_Road_Number (Intersection *point1, Intersection *point2);								// returns what road the two intersections are on
Road_Type Find_Road_Type (Intersection *point1, Intersection *point2);
int Max_Street_Number (list <Intersection *> &all);														// finds the maximum street number
int Max_Avenue_Number (list <Intersection *> &all);														// finds the maximum avenue number
Direction Find_1Way_Direction (Intersection *point1, Intersection *point2);					// Finds which way the one way road goes.
Intersection *Find_Intersection(list <Intersection *> &all, const int &street, const int &avenue);			// returns a pointer to the given intersection coordinates
int Find_Road_Segment(Intersection *point1, Intersection *point2);								// finds if point2 is in point1's adj list 
void Create_Road(list <Intersection *> &all, Intersection *point1, Intersection *point2);	// joins two intersections according to the rules of road segments
double Max_Red_Time(Intersection *n, Road_Segment *backedge);
double Avg_Red_Time (Intersection *n, Road_Segment *backedge);

City_Map::City_Map () {
	int street, avenue;
	int i, j;
	double x, y, greenStreet, greenAvenue;
	Intersection *iPtr;
	Intersection *nPtr, *sPtr, *ePtr, *wPtr;
	Road_Segment *rPtr;
	list <Intersection *>::iterator lit;

	while (cin >> street >> avenue >> x >> y >> greenStreet >> greenAvenue) {
		iPtr = new Intersection;

		iPtr->street = street;
		iPtr->avenue = avenue;
		iPtr->x = x;
		iPtr->y = y;
		iPtr->green[STREET] = greenStreet;
		iPtr->green[AVENUE] = greenAvenue;
		iPtr->backedge = NULL;

		all.push_back(iPtr);
	}

	first = Find_Intersection(all, 0, 0);
	first->backedge = NULL;
	last = Find_Intersection(all, Max_Street_Number(all), Max_Avenue_Number(all));

	for (street = 0; street <= Max_Street_Number(all); street++) {
		for (avenue = 0; avenue <= Max_Avenue_Number(all); avenue++) {
			iPtr = Find_Intersection(all, street, avenue);

			if (iPtr->street != -1) {
				// setting the north, south, east, and west pointers
				nPtr = Find_Intersection(all, street + 1, avenue);
				sPtr = Find_Intersection(all, street - 1, avenue);
				ePtr = Find_Intersection(all, street, avenue - 1);
				wPtr = Find_Intersection(all, street, avenue + 1);

				// creating valid roads
				if (nPtr->street != -1) Create_Road(all, iPtr, nPtr); 
				if (sPtr->street != -1) Create_Road(all, iPtr, sPtr);
				if (ePtr->street != -1) Create_Road(all, iPtr, ePtr);
				if (wPtr->street != -1) Create_Road(all, iPtr, wPtr);
			} 
		}
	}
}

/*	Dijktra's
 *	start coding assuming best
 *	bfsj = breadth first search queue
 *	inserting the first intersection into bfsq
 *	look at the first intersection in the queue
 *		all of the intesections it can travel to
 *		and calculate the time it would take to travel to those intersections (and potentionally wait; onlyt during avg or worst case)
 *		fill in the backedge
 *		insert the next intersection into bfsq IF it's not been visited OR it is faster than an other path to it
 *		insert returns an iterator to the item inserted, assign this iterator to point to bfsq_ptr
 *
 *	find the fastest path by traversing the backedges
 *	fill in the beckedges into path
 *	return the time of the path
 */

double City_Map::Dijkstra (int avg_best_worst) {
	Intersection *current;
	multimap <double, Intersection *>::iterator mit;
	multimap <double, Intersection *>::reverse_iterator rmit;
	list <Intersection *>::iterator lit;
	list <Road_Segment *>::iterator rit;
	double f, retval, time;
	Intersection *n, *t;
	int i;
	Road_Segment *r;

	// iterates through all and sets each backedge to NULL and best_time to -1
	for (lit = all.begin(); lit != all.end(); lit++) {
		(*lit)->backedge = NULL;
		(*lit)->best_time = -1;
	}

	first->best_time = 0.0;
	first->bfsq_ptr = bfsq.insert(make_pair(first->best_time, first));
	mit = bfsq.begin();

	// pull things off the queue
	while (!bfsq.empty()) {

		// erase the first element
		f = bfsq.begin()->first;
		n = bfsq.begin()->second;
		bfsq.erase(bfsq.begin());

		// if we have reached the end of the thing
		if (n == last) {
			while (n != first) {
				path.push_front(n->backedge);
				n = n->backedge->from;
			}

			return f;
		}

		for (rit = n->adj.begin(); rit != n->adj.end(); rit++) {
			r = (*rit);
			t = r->to;


			// if not visited
			if (t->best_time == -1) {
				
				//calculate the best time and set the backedge
				t->best_time = f + (r->distance / 30.0) * 3600;	// calculates the best time
				t->backedge = r;									// fill in the back edge
				
				// if looking for the worst case, add the red light time
				if (avg_best_worst == 'W') {
					t->best_time += Max_Red_Time(t, r);
				} else if (avg_best_worst == 'A') {
					t->best_time += Avg_Red_Time(t, r);
				}
				
				//insert it into the queue
				t->bfsq_ptr = bfsq.insert(make_pair(t->best_time, t));
			
			// else it has been visited
			} else {					

				time = f + (r->distance / 30.0) * 3600;
				if (avg_best_worst == 'W') {
					time += Max_Red_Time(t, r);
				} else if (avg_best_worst == 'A') {
					time += Avg_Red_Time(t, r);
				}
				
				if (t->best_time > time) { //if the current path is faster than the path on the queue
					
					bfsq.erase(t->bfsq_ptr);
					t->best_time = time;
					t->backedge = r;
					
					t->bfsq_ptr = bfsq.insert(make_pair(t->best_time, t));
				}
			}
		}
	}

	return 0;
}

double Find_Distance (Intersection *point1, Intersection *point2) {
	double deltaX, deltaY;

	deltaX = point1->x - point2->x;
	deltaY = point1->y - point2->y;

	return sqrt(pow(deltaX, 2) + pow(deltaY, 2));
}

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

int Max_Street_Number (list <Intersection *> &all) {
	list <Intersection *>::iterator lit;

	int max = 0;

	for (lit = all.begin(); lit != all.end(); lit++) {
		if ((*lit)->street > max) max = (*lit)->street;
	}

	return max;
}

int Find_Road_Number (Intersection *point1, Intersection *point2) {
	if (point1->street == point2->street) {
		return point1->street;
	} else if (point1->avenue == point2->avenue) {
		return point1->avenue;
	}
}

int Max_Avenue_Number (list <Intersection *> &all) {
	list <Intersection *>::iterator lit;

	int max = 0;

	for (lit = all.begin(); lit != all.end(); lit++) {
		if ((*lit)->avenue > max) max = (*lit)->avenue;
	}

	return max;
}

Road_Type Find_Road_Type (Intersection *point1, Intersection *point2) {
	if (point1->street == point2->street) {
		return STREET;
	} else {
		return AVENUE;
	}
}

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

Intersection *Find_Intersection(list <Intersection *> &all, const int &street, const int &avenue) {
	list <Intersection *>::iterator lit;
	Intersection *DNE;

	for (lit = all.begin(); lit != all.end(); lit++) {
		if (((*lit)->street == street) and ((*lit)->avenue == avenue)) {
			return (*lit);
		}
	}

	DNE = new Intersection;
	DNE->street = -1;
	DNE->avenue = -1;

	return DNE;
}

int Find_Road_Segment(Intersection *point1, Intersection *point2) {							// finds if point2 is in point1's adj list 
	list <Road_Segment *>::iterator lit;

	for (lit = point1->adj.begin(); lit != point1->adj.end(); lit++) {
		if ((*lit)->to == point2) return 1;
	}

	return 0;
}

void Create_Road(list <Intersection *> &all, Intersection *point1, Intersection *point2) {
	Road_Segment *rPtr;

	if (point1->street == -1 or point2->street == -1) return;	

	// if the road is a 2-way road
	if (((Find_Road_Type(point1, point2) == AVENUE) and (Find_Road_Number(point1, point2) == Max_Avenue_Number(all))) or (Is_2Way(point1, point2))) {
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

double Max_Red_Time(Intersection *n, Road_Segment *backedge) {

	if (backedge->type == STREET) {
		return n->green[AVENUE];
	} else {
		return n->green[STREET];
	}

}

double Avg_Red_Time (Intersection *n, Road_Segment *backedge) {
	if (backedge->type == STREET) {
		return (pow(n->green[AVENUE], 2) / ( 2 * (n->green[AVENUE] + n->green[STREET])));
	} else {
		return (pow(n->green[STREET], 2) / ( 2 * (n->green[AVENUE] + n->green[STREET])));
	}
}
