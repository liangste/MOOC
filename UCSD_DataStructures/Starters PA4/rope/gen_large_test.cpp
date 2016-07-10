#include <iostream>
#include <string>
#include <cstdlib>

#define NUM_S 300000
#define NUM_Q 100000

using namespace std;

int main() {

	string test_string;

	for (int i = 0; i <= NUM_S; i++) {
		test_string += rand() % 26 + 'A';
	}

	cout << test_string << endl;
	cout << NUM_Q << endl;
	for (int x = 0; x <= NUM_Q; x++) {
		int i = rand() % NUM_S;
		int j = rand() % NUM_S;
		if (j > i)
			swap(j, i);
		int k = rand() % (NUM_S - j + i + 1);
		cout << i << " " << j << " " << k << endl;
	}
	return 0;
}
