#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void sub_row_from_rows(vector<vector<float> >& matrix, int r, int c) {
	vector<float> sub_vec(matrix[r].size());
	float scale;

	for (int target_row = 0; target_row < matrix.size(); target_row++) {
		if ((target_row != r) && (matrix[target_row][c] != 0)) {
			scale = matrix[target_row][c] / matrix[r][c];
			std::copy(
				matrix[r].begin(),
				matrix[r].end(),
				sub_vec.begin());
			std::transform(
				sub_vec.begin(),
				sub_vec.end(),
				sub_vec.begin(),
				std::bind1st(std::multiplies<float>(), scale));
			// subtract vector
			for (int i = 0; i < matrix[target_row].size(); i++) {
				matrix[target_row][i] -= sub_vec[i];
			}
		}
	}
}

void solve(vector<vector<float> >& matrix) {
	int np_row_start = 0;
	for (int c = 0; c < (matrix[0].size() - 1); c++) {
		for (int r = np_row_start; r < matrix.size(); r++) {
			if (matrix[r][c] != 0) {
				// swap to row np_row_start
				if (r != np_row_start)
					std::swap(matrix[np_row_start], matrix[r]);

				// scale entire row so that matrix[r][c] is 1
				if (matrix[np_row_start][c] != 1)
					std::transform(
						matrix[np_row_start].begin(),
						matrix[np_row_start].end(),
						matrix[np_row_start].begin(),
						std::bind1st(std::multiplies<float>(), 1 / matrix[np_row_start][c]));

				// subtract so that other rows of this column are all 0
				sub_row_from_rows(matrix, np_row_start, c);

				np_row_start++;
				break;
			}
		}
	}
}

void show_solution(vector<vector<float> >& matrix) {
	/*
	for (int r = 0; r < matrix.size(); r++) {
		for (int c = 0; c <= matrix.size(); c++)
			printf("%0.6f ", matrix[r][c]);
		cout << endl;
	}
	*/
	for (int r = 0; r < matrix.size(); r++)
		printf("%0.6f ", matrix[r][matrix.size()]);
	cout << endl;
}

int main() {
	int n;
	cin >> n;

	if (n > 0) {
		vector<vector<float> > matrix(n);
		for (int r = 0; r < n; r++) {
			matrix[r].resize(n + 1);
			for (int c = 0; c <= n; c++)
				cin >> matrix[r][c];
		}

		solve(matrix);
		show_solution(matrix);
	}

	return 0;
}
