#include <iostream>
#include <stack>
#include <vector>

using namespace std;

// explore and mark all nodes reacheable from x
void explore(vector<vector<int> > &adj, bool visited[], int x) {
	stack<int> S;

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
}

int number_of_components(vector<vector<int> > &adj) {
	int res = 0;
	size_t n = adj.size();
	bool visited[n];

	for (int i = 0; i < n; i++) {
		visited[i] = false;
	}

	for (int i = 0; i < n; i ++) {
		if (!visited[i]) {
			explore(adj, visited, i);
			res++;
		}
	}

	return res;
}

int main() {
	size_t n, m;
	std::cin >> n >> m;
	vector<vector<int> > adj(n, vector<int>());
	for (size_t i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1);
		adj[y - 1].push_back(x - 1);
	}
	std::cout << number_of_components(adj);
}
