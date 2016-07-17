#include <climits>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <vector>

#define DEBUG 0

using namespace std;

long long eval(long long a, long long b, char op) {
	if (op == '*') {
		return a * b;
	} else if (op == '+') {
		return a + b;
	} else if (op == '-') {
		return a - b;
	} else {
		assert(0);
	}
}

long long max4(long long a, long long b, long long c, long long d) {
	long long max_ab = max(a, b);
	long long max_cd = max(c, d);
	return max(max_ab, max_cd);
}

long long min4(long long a, long long b, long long c, long long d) {
	long long _ab = min(a, b);
	long long _cd = min(c, d);
	return min(_ab, _cd);
}

struct MinMaxRet {
	long long min;
	long long max;
};

MinMaxRet CalcMinMax(int i, int j, const vector<unsigned>& numbers,
	const vector<char>& operations, vector<vector<long long> >& mins,
	vector<vector<long long> >& maxes) {
	MinMaxRet ret;
	long long tmp_max = LLONG_MIN;
	long long tmp_min = LLONG_MAX;

	// numbers are d_i, d_i+1, d_i+2, ... , d_j
	// operations are then op_i, op_i+1, ... , op_j-1
	for (int k = i; k < j; k++) {
		long long a = eval(maxes[i][k], maxes[k + 1][j], operations[k]);
		long long b = eval(maxes[i][k], mins[k + 1][j], operations[k]);
		long long c = eval(mins[i][k], maxes[k + 1][j], operations[k]);
		long long d = eval(mins[i][k], mins[k + 1][j], operations[k]);
		long long cur_max = max4(a, b, c, d);
		long long cur_min = min4(a, b, c, d);
#if DEBUG
		cout << "k = " << k << endl;
		//cout << "a,b,c,d = " << a << "," << b << "," << c << "," << d << endl;
#endif
		if (cur_max > tmp_max) {
			tmp_max = cur_max;
		}
		if (cur_min < tmp_min) {
			tmp_min = cur_min;
		}
	}

	ret.max = tmp_max;
	ret.min = tmp_min;
	return ret;
}

long long get_maximum_value(const string &exp) {
	// filter out bad arguments
	if (exp.size() <= 0) {
		return 0;
	} else if (exp.size() == 1) {
		return exp[0] - '0';
	}

	// count how many numbers and how many operations there are
	int num_cnt = (exp.size() + 1) / 2;
	int op_cnt = (exp.size() - 1) / 2;

	// parsed results of input operation string
	vector<unsigned> numbers(num_cnt);
	vector<char> operations(op_cnt);

	// max and min values of numbers from from i to j ... goes from 0 to op_cnt - 1
	vector<vector<long long> > minimums(num_cnt, vector<long long>(num_cnt, LLONG_MAX));
	vector<vector<long long> > maximums(num_cnt, vector<long long>(num_cnt, LLONG_MIN));

	// numbers appear at locations 0, 2, 4, 6, ...
	for (int i = 0; i < num_cnt; i++) {
		numbers[i] = exp[2 * i] - '0';
	}

	// operations appear at locations 1, 3, 5, ...
	for (int i = 0; i < op_cnt; i++) {
		operations[i] = exp[2 * i + 1];
	}

	// max/min of elements startina ending at index i is numers[i] itself
	for (int i = 0; i < num_cnt; i++) {
		minimums[i][i] = numbers[i];
		maximums[i][i] = numbers[i];
	}

#if DEBUG
	cout << "printing numbers" << endl;
	for (int i = 0; i < num_cnt; i++) {
		cout << numbers[i] << " ";
	}
	cout << endl;

	cout << "printing operations" << endl;
	for (int i = 0; i < op_cnt; i++) {
		cout << operations[i] << " ";
	}
	cout << endl;
#endif

#if DEBUG
	cout << "iterating over all possible start and end indices" << endl;
#endif
	int j;
	// operation lengths
	for (int seg_len = 1; seg_len < num_cnt; seg_len++) {
		// fill up minimums and maximums matrices
		 for (int i = 0; i < (num_cnt - seg_len); i++) {
			j = i + seg_len;
#if DEBUG
			cout << "(i, j) = " << i << ", " << j << endl;
#endif
			MinMaxRet ret = CalcMinMax(i, j, numbers, operations, minimums, maximums);
			minimums[i][j] = ret.min;
			maximums[i][j] = ret.max;
		}
	}

#if DEBUG
	cout << "minimums/maximums" << endl;
	std::cout.precision(5);
	for (int i = 0; i < num_cnt; i++) {
		for (int j = 0; j < num_cnt; j++) {
			cout << setw(10);
			if (minimums[i][j] == LLONG_MAX) cout << "NA";
			else cout << minimums[i][j];
			cout << "/";
			if (maximums[i][j] == LLONG_MIN) cout << "NA";
			else cout << maximums[i][j];
			cout << " ";
		}
		cout << endl;
	}
#endif

	return maximums[0][num_cnt - 1];
}

int main() {
	string s;
	std::cin >> s;
	std::cout << get_maximum_value(s) << '\n';
}
