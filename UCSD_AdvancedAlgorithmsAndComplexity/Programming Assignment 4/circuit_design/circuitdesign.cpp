#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<pair<int, int>> vpii;
typedef vector<vector<int>> vvi;

#define LOOP(x, m) for(int x = 0; x < m; ++x)
#define UNASSIGNED -1

int g_nVars, g_nClauses;
vvi g_adjList;

int ToIndex(int i) {
	return (i > 0) ? i - 1 : g_adjList.size() + i;
}

int GetNegIndex(int i) {
	if (i < g_nVars) return g_adjList.size() - i - 1;
	return g_adjList.size() - i - 1;
}

void AddImplication(int a, int b) {
	g_adjList[ToIndex(-a)].push_back(ToIndex(b));
	g_adjList[ToIndex(-b)].push_back(ToIndex(a));
}

void DFS(vvi& adjList, int s, vb& visited, vi& visitOrders) {
	visited[s] = true;
	LOOP(j, adjList[s].size())
		if (!visited[adjList[s][j]])
			DFS(adjList, adjList[s][j], visited, visitOrders);
	visitOrders.push_back(s); // post-visit order
}

void VisitAndMarkSCC(int idx, vb& removed, int sccNum, vi& sccs) {
	if (!removed[idx]) {
		sccs[idx] = sccNum;
		removed[idx] = true;
		LOOP(i, g_adjList[idx].size())
			if (!removed[g_adjList[idx][i]]) {
				VisitAndMarkSCC(g_adjList[idx][i], removed, sccNum, sccs);
			}
	}
}

void ComputeSCCs(vi& sccs) {
	sccs.resize(g_adjList.size());

	// build reverse graph
	vvi adjListR(g_adjList.size());
	LOOP(i, g_adjList.size())
		LOOP(j, g_adjList[i].size())
			adjListR[g_adjList[i][j]].push_back(i);

	// get nodes by decreasing post-visit numbers
	vb visited(adjListR.size(), false);
	vi visitOrders;
	LOOP(i, adjListR.size())
		if (!visited[i])
			DFS(adjListR, i, visited, visitOrders);
	reverse(visitOrders.begin(), visitOrders.end());

	// compute SCCs
	int sccNum = 0;
	vb removed(g_adjList.size(), false);
	sccs.resize(g_adjList.size());
	LOOP(i, visitOrders.size())
		if (!removed[visitOrders[i]]) {
			VisitAndMarkSCC(visitOrders[i], removed, sccNum, sccs);
			sccNum++;
		}
}

struct SccNode {
	int idx;
	int sccNum;

	bool operator<(const SccNode& other) const {
		return sccNum < other.sccNum;
	}

	bool operator>(const SccNode& other) const {
		return sccNum > other.sccNum;
	}
};

int main(void) {
	cin >> g_nVars >> g_nClauses;
	g_adjList.resize(g_nVars * 2);

	int a, b;
	while (g_nClauses--) {
		cin >> a >> b;
		AddImplication(a, b);
	}

	vi sccs;
	ComputeSCCs(sccs);

	// if x and ~x lies in same SCC, not satisfiable
	LOOP(i, g_nVars)
		if (sccs[i] == sccs[ToIndex(-1)]) {
			cout << "UNSATISFIABLE" << endl;
			return 0;
		}

	vb assigned(g_adjList.size(), false);

	priority_queue<SccNode, vector<SccNode>, greater<SccNode>> sccNodeQ;
	LOOP(i, sccs.size())
		sccNodeQ.push({i, sccs[i]});

	vi assignments(g_adjList.size(), UNASSIGNED);
	while (!sccNodeQ.empty()) {
		SccNode node = sccNodeQ.top();
		sccNodeQ.pop();
		if (assignments[node.idx] == UNASSIGNED) {
			assignments[node.idx] = 1;
			assignments[GetNegIndex(node.idx)] = 0;
		}
	}

	cout << "SATISFIABLE" << endl;
	LOOP(i, g_nVars)
		cout  << (assignments[i]? i + 1 : -(i + 1)) << " ";
	cout << endl;
	return 0;
}
