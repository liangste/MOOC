#include <iostream>
#include <queue>
#include <vector>

using std::queue;
using std::vector;
using std::pair;

// return true of cyclic
bool Explore(size_t vertex, vector<vector<int> >& adj, int visited_levels[]) {
	int num_nbrs = adj[vertex].size();

	if (visited_levels[vertex] == 1)
		return true;
	else {
			visited_levels[vertex] = 1;
			for (size_t n = 0; n < num_nbrs; ++n) {
				if(Explore(adj[vertex][n], adj, visited_levels))
					return true;
			}
			visited_levels[vertex] = 2;
	}
	return false;
}

int acyclic(vector<vector<int> > &adj) {
	size_t num_vertices = adj.size();
	int visited_levels[num_vertices];

	for (size_t i = 0; i < num_vertices; ++i) {
		visited_levels[i] = 0;
	}

	for (size_t i = 0; i < num_vertices; ++i) {
		if (visited_levels[i] == 0) {
			if (Explore(i, adj, visited_levels))
				return 1;
		}
	}

	return 0;
}

int main() {
	size_t vertices, edges;
	std::cin >> vertices >> edges;

	vector<vector<int> > adj(vertices, vector<int>());

	for (size_t i = 0; i < edges; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1); // x pointing to y
	}
	std::cout << acyclic(adj);
}
