#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;
using std::vector;

enum op_type {
	OpAdd1 = 0,
	OpMult2,
	OpMult3
};

vector<int> optimal_sequence(int n) {
	std::vector<int> sequence; // 0 to n
	std::vector<int> cost(n + 1, 0);
	std::vector<op_type> ops(n + 1, OpAdd1);

	for (int i = 2; i <= n; i++) {
		enum op_type min_cost_op = OpAdd1;
		int op1_cost = cost[i - 1] + 1;
		int op2_cost = INT_MAX;
		int op3_cost = INT_MAX;
		if ((i % 2) == 0) {
			op2_cost = cost[i/2] + 1;
		}
		if ((i % 3) == 0) {
			op3_cost = cost[i/3] + 1;
		}

		if (op1_cost > op2_cost) {
			if (op2_cost > op3_cost) {
				min_cost_op = OpMult3;
			} else {
				min_cost_op = OpMult2;
			}
		} else {
			if (op1_cost > op3_cost) {
				min_cost_op = OpMult3;
			} else {
				min_cost_op = OpAdd1;
			}
		}

		switch (min_cost_op) {
			case OpAdd1:
				cost[i] = op1_cost;
				break;
			case OpMult2:
				cost[i] = op2_cost;
				break;
			case OpMult3:
			default:
				cost[i] = op3_cost;
				break;
		}

		ops[i] = min_cost_op;
	}

	int elem = n;
	while (elem > 0) {
		sequence.push_back(elem);
		if (ops[elem] == OpAdd1) {
			elem -= 1;
		}
		else if (ops[elem] == OpMult2) {
			elem /= 2;
		}
		else {
			elem /= 3;
		}
	}
	reverse(sequence.begin(), sequence.end());
	return sequence;
}

int main() {
	int n;
	std::cin >> n;
	vector<int> sequence = optimal_sequence(n);
	std::cout << sequence.size() - 1 << std::endl;
	for (size_t i = 0; i < sequence.size(); ++i) {
		std::cout << sequence[i] << " ";
	}
}
