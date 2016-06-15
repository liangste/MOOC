#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;

long long MaxPairwiseProduct(const vector<int>& numbers) {
	int result = 0;
	int n = numbers.size();
	int max = -1;
	int secondMax = -1;
	for (int i = 0; i < n; ++i) {
		if (numbers[i] > max) {
			if (max > secondMax)
				secondMax = max;
			max = numbers[i];
		} else if (numbers[i] > secondMax) {
			secondMax = numbers[i];
		}
	}
	return (long long)secondMax * (long long)max;
}

int main() {
		int n;
		cin >> n;
		vector<int> numbers(n);
		for (int i = 0; i < n; ++i) {
				cin >> numbers[i];
		}

		cout << MaxPairwiseProduct(numbers) << std::endl;
		return 0;
}
