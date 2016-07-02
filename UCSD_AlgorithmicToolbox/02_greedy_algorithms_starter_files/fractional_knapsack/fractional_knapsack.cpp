#include <iostream>
#include <vector>

#define DEBUG 0

using namespace std;
using std::vector;

double get_optimal_value(int capacity,
												 vector<int> weights,
												 vector<int> values) {
	double value = 0.0;
	vector<double> frac_values(weights.size());

	for (int i = 0; i < weights.size(); i++) {
		frac_values[i] = (double) values[i] / (double) weights[i];
	}

	// sort by value/weight using insertion sort
	if (weights.size() > 1) {
		for (int i = 1; i < weights.size(); i++) {
			int j = i;
			while (j > 0 && frac_values[j - 1] < frac_values[j]) {
				double tmp = frac_values[j - 1];
				int tmp_w = weights[j - 1];
				int tmp_v = values[j - 1];

				frac_values[j - 1] = frac_values[j];
				weights[j - 1] = weights[j];
				values[j - 1] = values[j];

				frac_values[j] = tmp;
				weights[j] = tmp_w;
				values[j] = tmp_v;

				j--;
			}
		}
	}

#if DEBUG
	// optionally display the sorted weights and values
	std::cout << " Showing sorted weight/values" << endl;
	for (int i = 0; i < weights.size(); i++) {
		std::cout << "w = " << weights[i];
		std::cout << " v = " << values[i];
		std::cout << " v/w = " << frac_values[i] << endl;
	}
#endif

	int item_idx = 0;
	// put into knapsack
	while (capacity > 0 && item_idx < weights.size()) {
		// need to use up all of current item
		if (capacity > weights[item_idx]) {
			value += values[item_idx];
			capacity -= weights[item_idx];
		} else {
			// use up only a fraction
			value += capacity * frac_values[item_idx];
			capacity = 0;
		}
		item_idx++;
	}

	// must output at least 4 dicimal points
	return value;
}

int main() {
	int n;
	int capacity;
	std::cin >> n >> capacity;
	vector<int> values(n);
	vector<int> weights(n);
	for (int i = 0; i < n; i++) {
		std::cin >> values[i] >> weights[i];
	}

	double optimal_value = get_optimal_value(capacity,
																					 weights,
																					 values);

	std::cout << std::fixed;
	std::cout.precision(4);
	std::cout << optimal_value << std::endl;
	return 0;
}
