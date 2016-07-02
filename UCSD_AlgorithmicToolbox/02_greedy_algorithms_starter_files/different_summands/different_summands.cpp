#include <iostream>
#include <vector>

using std::vector;

vector<int> optimal_summands(int n) {
	vector<int> summands;
	int cur_num = 1;

	while (n > 0) {
		if ((cur_num * 2) < n) {
			summands.push_back(cur_num);
			n -= cur_num;
			cur_num++;
		} else {
			summands.push_back(n);
			n = 0;
		}
	}

	return summands;
}

int main() {
	int n;
	std::cin >> n;
	vector<int> summands = optimal_summands(n);
	std::cout << summands.size() << '\n';
	for (size_t i = 0; i < summands.size(); ++i) {
		std::cout << summands[i] << ' ';
	}
}
