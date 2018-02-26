// Matt Matto
// Lab 5 -- sb-analyze.cpp
// reads a Superball board and lists the scoring sets on the board

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <map>
#include "DJ.h"

using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
	public:
		Superball(int argc, char **argv);
		int r;
		int c;
		int mss;
		int empty;
		vector <int> board;
		vector <int> goals;
		vector <int> colors;
		Disjoint *DJSet;
		void analyze_superball();
		map <int, int> goalMap;					// a collection of sets from the Disjoint set that are in the goal (key: set parent location, value: goal location)
		map <int, int>::iterator mit;
		vector <int> sizes;
};

void usage(const char *s) {
	fprintf(stderr, "usage: sb-analyze rows cols min-score-size colors\n");
	if (s != NULL) fprintf(stderr, "%s\n", s);
	exit(1);
}

Superball::Superball(int argc, char **argv){
	int i, j;
	string s;

	if (argc != 5) usage(NULL);																			// checks for the correct number of arguments

	if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");							// checks for a valid row number
	if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");							// checks for a valid column number
	if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");		// checks for a valid min-score-size

	DJSet = new Disjoint(r*c);																				// declares a new Disjoint set		
	colors.resize(256, 0);																					// resizes the colors vector to 256 and initializes all the spots to 0

	for (i = 0; i < strlen(argv[4]); i++) {															// iterate through the colors argument
		if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");					// checks if the char is a member of the alphabet
		if (!islower(argv[4][i])) usage("Colors must be lowercase letters");					// checks if the char is a lowercase letter
		if (colors[argv[4][i]] != 0) usage("Duplicate color");									// checks for duplicates
		colors[argv[4][i]] = 2+i;																			// sets the char's point value
		colors[toupper(argv[4][i])] = 2+i;																// sets the uppercase's value, too
	}

	board.resize(r*c);																						// resizes the board to rows x columns
	goals.resize(r*c, 0);																					// also sets goals to the same size as the board
	sizes.resize(r*c, 1);																					// makes a copy of the ranks vector from the DJ set

	empty = 0;																									// initializes empty

	for (i = 0; i < r; i++) {																				// iterates through the rows of the board
		if (!(cin >> s)) {																					// inputs strings of each row
			fprintf(stderr, "Bad board: not enough rows on standard input\n");				// if the input is finished before the loop is
			exit(1);
		} if (s.size() != c) {																				// checks for the correct string length (columns)
			fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
			exit(1);
		}
		for (j = 0; j < c; j++) {																			// iterates through the columns
			if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {								// checks if the char in the string is either a '.', a '*', or a valid color
				fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);			// if not, error
				exit(1);
			}
			board[i*c+j] = s[j];																				// add the char to the board vector
			if (board[i*c+j] == '.') empty++;															// if the char is a '.'
			if (board[i*c+j] == '*') empty++;															// or a '*', it increments empty
			if (isupper(board[i*c+j]) || board[i*c+j] == '*') {									// checks if the char is a goal position
				goals[i*c+j] = 1;																				// adds that position to the goal board
				board[i*c+j] = tolower(board[i*c+j]);													// changes the uppercase char to a lowercase
			} // end of goal position check
		} // end of iterating through the columns
	} // end of iterating through the rows
} // end of the constructor

main(int argc, char **argv) {
	Superball *s;
	int i, j;
	int ngoal, tgoal;

	s = new Superball(argc, argv);

	tgoal = 0;																		// initializes the sum of their values
	ngoal = 0;																		// initializes the number of pieces in the goal cells to 0
	for (i = 0; i < s->r*s->c; i++) {										// iterates through board
		if (s->goals[i] && s->board[i] != '*') {							// checks if the char is a goal spot and it is nore empty
			tgoal += s->colors[s->board[i]];									// adds the point value to the sum
			ngoal++;																	// incremtents the pieces in goal counter
		}
	}

	s->analyze_superball();

	exit(0);
}

void Superball::analyze_superball() {
	int i, j, current, connect, parent, child1, child2;
//	vector <int> repeats;
//	bool condition;

	for (i = 0; i < r; i++) {																		// this loops through the whole board connecting everything
		for (j = 0; j < c; j++) {																	// that can be connected into disjoint sets
			if (isalpha(board[i * c + j])) {														// checks if the current element is a char in the alphabet, ie not a '.' or '*'
				current = (i * c) + j;																// assigns that location to current
				if (i * c + j + 1 != (i + 1) * c) {												// checks that the location to the right is not actually the first element in the next row
					if (board[i * c + j] == board[i * c + j + 1]) {							// checks if the char at current and to the right of current are the same
						connect = current + 1;														// sets the element to connect as being the next one
						child1 = DJSet->Find(connect);
						child2 = DJSet->Find(current);
						parent = DJSet->Union(DJSet->Find(current), DJSet->Find(connect));		// Unions the current and connect elementsi
						if (parent == child2) {
//							child = DJSet->Find(connect);
							sizes[parent] += sizes[child1];
						} else {
//							child = current;
							sizes[parent] += sizes[child2];
						}
					}																						// end of Union right
				}																							// end of check element location to the right
				if ((i + 1) * c + j < r * c) {													// checks to make sure the element one row down is not out of the bounds of the vector
					if (board[i * c + j] == board[((i + 1) * c) + j]) {					// checks if the char at the current element and below the current are the same
						connect = (i + 1) * c + j;													// sets the element to connect as being the one right below the current element
						child1 = DJSet->Find(connect);
						child2 = DJSet->Find(current);
						parent = DJSet->Union( DJSet->Find(current), DJSet->Find(connect));		// Unions the current and connect elements
						if (parent == child2) {
//							child = DJSet->Find(connect);
							sizes[parent] += sizes[child1];
						} else {
//							child = current;
							sizes[parent] += sizes[child2];
						}
					}																						// end of Union below
				}																							// end of check valid element location below
			}																								// end of alphabet check
		}																									// end of column iteration
	}																										// end of for loop that makes everything connected into sets:wq


/*
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			//			cout << "before the if\t";
			if (goals[i*c+j] == 1 and board[i*c+j] != '*' and sizes[DJSet->Find(i*c+j)] >= 5){	// if [i,j] is a goal position, and it is not empty, and it's set is scorable
//				printf("\ni*c+j = %d\nboard[i*c+j] = %c\n", i*c+j, board[i*c+j]);
				mit = goalMap.find(DJSet->Find(i*c+j));
				if (mit == goalMap.end()) {
					goalMap.insert(make_pair(DJSet->Find(i*j+c), i*c+j));										// add that set to the setMap
//					cout << DJSet->Find(i*c+j) << "\n";
				}
			}
			//			cout << "\tafter the if     i*c+j = " << i*c+j << endl;
		}
	}
*/
	for (i = 0; i < r*c; i++) {
		if (goals[i] == 1 and board[i] != '*' and sizes[DJSet->Find(i)] >= mss){	// if [i,j] is a goal position, and it is not empty, and it's set is scorable
			goalMap.insert(make_pair(DJSet->Find(i), i));										// add that set to the setMap
		}
	}
	
	if (goalMap.size() > 0) printf("Scoring sets:\n");

	for (mit = goalMap.begin(); mit != goalMap.end(); mit++) {
//		condition = true;
//		current = mit->first;
//		for (i = 0; i < repeats.size(); i++) if (repeats[i] == current) condition = false;
//		if (condition) {
			printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n", sizes[DJSet->Find(mit->second)], board[mit->second], (mit->second) / c, (mit->second) % c);
//			repeats.push_back(current);
//		}
	}

//	DJSet->Print();
}
