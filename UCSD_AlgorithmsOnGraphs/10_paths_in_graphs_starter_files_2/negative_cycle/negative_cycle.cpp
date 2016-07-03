#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int negative_cycle(vector<vector<int> > &adj, vector<vector<int> > &cost) {
	//write your code here
	vector<int> dist(adj.size());
	vector<int> dist2(adj.size());

	for (int i = 0; i < adj.size(); i++) {
		dist[i] = INT_MAX/2;
	}

	dist[0] = 0; // let node 0 be source

	for (int i = 1; i < adj.size(); i++) { // |V| - 1 times
		// for all edges
		for (int u = 0; u < adj.size(); u++) {
			for (int v = 0; v < adj[u].size(); v++) {
				int uu = u;
				int vv = adj[uu][v];

				if ((dist[uu] + cost[u][v]) < dist[vv]) {
					dist[vv] = dist[uu] + cost[u][v];
				}
			}
		}
	}

	dist2 = dist;

	for (int i = 1; i < adj.size(); i++) { // |V| - 1 times
		// for all edges
		for (int u = 0; u < adj.size(); u++) {
			for (int v = 0; v < adj[u].size(); v++) {
				int uu = u;
				int vv = adj[uu][v];

				if ((dist2[uu] + cost[u][v]) < dist2[vv]) {
					dist2[vv] = dist2[uu] + cost[u][v];
				}
			}
		}
	}

	for (int i = 0; i < dist.size(); i++) {
		if (dist2[i] < dist[i])
			return 1;
	}

	return 0;
}

int main() {
	int n, m;
	std::cin >> n >> m;
	vector<vector<int> > adj(n, vector<int>());
	vector<vector<int> > cost(n, vector<int>());
	for (int i = 0; i < m; i++) {
		int x, y, w;
		std::cin >> x >> y >> w;
		adj[x - 1].push_back(y - 1);
		cost[x - 1].push_back(w);
	}
	std::cout << negative_cycle(adj, cost);
}
