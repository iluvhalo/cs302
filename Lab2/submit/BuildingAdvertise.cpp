// Matt Matto
// Lab 2
// hits the most gloriously iritating infinite loop that after 4 hours of staring at, I still don't understand why

#include <cstdio>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

// class for the getMaxArea Fx
class BuildingAdvertise {
	public:
		long getMaxArea(vector <int> h, int n);
};

// right now, it does nothing but do an infinite loop
long BuildingAdvertise::getMaxArea(vector <int> h, int n) {
	// variable declarations
	vector <int> R;									// a vector of building heights
	int i, j, s;									// iterators
	int m, min, insert;								// ints used for calculations
	set <int> X;									// the set of building heights I put the building into in ascending order
	set <int>::iterator Xit;						// X's iterator

	R.resize(n + 2, 0);								// resizes R, to 2 bigger than h
	j = 0;
	m = h.size();

	for (i = 1; i < R.size() - 1; i++) {			// fills R and does other things I don't totally understand
		R[i] = h[j];								// this is basically copied for topcoder
		s = (j + 1) % m;
		h[j] = ((h[j] ^ h[s]) + 13) % 835454957;
		j = s;
	}
	
	min = 0;										// initializes min

	for (i = 0; i < R.size(); i++) {				// this for loop sets min to the index of the max height in R
		if (R[min] < R[i]) min = i;
	}

	insert = 0;
	X.insert(insert);								// insert the first index in R
	insert = R.size() - 1;
	X.insert(insert);								// insert the last index in R

	
	while (X.size() < R.size()) {					// my hideous infinite loop that I have no idea why it goes infinite
		for (i = 0; i < R.size(); i++) {					// iterates through R
			Xit = X.find(i);								// searches the set for the index i
			if ((Xit == X.end()) /*and (R[i] < R[min])*/) {	// if i is not in the set already
				if (R[i] < R[min]) {						// if the height at index i is less than the value at index min, then that means
															// R[i] is a shorter building and is not in the set yet
					min = i;								// sets min to i
					cout << min << endl;
				}
			}
		}
		X.insert(min);								// inserts the smallest building, not yet in the set, into the set
		for (i = 0; i < R.size(); i++) {			// resets min back to the index of the tallest building
			if (R[min] < R[i]) min = i;
		}
	}

	return -1;
}
