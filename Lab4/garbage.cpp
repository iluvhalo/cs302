#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

int main () {
	int integer = 0;
	string thing, binary;
	istringstream buffer;

	thing = "XEXEXEX.E";
	binary.resize(thing.size());
	cout << "binary before the loop is: " << binary << endl;
	for (int i = 0; i < binary.size(); i++) binary[i] = '0';
	for (int i = 0; i < thing.size(); i++) {
		if ((thing[i] == 'X') or (thing[i] == 'E')) {
			binary[i] = '1';
		}

		cout << binary << endl;

	}

	for (int i = 0; i < binary.size(); i++) {
		integer += (binary[i] - '0') * pow(2, binary.size() - i - 1);
	}

	printf("%x\n", integer);
}
