#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include <limits>

using namespace std;

///
///		Guassian Elimination
///
typedef enum GaussianEliminationResultEnum {
	SingleSolution = 0,
	NoSolution,
	MultipleSolutions
} GaussianEliminationResult_t;

void sub_row_from_rows(vector<vector<double> >& matrix, int r, int c) {
	vector<double> sub_vec(matrix[r].size());
	double scale;
	for (int target_row = 0; target_row < matrix.size(); target_row++) {
		if ((target_row != r) && (matrix[target_row][c] != 0.0)) {
			scale = matrix[target_row][c] / matrix[r][c];
			std::copy(matrix[r].begin(), matrix[r].end(), sub_vec.begin());
			std::transform(sub_vec.begin(), sub_vec.end(), sub_vec.begin(), std::bind1st(std::multiplies<double>(), scale));
			// subtract vector
			for (int i = 0; i < matrix[target_row].size(); i++) {
				matrix[target_row][i] -= sub_vec[i];
			}
		}
	}
}

void solve(vector<vector<double> >& matrix) {
	assert(matrix.size() > 0);

	int np_row_start = 0;
	for (int c = 0; c < (matrix[0].size() - 1); c++) {
		for (int r = np_row_start; r < matrix.size(); r++) {
			if (matrix[r][c] != 0) {
				// swap to row np_row_start
				if (r != np_row_start)
					std::swap(matrix[np_row_start], matrix[r]);

				// scale entire row so that matrix[r][c] is 1
				if (matrix[np_row_start][c] != 1.0)
					std::transform(
						matrix[np_row_start].begin(),
						matrix[np_row_start].end(),
						matrix[np_row_start].begin(),
						std::bind1st(std::multiplies<double>(), 1.0 / matrix[np_row_start][c]));

				// subtract so that other rows of this column are all 0
				sub_row_from_rows(matrix, np_row_start, c);

				np_row_start++;
				break;
			}
		}
	}
}

GaussianEliminationResultEnum show_solution(vector<vector<double> >& matrix, vector<double>& sol) {

	for (int r = 0; r < matrix.size(); r++) {
		assert(matrix.size() == matrix[r].size() - 1);
		if (matrix[r][r] != 1.0) {
			for (int c = 0; c < matrix[r].size(); c++) {
				if (matrix[r][c] != 0.0) {
					return NoSolution;
				}
			}
			return MultipleSolutions;
		}
	}

	for (int r = 0; r < matrix.size(); r++)
		sol.push_back(matrix[r][matrix.size()]);

	return SingleSolution;
}

void test_no_solution() {
	cout << " == testing Gaussian Elimination with No Solution ==" << endl;
	vector<vector<double> > test_matrix;
	test_matrix.push_back({1, 1, -1, 2});
	test_matrix.push_back({2, 3, -1, 0});
	test_matrix.push_back({3, 4, -2, 1});
	solve(test_matrix);

	for (int r = 0; r < test_matrix.size(); r++) {
		for (int c = 0; c < test_matrix[r].size(); c++) {
			cout << test_matrix[r][c] << " ";
		}
		cout << endl;
	}

	vector<double> sol;
	GaussianEliminationResultEnum result = show_solution(test_matrix, sol);
	if (NoSolution == result) {
		cout << "Correct, no solution" << endl;
	}
}

void test_multiple_solution() {
	cout << " == testing Gaussian Elimination with Multiple Solution ==" << endl;
	vector<vector<double> > test_matrix;
	test_matrix.push_back({1, 1, 1, 3});
	test_matrix.push_back({2, 4, 1, 8});
	test_matrix.push_back({6, 10, 4, 22});
	solve(test_matrix);

	for (int r = 0; r < test_matrix.size(); r++) {
		for (int c = 0; c < test_matrix[r].size(); c++) {
			cout << test_matrix[r][c] << " ";
		}
		cout << endl;
	}

	vector<double> sol;
	GaussianEliminationResultEnum result = show_solution(test_matrix, sol);
	if (MultipleSolutions == result) {
		cout << "Correct, multiple solutions" << endl;
	}
}

void test_single_solution() {
	cout << " == testing Gaussian Elimination with Single Solution ==" << endl;
	vector<vector<double> > test_matrix;
	test_matrix.push_back({1, 1, -1, 9});
	test_matrix.push_back({0, 1, 3, 3});
	test_matrix.push_back({-1, 0, -2, 2});
	solve(test_matrix);
	vector<double> sol;
	GaussianEliminationResultEnum result = show_solution(test_matrix, sol);
	if (SingleSolution == result) {
		cout << "Correct, single solution" << endl;
	}
}

int main(void) {
	test_no_solution();
	test_multiple_solution();
	test_single_solution();
}
