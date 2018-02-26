#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <stack>

using namespace std;

class Answer {
	public:
		int length;
		int r;
		int c;
		string key;
		Answer *nexta;
};

class Spellseeker {
	public:
		vector < string > board;
		map <string, Answer *> cache;
		int total_r;
		int total_c;

		void DFS(vector <string> &board);
		Answer *Solve(int row, int col);
};

void makeBoard(vector < string > &board);

int main () {
	Spellseeker s;
	Answer *a, *b; 
	Answer *temp;
	int i, j;
	
	makeBoard(s.board);
	s.total_r = s.board.size();
	s.total_c = s.board[0].size();

	for (i = 0; i < s.board.size(); i++) {
		printf("%s\n", s.board[i].c_str());
	}
	
	a = new Answer;
	b = a;
	a->length = 0;

	for (i = 0; i < s.board.size(); i++) {
		for (j = 0; j < s.board[i].size(); j++) {
			temp = s.Solve(i, j);

			if (temp->length > a->length) a = temp;
		}
	}

	delete b;

	return 0;
}

void Spellseeker::DFS(vector <string> &key) {
	stack <pair <int, int> > s;
	vector <pair <int, int> > v;

	s.push(


	
	return;
}

Answer *Spellseeker::Solve(int row, int col) {
	

}

/*
	// look left
	if (col > 0) {
		if ((board[row][col - 1] == save - 1) or (board[row][col - 1] == save + 1)) {
			temp = Solve(row, col - 1);

			if (temp->length > a->length) a = temp;
		}
	}

	// look right
	if (col < total_c - 1) {
		if ((board[row][col + 1] == save - 1) or (board[row][col + 1] == save + 1)) {
			temp = Solve(row, col + 1);

			if (temp->length > a->length) a = temp;
		}
	}

	// look up
	if (row > 0) {
		if ((board[row - 1][col] == save - 1) or (board[row - 1][col] == save + 1)) {
			temp = Solve(row - 1, col);

			if (temp->length > a->length) a = temp;
		}
	}

	// look down
	if (row < board.size() - 1) {
		if ((board[row + 1][col] == save - 1) or (board[row + 1][col] == save + 1)) {
			temp = Solve(row + 1, col);

			if (temp->length > a->length) a = temp;
		}
	}

	// if col is even
	if (col % 2 == 0) {
		// look down-right
		if ((row < board.size() - 1) and (col < board[row].size() - 1)) {
			if ((board[row + 1][col + 1] == save - 1) or (board[row + 1][col + 1] == save + 1)) {
				temp = Solve(row + 1, col + 1);

				if (temp->length > a->length) a = temp;
			}
		}

		// look down-left
		if ((row < board.size() - 1) and (col > 0)) {
			if ((board[row + 1][col - 1] == save - 1) or (board[row + 1][col - 1] == save + 1)) {
				temp = Solve(row + 1, col - 1);

				if (temp->length > a->length) a = temp;
			}
		}
	} 

	// if col is odd
	if (col % 2 == 1) {
		// look up-right
		if ((row > 0) and (col < board[row].size() - 1)) {
			if ((board[row - 1][col + 1] == save - 1) or (board[row - 1][col + 1] == save + 1)) {
				temp = Solve(row - 1, col + 1);

				if (temp->length > a->length) a = temp;
			}
		}

		// look up-left
		if ((row > 0) and (col > 0)) {
			if ((board[row - 1][col - 1] == save - 1) or (board[row - 1][col - 1] == save + 1)) {
				temp = Solve(row - 1, col - 1);

				if (temp->length > a->length) a = temp;
			}
		}
	}
*/

void makeBoard(vector < string > &board) {
	int i, j;
	string input;

	i = 0;

	while (cin >> input) {
		board.resize(board.size() + 1);
		board[i].resize(input.length());

		for (j = 0; j < board[i].size(); j++) {
			board[i][j] = input[j];
		}

		i++;
	}

	return;
}
