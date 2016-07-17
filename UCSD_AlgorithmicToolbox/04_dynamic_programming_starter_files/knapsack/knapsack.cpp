#include <iostream>
#include <vector>
#include <string.h>

using namespace std;
using std::vector;

int optimal_weight(int W, const vector<int> &w) {
	//write your code here
	vector<vector<int> > value(W + 1, vector<int>(w.size() + 1, 0));

	for (int i = 1; i <= w.size(); i++) {
		for (int weight = 1; weight <= W; weight++) {
			value[weight][i] = value[weight][i - 1];
			int cur_value = value[weight][i];
			int cur_weight = w[i - 1];
			if (cur_weight <= weight) { // if item i fits, we consider it
				int new_value = value[weight - cur_weight][i - 1] + cur_weight;
				if (new_value > cur_value) {
					value[weight][i] = new_value;
				}
			}
		}
	}

	/*
	for (int w_i = 0; w_i <= w.size(); w_i++) {
		cout << "i = " << w_i << " : ";
		for (int weight = 0; weight <= W; weight++) {
			cout << value[weight][w_i] << " ";
		}
		cout << endl;
	}
	*/

	return value[W][w.size()];
}

int main() {
	int n, W;
	std::cin >> W >> n;
	vector<int> w(n);
	for (int i = 0; i < n; i++) {
		std::cin >> w[i];
	}
	std::cout << optimal_weight(W, w) << '\n';
}
