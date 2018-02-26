// Matt Matto
// Lab B
// spellpath.cpp
// creates a board of chars and recursively finds the longest path on the board
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int maxPathLength(int row, int col, vector < string > &board);

int main () {
	vector < string > board;		// game board
	int temp, maxLength;				// used to calculate the make path length
	int i, j;							// for loop iterators
	string input;

	// fills in the board
	while (cin >> input) {
		board.push_back(input);
	}

	// iterates through the whole board, trying to find the longest path starting from each position
	for (i = 0; i < board.size(); i++) {
		for (j = 0; j < board[i].size(); j++) {
			temp = maxPathLength(i, j, board);
	
			if (temp > maxLength) maxLength = temp;
		}
	}

	// prints the max length and returns
	printf("%d\n", maxLength);
		
	return maxLength;
}

// recursively fins the max lenth of a path on the board
int maxPathLength(int row, int col, vector < string > &board) {
	char save;				// saves the char at position [row,col]
	int temp, retval;		// used to return the max path length

	// saves the char at position [row,col] and removes it from the board
	save = board[row][col];
	board[row][col] = '-';

	retval = 0;

	// look left
	if (col > 0) {
		if ((board[row][col - 1] == save - 1) or (board[row][col - 1] == save + 1)) {
			temp = maxPathLength(row, col - 1, board);

			if (temp > retval) retval = temp;
		}
	}

	// look right
	if (col < board[row].size() - 1) {
		if ((board[row][col + 1] == save - 1) or (board[row][col + 1] == save + 1)) {
			temp = maxPathLength(row, col + 1, board);

			if (temp > retval) retval = temp;
		}
	}

	// look up
	if (row > 0) {
		if ((board[row - 1][col] == save - 1) or (board[row - 1][col] == save + 1)) {
			temp = maxPathLength(row - 1, col, board);

			if (temp > retval) retval = temp;
		}
	}

	// look down
	if (row < board.size() - 1) {
		if ((board[row + 1][col] == save - 1) or (board[row + 1][col] == save + 1)) {
			temp = maxPathLength(row + 1, col, board);

			if (temp > retval) retval = temp;
		}
	}

	// if col is even
	if (col % 2 == 0) {
		// look down-right
		if ((row < board.size() - 1) and (col < board[row].size() - 1)) {
			if ((board[row + 1][col + 1] == save - 1) or (board[row + 1][col + 1] == save + 1)) {
				temp = maxPathLength(row + 1, col + 1, board);

				if (temp > retval) retval = temp;
			}
		}

		// look down-left
		if ((row < board.size() - 1) and (col > 0)) {
			if ((board[row + 1][col - 1] == save - 1) or (board[row + 1][col - 1] == save + 1)) {
				temp = maxPathLength(row + 1, col - 1, board);

				if (temp > retval) retval = temp;
			}
		}
	} 

	// if col is odd
	if (col % 2 == 1) {
		// look up-right
		if ((row > 0) and (col < board[row].size() - 1)) {
			if ((board[row - 1][col + 1] == save - 1) or (board[row - 1][col + 1] == save + 1)) {
				temp = maxPathLength(row - 1, col + 1, board);

				if (temp > retval) retval = temp;
			}
		}

		// look up-left
		if ((row > 0) and (col > 0)) {
			if ((board[row - 1][col - 1] == save - 1) or (board[row - 1][col - 1] == save + 1)) {
				temp = maxPathLength(row - 1, col - 1, board);

				if (temp > retval) retval = temp;
			}
		}
	}

	// put the saved char back into the board
	board[row][col] = save;
	
	return retval + 1;
}
