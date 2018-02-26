// Matt Matto
// Lab 4
// Recursively enumerates permutations and "n choose r" enumerations, then prints them out in either a matrix or in hexadecimal

#include <cstdio>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

// Plank's Matrix class, edited a little bit
class Matrix {
	public:
		int W;
		int E;
		char P;								/* This is 'x' or 'h' */
		vector <int> Perm;					/* Permutation of 0 ... (W-1), for the 'X' elements. */
		vector <int> Non_X;					/* This is the row/col id of each of the non-X elements. */
		vector <int> E_ID;					/* This is the row/col id of the E elements */
		void Print();						/* Print the matrix defined by W, Perm and E_ID */
		void Permute (int index);			/* This is the recursive permuting method*/
		void Choose (int indexi, int Es);	/* This is the recursive n-choose-k method*/
};



int main (int argc, char *argv[]) {							
	// variables
	Matrix matrix;					// Matrix object
	istringstream buffer;			// used for the command line arguments
	int i;							// used for for loops and 

	if (argc != 4) {				// checks if there are the correct amount of arguments
		cout << "err\n";
		return -1;
	}
						
	buffer.str(argv[1]);			// a hideous way to put the arguments into variables
	buffer >> matrix.W;
	buffer.clear();
	buffer.str(argv[2]);
	buffer >> matrix.E;
	buffer.clear();
	buffer.str(argv[3]);
	buffer >> matrix.P;
	buffer.clear();

	if ((matrix.P != 'x') and (matrix.P != 'h')) {			// checks if P is a valid value
		cout << "err\n";
		return -1;
	}

	matrix.Perm.resize(matrix.W);							// resizes Perm to W

	for (i = 0; i < matrix.Perm.size(); i++) {				// fills Perm with { 0, 1, 2, ... , W-1 }
		matrix.Perm[i] = i;
	}

	matrix.Permute(0);										// starts the recersive enumeration

	return 0;
} // end of main()

void Matrix::Print() {									// Prints the matrix formed by the recursive functions
	vector <string> printmatrix;						// a vector of strings to build my matrix
	int i, j, k;										// for loop iterators
	int integer;										// holds the calculated integer from a binary string
	string binary;										// said binary string

	printmatrix.resize(W);								// resize the printmatrix to W

	for (i = 0; i < printmatrix.size(); i++) {			// fills the printmatrix with periods
		printmatrix[i].resize(W, '.');
	}

	for (i = 0; i < printmatrix.size(); i++) {			// goes through the printmatrix and places the X's
		j = Perm[i];
		printmatrix[i][j] = 'X';
	}

	for (i = 0; i < E_ID.size(); i++) {					// goes through E_ID
		j = E_ID[i] / W;								// calculates the row
		k = E_ID[i] % W;								// calculates the column
		printmatrix[j][k] = 'E';						// insert an 'E' at the row/col
	}
	if (P == 'x') {										// if P is an 'x'
		for (i = 0; i < printmatrix.size(); i++) {		// just prints the matrix normally
			printf("%s\n", printmatrix[i].c_str());
		}
	} else if (P == 'h') {															// if it is an 'h' does some extra calculations
		binary.resize(W);															// sets the size of the binary string
		for (i = 0; i < printmatrix.size(); i++) {									// iterates through the printmatrix
			integer = 0;															// initializes the integer to 0
			for (j = 0; j < binary.size(); j++) binary[j] = '0';					// initilizes the binary string to "000 ... 000"
			for (j = 0; j < printmatrix[i].size(); j++) {							// goes through the rows of the printmatrix
				if ((printmatrix[i][j] == 'X') or (printmatrix[i][j] == 'E')) {		// whenever it encounters an X or an E
					binary[j] = '1';												// sets that place in the binary string to 1
				}
			}

			for (j = 0; j < binary.size(); j++) {									// goes through the binary string and converts it to an integer
				integer += (binary[j] - '0') * pow(2, binary.size() - j - 1);		// converts binary[j] from a char to an int, sorta
			}																		// then it multiplies it by 2^t, where t is the place from the right of the number

			printf("%x\n", integer);												// prints the integer calculated in hexadecimal
		}
	}
} // end of Print()

void Matrix::Permute (int index) {					// recursively permutes the X locations in the W * W matrix
	int i, j;
	int tmp;
	bool change;

	if (index == Perm.size()) {						// this is the (disgusting) base case
		for (i = 0; i < pow(W, 2); i++) {			// iterates through W^2
			change = false;		
			for (j = 0; j < Perm.size(); j++) {		// iterates through Perm
				if (j * W + Perm[j] == i) {			// if the converted value of Perm is equal to i
					change = false;					// change is false so is don't cahnge something later (I know, its confusing)
					break;							// Plank hates these
				} else {							// else the i value is not a value in Perm and can be added to the Non_X vector
					change = true;
				}
			}
			if (change) {							// adds the i value to Non_X
				Non_X.push_back(i);
			}
		}
		Choose(0, E);								// starts Choose
		Non_X.clear();								// resets Non_X for the next form of the X positions
		return;
	}

	for (i = index; i < Perm.size(); i++) {			// this is exactly like the notes on recursive enumeration
		tmp = Perm[i];								// swaps the index and i
		Perm[i] = Perm[index];
		Perm[index] = tmp;

		Permute(index + 1);							// advances to the next index

		tmp = Perm[i];								// swaps them back
		Perm[i] = Perm[index];
		Perm[index] = tmp;
	}
} // end of Permute()

void Matrix::Choose (int index, int Es) {			// recursively enumerates "n choose r"
	int i, j, k, value;

	if (Es == 0) {									// base case of when E's left to place is 0
		Print();									// calls print
		cout << endl;
		return;
	}

	if (Es > Non_X.size() - index) return;			// base case of when there is not enough room to place the E's

	E_ID.push_back(Non_X[index]);					// pushes the value in Non_X into E_ID
	Choose(index + 1, Es - 1);						// calls Choose for pick another E location
	E_ID.pop_back();								// removes the E i just placed two line ago
	Choose(index + 1, Es);							// calls Choose for not placing an E

	return;
} // end of Choose()
