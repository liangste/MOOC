#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string BWT(const string& text) {
	string result = "";
	vector<string> rotations;

	// write your code here
	for (unsigned u = 0; u < text.size(); u++) {
		string chopped = text.substr(0, u);
		string remaining = text.substr(u);
		rotations.push_back(remaining + chopped);
	}
	sort(rotations.begin(), rotations.end());

	for (string& r : rotations) {
		result += r[r.size() - 1];
	}

	return result;
}

int main() {
	string text;
	cin >> text;
	cout << BWT(text) << endl;
	return 0;
}
