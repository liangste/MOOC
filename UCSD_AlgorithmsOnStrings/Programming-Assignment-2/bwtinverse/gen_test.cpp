#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

#define SIZE 1000000

char nucleotides[] = {'A', 'C', 'T', 'G'};

int main(void) {
	string test;
	test.resize(SIZE);

	for (int i = 0; i < SIZE; i++) {
		test[i] = nucleotides[rand() % 4];
	}

	test[rand() % SIZE] = '$';

	cout << test << endl;
}
