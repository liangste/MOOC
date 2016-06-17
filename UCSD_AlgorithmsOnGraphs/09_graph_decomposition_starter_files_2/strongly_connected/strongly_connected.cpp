#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;
using std::pair;

void explore(vector<vector<int> > &adj, vector<int> &used, int x) {
	int nbr_size = adj[x].size();

	used[x] = 1;
	for (size_t i = 0; i < nbr_size; i++) {
		if (used[adj[x][i]] == 0) // not visited
		{
			explore(adj, used, adj[x][i]);
		}
	}
}

void dfs(vector<vector<int> > &adj, vector<int> &used, vector<int> &order, int x) {
	//write your code here
	int nbr_size = adj[x].size();

	used[x] = 1;
	for (size_t i = 0; i < nbr_size; i++) {
		if (used[adj[x][i]] == 0) // not visited
		{
			dfs(adj, used, order, adj[x][i]);
		}
	}

	order.push_back(x);
}

int number_of_strongly_connected_components(vector<vector<int> > adj) {
	int result = 0;

	// create reverse graph
	vector<vector<int> > adj_R(adj.size(), vector<int>());
	vector<int> used(adj.size(), 0);
	vector<int> order;

	for (size_t i = 0; i < adj.size(); ++i) {
		used[i] = 0; // clear all visited markers
	}

	for (size_t i = 0; i < adj.size(); i++) {
		for (size_t j = 0; j < adj[i].size(); j++) {
			adj_R[adj[i][j]].push_back(i);
		}
	}

	for (size_t i = 0; i < adj_R.size(); ++i) {
		if (used[i] == 0) {
			dfs(adj_R, used, order, i);
		}
	}

	std::reverse(order.begin(), order.end());

	for (size_t i = 0; i < adj.size(); ++i) {
		used[i] = 0; // clear all visited markers
	}

	for (size_t i = 0; i < order.size(); i++) {
		int v = order[i];
		if (used[v] == 0) {
			explore(adj, used, v);
			result++;
		}
	}

	return result;
}

int main() {
	size_t n, m;
	std::cin >> n >> m;
	vector<vector<int> > adj(n, vector<int>());
	for (size_t i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1);
	}
	std::cout << number_of_strongly_connected_components(adj);
}
