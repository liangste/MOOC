#include <iostream>
#include <vector>

using namespace std;

#define DEBUG 1

const vector<int> colors = {0, 1, 2};

// x_ij
// i == index of vertex
// j == index of Hamiltonian path position

int GetVarNum(int i_v, int i_p, int N) {
	return N * i_v + i_p + 1;
};

void PrintCNF(const vector<vector<bool> >& adjMatrix) {
	int N = adjMatrix.size();
	int nVars = N * N;
	vector<vector<int> > clauses; // without 0 at end
	vector<int> clause;

	// all vertices must be on the path
	for (int i = 0; i < N; i++) {
		clause.clear();
		for (int j = 0; j < N; j++) {
			clause.push_back(GetVarNum(i, j, N));
		}
		clauses.push_back(clause);
	}

	// each vertex must be visited exactly once
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = j + 1; k < N; k++) {
				clauses.push_back({-GetVarNum(i, j, N), -GetVarNum(i, k, N)});
			}
		}
	}

	// there's only one vertex on each position
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			for (int k = i + 1; k < N; k++) {
				clauses.push_back({-GetVarNum(i, j, N), -GetVarNum(k, j, N)});
			}
		}
	}

	// two successive vertices must be connected by an edge
	// if no edge exists between vertices i and j, then they cannot be adjacent
	// to each other in the Hamiltonian path
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!adjMatrix[i][j]) {
				for (int k = 0; k < (N - 1); k++) {
					clauses.push_back({-GetVarNum(i, k, N), -GetVarNum(j, k + 1, N)});
					clauses.push_back({-GetVarNum(j, k, N), -GetVarNum(i, k + 1, N)});
				}
			}
		}
	}

#if DEBUG
	cout << "p cnf " << nVars << " " << clauses.size() << endl;
#else
	cout << clauses.size() << " " << nVars << endl;
#endif

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
