// Matt Matto
// Lab 6 -- merge_1_sort.cpp
// sorts a vector of numbers using merge sort

#include <vector>
#include <cstdio>
#include <iostream>
#include "sorting.h"

using namespace std;

void debug_print (vector <double> &v);
void swap_vector_comps (vector <double> &v, int index1, int index2);

void recursive_sort (vector <double> &v, vector <double> &temp, int start, int size, int print){
	int i, j, k, lsize, rsize;
	vector <double> v1, v2;

	// size == 1 base case
	if (size == 1) {
		return;
	}

	if (print) {
		printf("B: %5d %5d  ", start, size);
		debug_print (temp);
	}

	// size == 2 base case
	if (size == 2) {
		
		// manually sorts
		if (temp[start] > temp[start + 1]) {
			swap_vector_comps (temp, start, start + 1);
		}
	
		if (print) {
			printf("E: %5d %5d  ", start, size);
			debug_print (temp);
		}

		// copies temp over to v
		v[0] = temp[start];
		v[1] = temp[start + 1];
		
		return;

	// else we need to split it up and call recursive_sort on each half
	} else if (size > 2) {

		// makes left vector
		lsize = v.size() / 2;
		for (i = 0; i < lsize; i++) {
			v1.push_back(v[i]);
		}

		// make right vector
		rsize = v.size() - lsize;
		for (i = v.size() / 2; i < v.size(); i++) {
			v2.push_back(v[i]);
		}
		
		// calls function to sort each half
		recursive_sort(v1, temp, start, lsize, print);
		recursive_sort(v2, temp, start + lsize, rsize, print);

		// iterators for each of my left and right vectors
		j = 0;
		k = 0;

		// goes through the temp vector and merges v1 and v2 into it
		for (i = start; i < start + size; i++) {
			if ((j < v1.size()) and (k < v2.size())) {
				if (v1[j] < v2[k]) {
					temp[i] = v1[j];
					j++;
				} else {
					temp[i] = v2[k];
					k++;
				}
			} else if ((j < v1.size()) and !(k < v2.size())) {
				temp[i] = v1[j];
				j++;
			} else if (!(j < v1.size()) and (k < v2.size())) {
				temp[i] = v2[k];
				k++;
			}
		}

		if (print) {
			printf("E: %5d %5d  ", start, size);
			debug_print(temp);
		}

		// copies temp over to v
		for (i = 0; i < v.size(); i++) {
			v[i] = temp[i + start];
		}

		return;
	}
}

// starter function
void sort_doubles (vector <double> &v, int print) {
	int i;
	vector <double> temp;

	// sets temp to equal v
	temp = v;
	
	// sorts v
	recursive_sort (v, temp, 0, v.size(), print);
	
	// copies temp into v
	v = temp;

	// prints the sorted vector
	if (print) {
		printf("\t\t");
		debug_print(v);
	}

	return;
}

// prints the contents of a passed in vector
void debug_print (vector <double> &v) {
	int i = 0;

	for ( ;i < v.size(); i++) {
		printf(" %.2f", v[i]);
	}
	cout << endl;

	return;
}

// swaps around the contents of two vector indexes
void swap_vector_comps (vector <double> &v, int index1, int index2) {
	double hold;

	hold = v[index1];
	v[index1] = v[index2];
	v[index2] = hold;

	return;
}
