#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

int calc_fib(int n, vector<int>& fibs) {
		if (n <= 1)
				return n;

		fibs[0] = 0;
		fibs[1] = 1;

		for (int i = 2; i <= n; i++) {
			fibs[i] = fibs[i - 1] + fibs[i - 2];
		}

		return fibs[n];
}

int main() {
		int n = 0;
		std::cin >> n;
		vector<int> fibs;

		fibs.resize(n + 1);

		std::cout << calc_fib(n, fibs) << '\n';
		return 0;
}
