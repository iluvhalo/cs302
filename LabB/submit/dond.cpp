// Matt Matto
// Lab B
// dond.cpp
// takes in command line arguments and returns the probability of winning based on the sides of the dice, the number of rolls eft, and the last roll rolled
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>

using namespace std;

// Dond class that holds my cache and function
class Dond {
	public:
		int s;		// sides on die
		map <string, double> cache;
		double findProbability(double t, double last);
};

// takes in the command line arguments and calls the findProbability function and prints the result
int main(int argc, char *argv[]) {
	int t, last_roll;			// used when taking in command line arguments
	double retval;				// used to hold the return value
	Dond d;						// Dond object

	if (argc != 4) {
		printf("usage\n");
		return -1;
	}

	// inputs command line arguments
	d.s = atoi(argv[1]);
	t = atoi(argv[2]);
	last_roll = atoi(argv[3]);

	// finds the Probability
	retval = d.findProbability(t, last_roll);

	// prints the result
	printf("%g\n", retval);

	return 0;
}

// recursive functions the calculates the probability of winning dond
double Dond::findProbability(double t, double last) {
	int i;										// for loop iterator
	map <string, double>::iterator mit;	// cache iterator
	double sum = 0.0;							// sum for returning the return value
	char buf[50];								// used to build my cache key
	
	// builds my cache key by splicing t and last together into a string
	sprintf(buf, "%f %f", t, last);		

	// searches the cache for if we have already calculated the probability
	mit = cache.find(buf);
	if (mit != cache.end()) {
		return mit->second;
	}

	// base case
	if (t == 1.0) {
		if (last == -1) {
			return 1.0;
		} else if ((last == 0) or (last == s - 1)) {
			return (s - 2.0) / s;
		} else {
			return (s - 3.0) / s;
		}
	}

	// calls findProbability from 0 to s for all t - 1 that are valid
	for (i = 0; i < s; i++) {
		if (((last != i - 1) and (last != i) and (last != i + 1)) or (last == -1)) {
			sum += findProbability(t - 1, i);
		}
	}

	// inserts the result into the cache
	cache.insert(make_pair(buf, sum / s));
	
	// returns the probability
	return sum / s;
}
