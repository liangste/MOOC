#include <iostream>
#include <vector>
#include <stack>
#include <stdint.h>

using namespace std;

int reach(vector<vector<int> > &adj, int x, int y) {
	//write your code here
	size_t n = adj.size();
	stack<int> S;
	bool visited[n];
	for (int i = 0; i < n; i++) {
		visited[i] = false;
	}
	S.push(x);

	while (!S.empty()) {
		int v = S.top();
		S.pop();
		if (!visited[v]) {
			visited[v] = true;
			for (int j = 0; j < adj[v].size(); j++) {
				S.push(adj[v][j]);
			}
		}
	}

	return (visited[y])? 1 : 0;
}

int main() {
	size_t n, m;
	std::cin >> n >> m; // n - number of vertices, m - number of edges
	vector<vector<int> > adj(n, vector<int>());
	for (size_t i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1);
		adj[y - 1].push_back(x - 1);
	}
	int x, y;
	std::cin >> x >> y;
	std::cout << reach(adj, x - 1, y - 1);
}
