#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

#define DEBUG 0

struct part3ret {
	part3ret(int m_in, int n_in) : m(m_in), n(n_in) {};

	int m;
	int n;
};

part3ret partition2(vector<int> &a, int l, int r) {

	int x = a[r]; // pivot
	int j = l;
	for (int i = l; i < r; i++) {
		if (a[i] <= x) {
#if DEBUG
			cout << "swap a[" << i << "] = " << a[i] << " and a[" << j << "] = " << a[j] << endl;
#endif
			swap(a[i], a[j]);
			j++;
		}
#if DEBUG
		cout << "it[" << i << "]";
		for (int u = 0; u < a.size(); u++) {
			cout << " " << a[u];
		}
		cout << " (i,j) = " << i << "," << j;
		cout << endl;
#endif
	}
	swap(a[r], a[j]);
#if DEBUG
	cout << "pivot ended at " << j << endl;
#endif

	int m, n;
	m = n = j;

	// to right-most index equal to pivot
	while ((n + 1) <= r && a[n + 1] == x) {
		n++;
	}

	// to left-most index equal to pivot
	while ((m - 1) >= l && a[m - 1] == x) {
		m--;
	}

#if DEBUG
	cout << " (m,n) = " << m << "," << n;
	cout << endl;
#endif

	return part3ret(m, n);
}

void randomized_quick_sort(vector<int> &a, int l, int r) {

	if (l == r)
		return;

#if DEBUG
	cout << "(l, r) = " << l << "," << r << endl;
#endif

	int k;
	int m = (l + r) / 2;

	// choose median element as pivot
	if (a[l] > a[r]) {
		if (a[l] > a[m])
			k = m;
		else
			k = l;
	} else {
		if (a[r] > a[m])
			k = m;
		else
			k = r;
	}
	swap(a[r], a[k]); // set last element as pivot

#if DEBUG
	cout << "===============================" << endl;
	cout << "be4r ";
	for (int i = 0; i < a.size(); i++) {
		cout << " " << a[i];
	}
	cout << endl;

	cout << "pivot x = a[" << k << "] = " << a[r] << endl;
#endif

	part3ret ret = partition2(a, l, r);

#if DEBUG
	cout << "done ";
	for (int i = 0; i < a.size(); i++) {
		cout << " " << a[i];
	}
	cout << endl;
	cout << "===============================" << endl;
#endif

	// partition left of pivot
	if (ret.m > l) {
		randomized_quick_sort(a, l, ret.m - 1);
	}

	// partition right of pivot
	if (ret.n < r) {
		randomized_quick_sort(a, ret.n + 1, r);
	}
}

int main() {
	int n;
	std::cin >> n;
	vector<int> a(n);
	for (size_t i = 0; i < a.size(); ++i) {
		std::cin >> a[i];
	}
	randomized_quick_sort(a, 0, a.size() - 1);
	for (size_t i = 0; i < a.size(); ++i) {
		std::cout << a[i] << ' ';
	}
}
