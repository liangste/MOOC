#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <limits>

using namespace std;
#define DEBUG 0

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

vector<vector<int> > getAllSubsets(vector<int> set)
{
    vector< vector<int> > subset;
    vector<int> empty;
    subset.push_back( empty );

    for (int i = 0; i < set.size(); i++)
    {
        vector< vector<int> > subsetTemp = subset;

        for (int j = 0; j < subsetTemp.size(); j++)
            subsetTemp[j].push_back( set[i] );

        for (int j = 0; j < subsetTemp.size(); j++)
            subset.push_back( subsetTemp[j] );
    }
    return subset;
}

vector<vector<int> > getSizedSubsets(vector<vector<int> >& all_sets, int size) {
	vector<vector<int> > subset;
	for (int i = 0; i < all_sets.size(); i++)
		if (all_sets[i].size() == size)
			subset.push_back(all_sets[i]);
	return subset;
}

bool isValidSolution(vector<vector<double>>& A, vector<double>& b, vector<double>& sol) {

	if (sol.size() == 0)
		return false;

	double tmp;
	for (int i = 0; i < A.size(); i++) {
		tmp = 0.0F;
		for (int j = 0; j < A[i].size(); j++) {
			tmp += A[i][j] * sol[j];
		}
		if (tmp > b[i])
			return false;
	}
	return true;
}

double getPleasure(vector<double>& vertex, vector<double>& c) {
	double p = 0.0;
	for (int i = 0; i < vertex.size(); i++) {
		p += vertex[i] * c[i];
	}
	return p;
}

double VeryBigNumber = 1000000000.0;

pair<int, vector<double>> solve_diet_problem(int n, int m, vector<vector<double>>& A,
	vector<double>& b, vector<double>& c) {

	vector<double> vertex;
	vector<double> sol_vertex;

	if (n <= 0) // no constraints but >= 0
		return {1, sol_vertex};

	if (m <= 0) // nothing to solve
		return {-1, sol_vertex}; // no solution

	// build matrix of n + m inequalities
	for (int i = 0; i < m; i++) {
		vector<double> new_row(m, 0.0);
		new_row[i] = -1.0;
		A.push_back(new_row);
		b.push_back(0.0);
	}

	vector<int> set;

	for (int i = 0; i < A.size(); i++)
		set.push_back(i);

	vector<vector<int> > all_subsets = getAllSubsets(set);
	vector<vector<int> > subsets = getSizedSubsets(all_subsets, m);

#if DEBUG
		cout << "Showing constraint matrix" << endl;;
		for (int i = 0; i < A.size(); i++) {
			cout << "  ";
			for (int j = 0; j < A[i].size(); j++) {
				cout << A[i][j] << " ";
			}
			cout << b[i] << endl;
		}
#endif

	volatile double optimal_pleasure = std::numeric_limits<double>::min();
	vector<vector<double> > subset_matrix;
	volatile double p;
	volatile bool found_sol = false;
	for (int s = 0; s < subsets.size(); s++) {
		subset_matrix.clear();
		for (int i = 0; i < subsets[s].size(); i++) {
			subset_matrix.push_back(A[subsets[s][i]]);
			subset_matrix[i].push_back(b[subsets[s][i]]);
		}

#if DEBUG
		cout << "Solving sub-matrix" << endl;;
		for (int i = 0; i < subset_matrix.size(); i++) {
			cout << "| ";
			for (int j = 0; j < subset_matrix[i].size(); j++) {
				cout << subset_matrix[i][j] << " ";
			}
			cout << endl;
		}
#endif

		solve(subset_matrix);
		vertex.clear();
		vertex = show_solution(subset_matrix);

#if DEBUG
		cout << " found solution ";
		for (int i = 0; i < vertex.size(); i++) {
			cout << vertex[i] << ", ";
		} cout << endl;
#endif

		if (isValidSolution(A, b, vertex)) {
			found_sol = true;
			// check if largest
			p = getPleasure(vertex, c);
			if (p > optimal_pleasure) {
				optimal_pleasure = p;
				sol_vertex = vertex;
			}
		}
	}

	// add row of 1, 1, ... 1, 1 to solve the Infinity problem
	int augmented_index = A.size();
	A.push_back(vector<double>(m, 1.0));
	b.push_back(VeryBigNumber);

#if DEBUG
		cout << "Showing augmented-constraint matrix" << endl;;
		for (int i = 0; i < A.size(); i++) {
			cout << "  ";
			for (int j = 0; j < A[i].size(); j++) {
				cout << A[i][j] << " ";
			}
			cout << b[i] << endl;
		}
#endif

	set.clear();
	all_subsets.clear();
	subsets.clear();
	for (int i = 0; i < A.size(); i++)
		set.push_back(i);
	all_subsets = getAllSubsets(set);
	subsets = getSizedSubsets(all_subsets, m);

	bool infinity = false;
	for (int s = 0; s < subsets.size(); s++) {
		subset_matrix.clear();
		for (int i = 0; i < subsets[s].size(); i++) {
			subset_matrix.push_back(A[subsets[s][i]]);
			subset_matrix[i].push_back(b[subsets[s][i]]);
		}

#if DEBUG
		cout << "Solving augmented sub-matrix from sub-sets: ";
		for (int i = 0; i < subsets[s].size(); i++) {
			cout << subsets[s][i] << " ";
		} cout << endl;
		for (int i = 0; i < subset_matrix.size(); i++) {
			cout << "| ";
			for (int j = 0; j < subset_matrix[i].size(); j++) {
				cout << subset_matrix[i][j] << " ";
			}
			cout << endl;
		}
#endif

		solve(subset_matrix);
		vertex.clear();
		vertex = show_solution(subset_matrix);

#if DEBUG
		cout << " found solution ";
		for (int i = 0; i < vertex.size(); i++) {
			cout << vertex[i] << ", ";
		} cout << endl;
#endif

		if (isValidSolution(A, b, vertex)) {
			for (int i = 0; i < subsets[s].size(); i++) {
				if (subsets[s][i] == augmented_index) {
					infinity = true;
					break;
				}
			}

			if (infinity)
				break;
		}
	}

	if (infinity)
		return {1, sol_vertex};

	if (found_sol)
		return {0, sol_vertex};
	else
		return {-1, sol_vertex}; // no solution
}

int main(){
  int n, m;
  cin >> n >> m;
  vector<vector<double>> A(n, vector<double>(m));
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
      for (int i = 0; i < ans.second.size(); i++) {
        printf("%.18f%c", ans.second[i], " \n"[i + 1 == m]);
      }
      break;
    case 1:
      printf("Infinity\n");
      break;
  }
  return 0;
}
