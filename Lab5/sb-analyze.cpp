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

	if (argc != 5) usage(NULL);	

	if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");							
	if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");							
	if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");		

	DJSet = new Disjoint(r*c);																						
	colors.resize(256, 0);																					

	for (i = 0; i < strlen(argv[4]); i++) {															
		if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");					
		if (!islower(argv[4][i])) usage("Colors must be lowercase letters");					
		if (colors[argv[4][i]] != 0) usage("Duplicate color");									
		colors[argv[4][i]] = 2+i;																			
		colors[toupper(argv[4][i])] = 2+i;																
	}

	board.resize(r*c);																						
	goals.resize(r*c, 0);																					
	sizes.resize(r*c, 0);																					

	empty = 0;																									

	for (i = 0; i < r; i++) {																				
		if (!(cin >> s)) {																					
			fprintf(stderr, "Bad board: not enough rows on standard input\n");				
			exit(1);
		} if (s.size() != c) {																				
			fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
			exit(1);
		}
		for (j = 0; j < c; j++) {																			
			if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {								
				fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);			
				exit(1);
			}
			board[i*c+j] = s[j];																				
			if (board[i*c+j] == '.') empty++;															
			if (board[i*c+j] == '*') empty++;															
			if (isupper(board[i*c+j]) || board[i*c+j] == '*') {									
				goals[i*c+j] = 1;																				
				board[i*c+j] = tolower(board[i*c+j]);													
			} // end of goal position check
		} // end of iterating through the columns
	} // end of iterating through the rows
} // end of the constructor

main(int argc, char **argv) {
	Superball *s;
	int i, j;
	int ngoal, tgoal;

	s = new Superball(argc, argv);

	tgoal = 0;																		
	ngoal = 0;																		
	for (i = 0; i < s->r*s->c; i++) {										
		if (s->goals[i] && s->board[i] != '*') {							
			tgoal += s->colors[s->board[i]];									
			ngoal++;																	
		}
	}

	s->analyze_superball();

	exit(0);
}

void Superball::analyze_superball() {
	int i, j;
	bool first = true;
	vector <int> repeat;
	
	// iterates through the superball board and unions elements into Disjoint sets
	for (i = 0; i < r * c; i++) {
		if (board[i] > 'a') {																								
			//Union Right
			if (i + 1 != ((i / c) + 1) * c) {																			
				if (board[i] == board[i + 1]) {																			
					if (!(DJSet->Find(i) == DJSet->Find(i + 1))) {
						DJSet->Union(DJSet->Find(i), DJSet->Find(i + 1));
					}
				}
			}
			//Union Down
			if (i + c < r * c) {																											
				if (board[i] == board[i + c]) {																			
					if (!(DJSet->Find(i) == DJSet->Find(i + c))) {
						DJSet->Union(DJSet->Find(i), DJSet->Find(i + c));
					}
				}
			}
		}
	}
	
	// iterates through the superball board and adds up the size of the sets
	for (i = 0; i < r * c; i++) {
		sizes[DJSet->Find(i)]++;
	}
		
	repeat.resize(r*c, 0);
	
	// iterates through the board and when it comes across an element that is a goal position,
	// not empty, is part of a set that is >= the minimum scoring size, and is not already printed
	// prints the set it belongs to and marks it as printed in repeat[]
	for (i = 0; i < sizes.size(); i++) {
		if (goals[i] == 1 and board[i] != '*') {
			if (sizes[DJSet->Find(i)] >= mss and repeat[DJSet->Find(i)] == 0) {
				if (first) {
					printf("Scoring sets:\n");
					first = false;
				}
				printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n", sizes[DJSet->Find(i)], board[i], i / c, i % c);
				repeat[DJSet->Find(i)]++;
			}
		}
	}
}
