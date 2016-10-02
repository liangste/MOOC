#include <iostream>
#include <vector>

using namespace std;

const vector<int> colors = {0, 1, 2};

int GetVarNum(int nodeIdx, int color) {
	// input is 0-index, but output is 1-indexed (so can be converted to negative)
	return 3 * nodeIdx + color + 1;
}

void PrintCNF(const vector<vector<int> >& adjList) {
	int nVars = adjList.size() * 3; // each vertex * 3 colors
	vector<vector<int> > clauses; // without 0 at end

	// for each vertex, choose one color
	for (int i = 0; i < adjList.size(); i++) {
		clauses.push_back({GetVarNum(i, 0), GetVarNum(i, 1), GetVarNum(i, 2)});
		clauses.push_back({-GetVarNum(i, 0), -GetVarNum(i, 1)});
		clauses.push_back({-GetVarNum(i, 0), -GetVarNum(i, 2)});
		clauses.push_back({-GetVarNum(i, 2), -GetVarNum(i, 1)});
	}

	// for each edge, colors must be different
	for (int i = 0; i < adjList.size(); i++) {
		for (int j = 0; j < adjList[i].size(); j++) {
			// edge between i and adjList[i][j]
			int a = i;
			int b = adjList[i][j];

			clauses.push_back({-GetVarNum(a, 0), -GetVarNum(b, 0)});
			clauses.push_back({-GetVarNum(a, 1), -GetVarNum(b, 1)});
			clauses.push_back({-GetVarNum(a, 2), -GetVarNum(b, 2)});
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

	vector<vector<int> > adjList(n);

	int a, b;
	for (int i = 0; i < m; i++) {
		cin >> a >> b;
		adjList[a - 1].push_back(b - 1);
	}

	PrintCNF(adjList);
};
