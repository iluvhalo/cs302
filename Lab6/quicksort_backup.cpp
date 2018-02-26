// Matt Matto
// Lab6 -- quick_2_sort.cpp
// sorts a vector of numbers using quick sort

#include <vector>
#include <cstdio>
#include <iostream>
#include "sorting.h"

using namespace std;

void recursive_sort (vector <double> &v, int start, int size, int print);
void find_pivot (vector <double> &v, int start, int size);							// find the pivot and places it at the front of the partition to sort
void debug_print(vector <double> &v);														// just prints the contents of a vector


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

void recursive_sort (vector <double> &v, int start, int size, int print) {
	int lp, rp;
	int swap;
	double temp;

	if (size < 1) {
		cout << "I fucked up.\n";
		exit(-1);
	}

	if (size == 1) {
		return;
	}

	if (print) {
		printf("S: %5d %5d       ", start, size);
		debug_print(v);
	}

	if (size == 2) {
		if (v[start] > v[start + 1]) {
			temp = v[start];
			v[start] = v[start + 1];
			v[start + 1] = temp;
		}

		return;
	}
	
	find_pivot(v, start, size);

	if (print) {
		printf("M: %5d %5d %5.2f ", start, size, v[start]);
		debug_print(v);
	}

	lp = start+1;
	rp = start + size - 1;

	while (lp < rp) {
		while ((v[lp] < v[start]) and (lp != start + size)) {
			lp++;
		}
		while ((v[rp] > v[start]) and (rp != start)) {
			rp--;
		}

		if (rp <= lp) break;

		temp = v[lp];
		v[lp] = v[rp];
		v[rp] = temp;
		lp++;
		rp--;

		if (rp <= lp) break;
	}
	
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

	if ((rp - start > 0) and !(start < 0)) {
		recursive_sort(v, start, rp - start, print);
	}
	if ((size -  (rp - start) - 1 > 0) and !(rp + 1 > v.size())) {
		recursive_sort(v, start + (rp - start) + 1, size - (rp - start) - 1, print);
	}

	return;
}

void find_pivot (vector <double> &v, int start, int size) {
	double beginning, middle, end;
	double temp;

	beginning = v[start];
	middle = v[start + (size / 2)];
	end = v[start + size - 1];

	if (middle == end) {
		temp = v[start];
		v[start] = v[start + size - 1];
		v[start + size - 1] = temp;
		return;
	} else if ((beginning == end) or (middle == end)) {
		return;
	}

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

	return;
}

void debug_print(vector <double> &v) {
	for (int i = 0; i < v.size(); i++) {
		printf(" %.2f", v[i]);
	}
	cout << endl;
}
