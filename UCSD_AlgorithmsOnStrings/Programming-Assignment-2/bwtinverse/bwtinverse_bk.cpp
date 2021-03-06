#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <map>

// NOTE following algo runs in ~7s for 1,000,000 data

#define DEBUG 0

using namespace std;

enum IBWTState {
	First = 0,
	Last
};

// TODO use hash table to speed up First-Last lookup

string InverseBWT(const string& bwt) {
	string firstColumn(bwt);
	sort(firstColumn.begin(), firstColumn.end());
	string lastColumn(bwt);
	string text = bwt;

#if DEBUG
	cout << "first column: " << firstColumn << endl;
	cout << "last column: " << lastColumn << endl;
#endif

	// TODO build string->row map of first column
	unsigned cnt_a, cnt_c, cnt_g, cnt_t;
	map<string, unsigned> lastFirstMap;
	string str;
	cnt_a = cnt_c = cnt_g = cnt_t = 0;
	for (unsigned u = 1; u < firstColumn.size(); u++) {
		str = "";
		switch(firstColumn[u]) {
			case 'A':
				str = string("A") + std::to_string(cnt_a++);
				break;
			case 'C':
				str = string("C") + std::to_string(cnt_c++);
				break;
			case 'G':
				str = string("G") + std::to_string(cnt_g++);
				break;
			case 'T':
				str = string("T") + std::to_string(cnt_t++);
				break;
			default:
				str = "$";
				break;
		}
		if (str.size() > 0)
			lastFirstMap[str] = u;
	}
	// TODO build string keys of last column
	vector<string> lastKeys;
	lastKeys.resize(firstColumn.size());
	cnt_a = cnt_c = cnt_g = cnt_t = 0;
	for (unsigned u = 0; u < firstColumn.size(); u++) {
		switch(lastColumn[u]) {
			case 'A':
				str = string("A") + std::to_string(cnt_a++);
				break;
			case 'C':
				str = string("C") + std::to_string(cnt_c++);
				break;
			case 'G':
				str = string("G") + std::to_string(cnt_g++);
				break;
			case 'T':
				str = string("T") + std::to_string(cnt_t++);
				break;
			default:
				str = "$";
				break;
		}
		lastKeys[u] = str;
	}

#if DEBUG
	for (auto& m : lastFirstMap) {
		cout << m.first << " -> " << m.second << endl;
	}

	for (auto& k : lastKeys) {
		cout << k << endl;
	}
#endif

	// Inverse-BWT using First-Last Property
	unsigned row = 0;
	IBWTState state = Last;
	string key;

	text[0] = '$';
	unsigned strIndex = 1;
	while (strIndex < bwt.size()) {
		switch(state) {
			case First:
				row = lastFirstMap[lastKeys[row]];
				state = Last;
				break;
			case Last:
			default:
				text[strIndex++] = lastColumn[row];
				state = First;
				break;
		}
	}

	reverse(text.begin(), text.end());

	return text;
}

int main() {
	string bwt;
	cin >> bwt;
	cout << InverseBWT(bwt) << endl;
	return 0;
}
