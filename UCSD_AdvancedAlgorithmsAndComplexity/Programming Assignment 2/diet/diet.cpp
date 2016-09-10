#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

typedef vector<vector<double>> Matrix_t;

void sub_row_from_rows(vector<vector<double> >& matrix, int r, int c) {
	vector<double> sub_vec(matrix[r].size());
	double scale;

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
				std::bind1st(std::multiplies<double>(), scale));
			// subtract vector
			for (int i = 0; i < matrix[target_row].size(); i++) {
				matrix[target_row][i] -= sub_vec[i];
			}
		}
	}
}

void solve(vector<vector<double> >& matrix) {
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
						std::bind1st(std::multiplies<double>(), 1 / matrix[np_row_start][c]));

				// subtract so that other rows of this column are all 0
				sub_row_from_rows(matrix, np_row_start, c);

				np_row_start++;
				break;
			}
		}
	}
}

vector<double> show_solution(vector<vector<double> >& matrix) {
  vector<double> sol;
	for (int r = 0; r < matrix.size(); r++)
		sol.push_back(matrix[r][matrix.size()]);
	return sol;
}

pair<int, vector<double>> solve_diet_problem(int n, int m, Matrix_t A, vector<double> b, vector<double> c) {
  // Write your code here

  // solve subset of marix A, use subset matrix n by n to get a vertex to be used
  // by our simplex algorithm, if cannot be solved, then we don't have a solution
  if ((n < m))
    return {1, vector<double>(m, 0)};

  vector<double> vertex;
  if (n > 0) {
    // get m by m subset matrix
		vector<vector<double> > subset_matrix(m, vector<double>(m));
		for (int i = 0; i < m; i++) {
      subset_matrix[i] = A[i];
      subset_matrix[i].push_back(b[i]);
    }
		solve(subset_matrix);
		vertex = show_solution(subset_matrix);
	}

  if (vertex.size() == 0)
    return {-1, vector<double>(m, 0)};

  // apply constraint that vertices amounts must be positive
  for (int i = 0; i < vertex.size(); i++)
    if (vertex[i] <= 0)
      vertex[i] = 0.0;

  for (int i = 0; i < vertex.size(); i++)
    cout << vertex[i] << " ";
  cout << endl;

  return {0, vector<double>(m, 0)};
}

int main(){
  int n, m;
  cin >> n >> m;
  Matrix_t A(n, vector<double>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> A[i][j];
    }
  }
  vector<double> b(n);
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }
  vector<double> c(m);
  for (int i = 0; i < m; i++) {
    cin >> c[i];
  }

  pair<int, vector<double>> ans = solve_diet_problem(n, m, A, b, c);

  switch (ans.first) {
    case -1:
      printf("No solution\n");
      break;
    case 0:
      printf("Bounded solution\n");
      for (int i = 0; i < m; i++) {
        printf("%.18f%c", ans.second[i], " \n"[i + 1 == m]);
      }
      break;
    case 1:
      printf("Infinity\n");
      break;
  }
  return 0;
}
