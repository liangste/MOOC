#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define DEBUG 0

using namespace std;

string InverseBWT(const string& bwt) {
	string text;
	text.resize(bwt.size());

	// NOTE see http://web.stanford.edu/class/cs262/presentations/lecture4.pdf
	unsigned LF[bwt.size()];
	map<char, unsigned> C;

	unsigned a_cnt, c_cnt, g_cnt, t_cnt;

	// construct C
	a_cnt = c_cnt = g_cnt = t_cnt = 0;
	for (unsigned u = 0; u < bwt.size(); u++) {
		switch(bwt[u]) {
			case 'A':
				a_cnt++;
				break;
			case 'C':
				c_cnt++;
				break;
			case 'G':
				g_cnt++;
				break;
			case 'T':
				t_cnt++;
				break;
			default:
				break;
		}
	}

#if DEBUG
	cout << "bwt: " << bwt << endl;
	cout << "A: " << a_cnt << endl;
	cout << "C: " << c_cnt << endl;
	cout << "G: " << g_cnt << endl;
	cout << "T: " << t_cnt << endl;
#endif

	C['A'] = 1;
	C['C'] = a_cnt + C['A']; // all A's + $
	C['G'] = c_cnt + C['C']; // all C's + A's + $
	C['T'] = g_cnt + C['G'];

#if DEBUG
	cout << "C[A]: " << C['A'] << endl;
	cout << "C[C]: " << C['C'] << endl;
	cout << "C[G]: " << C['G'] << endl;
	cout << "C[T]: " << C['T'] << endl;
#endif

	// construct LF array
	a_cnt = c_cnt = g_cnt = t_cnt = 0;
	for (unsigned u = 0; u < bwt.size(); u++) {
		switch(bwt[u]) {
			case 'A':
				a_cnt++;
				LF[u] = a_cnt + C['A'];
				break;
			case 'C':
				c_cnt++;
				LF[u] = c_cnt + C['C'];
				break;
			case 'G':
				g_cnt++;
				LF[u] = g_cnt + C['G'];
				break;
			case 'T':
				t_cnt++;
				LF[u] = t_cnt + C['T'];
				break;
			case '$':
				LF[u] = 1;
				break;
			default:
				break;
		}
	}

#if DEBUG
	for (unsigned u = 0; u < bwt.size(); u++) {
		cout << LF[u] << " ";
	} cout << endl;
#endif

	unsigned idx = 0;
	text[0] = '$';
	for (unsigned u = 0; u < (bwt.size() - 1); u++) {
		text[u + 1] = bwt[idx];
		idx = LF[idx] - 1;
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
