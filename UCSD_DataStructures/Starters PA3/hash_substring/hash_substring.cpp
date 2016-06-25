#include <iostream>
#include <string>
#include <vector>

#define DEBUG 0

using namespace std;

typedef unsigned long long ull;

static const size_t multiplier = 1;
static const size_t prime = 1000000007;

bool CheckSameStr(const string& s1, int begin1, int end1, const string& s2, int begin2, int end2) {
	int ii = begin2;
	for (int i = begin1; i <= end1; i++, ii++) {
		if (s1[i] != s2[ii])
			return false;
	}
	return true;
}

ull PolyHash(const string& s, int begin, int end) {

		unsigned long long hash = 0;
		for (int i = end; i >= begin; --i)
				hash = (hash * (ull)multiplier + (ull)s[i]) % prime;
		return hash;
}

struct Data {
		string pattern, text;
};

Data read_input() {
		Data data;
		std::cin >> data.pattern >> data.text;
		return data;
}

void print_occurrences(const std::vector<ull>& output) {
		for (size_t i = 0; i < output.size(); ++i)
				std::cout << output[i] << " ";
		std::cout << "\n";
}

std::vector<ull> get_occurrences(const Data& input) {
		const string& p = input.pattern, t = input.text;
		std::vector<ull> ans;

		if (p.size() == t.size()) {
			if (CheckSameStr(t, 0, t.size() - 1, p, 0, p.size() -1)) {
				ans.push_back(0);
				return ans;
			}
		}

		if (p.size() > t.size()) {
			return ans;
		}

		if (p.size() == 0) {
			return ans;
		}

		std::vector<size_t> preCompHashes;
		int begin = t.size() - p.size();
		int end = t.size() - 1;
		preCompHashes.resize(t.size() - p.size() + 1);
		preCompHashes[begin] = PolyHash(t, begin, end);
		ull y = 1;
		for (int i = 0; i < p.size(); i++) {
			y = (y * multiplier) % prime;
		}

#if DEBUG
		for (int i = begin - 1; i >= 0; i--) {
			preCompHashes[i] = PolyHash(t, i, i + p.size() - 1);
		}

		cout << "showing PolyHash hashes" << endl;
		for (int i = 0; i < preCompHashes.size(); i++) {
			cout << i << " = " << preCompHashes[i] << endl;
		}
#endif

		for (int i = begin - 1; i >= 0; i--) {
			long long large = (multiplier * preCompHashes[i + 1])  + t[i] - y * t[i + p.size()];
			preCompHashes[i] = (ull) large % prime;
#if DEBUG
			cout << "preCompHash = " << preCompHashes[i] << endl;
#endif
		}

		size_t pHash = PolyHash(p, 0, p.size() - 1);

#if DEBUG
	cout << "showing incremental hashes" << endl;
		for (int i = 0; i < preCompHashes.size(); i++) {
			cout << i << " = " << preCompHashes[i] << endl;
		}

		cout << "pHash = " << pHash << endl;
#endif

		for (int i = 0; i < (t.size() - p.size() + 1); i++) {
			if (pHash == preCompHashes[i]) {
#if DEBUG
				cout << "potential match at " << i << endl;
#endif
				if (CheckSameStr(t, i, i + p.size() - 1, p, 0, p.size() -1)) {
					ans.push_back(i);
				}
			}
		}

		return ans;
}


int main() {
		std::ios_base::sync_with_stdio(false);
		print_occurrences(get_occurrences(read_input()));
		return 0;
}
