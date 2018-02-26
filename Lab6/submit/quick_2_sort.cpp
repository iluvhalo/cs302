// Matt Matto
// Lab 6 -- quick_2_sort.cpp
// sorts a vector of numbers using quick sort

#include <vector>
#include <cstdio>
#include <iostream>
#include "sorting.h"

using namespace std;

void recursive_sort (vector <double> &v, int start, int size, int print);
void find_pivot (vector <double> &v, int start, int size);							// find the pivot and places it at the front of the partition to sort
void debug_print(vector <double> &v);														// just prints the contents of a vector


// sets up and calls the recursive quicksort function
void sort_doubles (vector <double> &v, int print) {
	int size;
	int i = 0;

	size = v.size();
	recursive_sort (v, 0, size, print);	
	
	if (print) {
		printf("\t\t     ");
		debug_print(v);
	}

	return;
}


// recusive quicksort function
void recursive_sort (vector <double> &v, int start, int size, int print) {
	int lp, rp;			// left pointers and right pointers
	int swap;			// holds the swap index for printing
	double temp;		// used for swapping

	if (size < 1) {
		cout << "I fucked up.\n";
		exit(-1);
	}

	// size == 1 base case
	if (size == 1) {
		return;
	}

	if (print) {
		printf("S: %5d %5d       ", start, size);
		debug_print(v);
	}
	
	// size == 2 base case
	if (size == 2) {

		// manually sorts the two elements
		if (v[start] > v[start + 1]) {
			temp = v[start];
			v[start] = v[start + 1];
			v[start + 1] = temp;
		}

		return;
	}
	
	// find the pivot and places it at the front of the partition
	find_pivot(v, start, size);

	if (print) {
		printf("M: %5d %5d %5.2f ", start, size, v[start]);
		debug_print(v);
	}

	// initializes the left and right pointers
	lp = start+1;
	rp = start + size - 1;

	// organizes the vector to prepare for partitioning
	while (lp < rp) {

		// sets the left pointer
		while ((v[lp] < v[start]) and (lp != start + size)) {
			lp++;
		}

		// sets the right pointer
		while ((v[rp] > v[start]) and (rp != start)) {
			rp--;
		}

		// checks if we are done
		if (rp <= lp) break;
	
		// swaps the elements we are pointing at
		temp = v[lp];
		v[lp] = v[rp];
		v[rp] = temp;
		lp++;
		rp--;

		// checks again if we are done
		if (rp <= lp) break;
	}
	
	// swaps the pivot into it correct location based on a number of factors
	if (lp == rp) {
		if (v[start] >= v[lp]) {
			temp = v[lp];
			v[lp] = v[start];
			v[start] = temp;
			swap = lp;
		} else if (v[start] < v[lp]) {
			temp = v[lp - 1];
			v[lp - 1] = v[start];
			v[start] = temp;
			rp--;
			swap = lp - 1;
		}
	} else if (lp != rp) {
		temp = v[lp - 1];
		v[lp - 1] = v[start];
		v[start] = temp;
		swap = lp - 1;
	}

	if (print) {
		printf("P: %5d %5d %5d ", start, size, swap);
		debug_print(v);
	}
	
	// recursively calls quicksort on the left partition
	if ((rp - start > 0) and !(start < 0)) {
		recursive_sort(v, start, rp - start, print);
	}

	// recusrively calls quicksort on the right partition
	if ((size -  (rp - start) - 1 > 0) and !(rp + 1 > v.size())) {
		recursive_sort(v, start + (rp - start) + 1, size - (rp - start) - 1, print);
	}

	// the vector is sorted and we return
	return;
}

// finds the median of the first, middle, and last elements of a partition and moves it to the start
void find_pivot (vector <double> &v, int start, int size) {
	double beginning, middle, end;
	double temp;

	beginning = v[start];
	middle = v[start + (size / 2)];
	end = v[start + size - 1];

	// finds the median if two or more of the numbers equal each other
	if (middle == end) {
		temp = v[start];
		v[start] = v[start + size - 1];
		v[start + size - 1] = temp;
		return;
	} else if ((beginning == end) or (middle == end)) {
		return;
	}

	// finds the median of three differnet numbers
	// I got this off a post on Stack Exchange
	// this is way to elegent for me to come up with
	// and it works too, which is really cool
	if ((beginning - middle) * (middle - end) > 0) {
		temp = v[start + (size/2)];
		v[start + (size/2)] = v[start];
		v[start] = temp;
		return;
	} 
	if ((beginning - middle) * (beginning - end) > 0) {
		temp = v[start + size - 1];
		v[start + size - 1] = v[start];
		v[start] = temp;
		return;
	}

	// just in case
	return;
}

// prints the contents on a vector
void debug_print(vector <double> &v) {
	for (int i = 0; i < v.size(); i++) {
		printf(" %.2f", v[i]);
	}
	cout << endl;
}
