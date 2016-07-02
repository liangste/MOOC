#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

long long min_dot_product(vector<int> a, vector<int> b) {
	// write your code here
	long long result = 0;

	priority_queue<int, vector<int>, greater<int> > q1;
	priority_queue<int, vector<int>, less<int> > q2;

	if (a.size() != b.size()) {
		cout << "a.size() = " << a.size() << endl;
		cout << "b.size() = " << b.size() << endl;
		return result;
	}

	for (int i = 0; i < a.size(); i++) {
		q1.push(a[i]);
		q2.push(b[i]);
	}

	while (q1.size() > 0 && q2.size() > 0) {
		result += (long long) q1.top() * (long long) q2.top();
		q1.pop();
		q2.pop();
	}

	// sort a in non-decreasing order
	// sort b in non-increasing order
	// multiple a and b
	return result;
}

int main() {
	size_t n;
	std::cin >> n;
	vector<int> a(n), b(n);
	for (size_t i = 0; i < n; i++) {
		std::cin >> a[i];
	}
	for (size_t i = 0; i < n; i++) {
		std::cin >> b[i];
	}
	std::cout << min_dot_product(a, b) << std::endl;
}
