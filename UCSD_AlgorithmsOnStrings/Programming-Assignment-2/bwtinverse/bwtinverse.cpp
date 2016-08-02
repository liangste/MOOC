#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum IBWTState {
	First = 0,
	Last
};

// TODO use hash table to speed up First-Last lookup

// get row id of index's occurance of symbol in bwt
unsigned getFirstRow(const string& col, char symbol, unsigned index) {
	unsigned cur_index = 0;
	for (unsigned u = 0; u < col.size(); u++) {
		if (col[u] == symbol) {
			if (index == cur_index) {
				return u;
			} else {
				cur_index++;
			}
		}
	}
	return 0;
}

// return i-th occurance of symbol on row in bwt
unsigned getLastIndex(const string& col, char symbol, unsigned row) {
	assert(col[row] == symbol);
	unsigned ret = 0;
	for (unsigned u = 0; u < row; u++) {
		if (col[u] == symbol) ret++;
	}
	return ret;
}

string InverseBWT(const string& bwt) {
	string firstColumn(bwt);
	sort(firstColumn.begin(), firstColumn.end());
	string lastColumn(bwt);
	string text = bwt;

	// Inverse-BWT using First-Last Property
	char symbol; // current symbol
	unsigned index; // ith count of symbol
	unsigned row = 0;
	IBWTState state = Last;

	text[0] = '$';
	unsigned strIndex = 1;
	while (strIndex < bwt.size()) {
		switch(state) {
			case First:
				row = getFirstRow(firstColumn, symbol, index);
				state = Last;
				break;
			case Last:
			default:
				symbol = bwt[row];
				text[strIndex++] = symbol;
				index = getLastIndex(lastColumn, symbol, row);
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
