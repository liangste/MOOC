#include <iostream>
#include <vector>

using namespace std;

const vector<int> colors = {0, 1, 2};

// x_ij
// i == position in Hamiltonian path
// j == node index

int GetVarNum(int posIdx, int nodeIdx, int N) {
	return N * posIdx + nodeIdx + 1;
};

void PrintCNF(const vector<vector<bool> >& adjMatrix) {
	int N = adjMatrix.size();
	int nVars = N * N;
	vector<vector<int> > clauses; // without 0 at end

	// each node must appear in the path
	for (int j = 0; j < N; j++) {
		vector<int> clause;
		clause.clear();
		for (int i = 0; i < N; i++) {
			clause.push_back(GetVarNum(i, j, N));
		}
		clauses.push_back(clause);
	}

	// no node j appears twice in the path
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				if (i != k) {
					clauses.push_back({-GetVarNum(i, j, N), -GetVarNum(k, j, N)});
				}
			}
		}
	}

	// every position i on the path must be occupied
	for (int i = 0; i < N; i++) {
		vector<int> clause;
		clause.clear();
		for (int j = 0; j < N; j++) {
			clause.push_back(GetVarNum(i, j, N));
		}
		clauses.push_back(clause);
	}

	// no two noes j and k occupy the same position in the path
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				if (j != k) {
					clauses.push_back({-GetVarNum(i, j, N), -GetVarNum(i, k, N)});
				}
			}
		}
	}

	// nonadjacent nodes i and j cannot be adjacent in the path
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < (N - 1); k++) {
				if (!adjMatrix[i][j]) {
					clauses.push_back({-GetVarNum(k, i, N), -GetVarNum(k + 1, j, N)});
				}
				if (!adjMatrix[j][i]) {
					clauses.push_back({-GetVarNum(k, i, N), -GetVarNum(k + 1, j, N)});
				}
			}
		}
	}

	cout << clauses.size() << " " << nVars << endl;
	for (int i = 0; i < clauses.size(); i++) {
		for (int j = 0; j < clauses[i].size(); j++) {
			cout << clauses[i][j] << " ";
		}
		cout << "0" << endl;
	}
}

int main(void) {
	int n, m;
	cin >> n >> m;

	vector<vector<bool> > adjMatrix(n, vector<bool>(n, false));

	int a, b;
	for (int i = 0; i < m; i++) {
		cin >> a >> b;
		adjMatrix[a - 1][b - 1] = true;
		adjMatrix[b - 1][a - 1] = true;
	}

	PrintCNF(adjMatrix);
};
