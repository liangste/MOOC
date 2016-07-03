#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using namespace std;

const int imax = numeric_limits<int>::max();
const long long llmax = numeric_limits<long long>::max();

void shortest_paths(vector<vector<int> > &adj,
										vector<vector<int> > &cost,
										int s,
										vector<long long> &distance,
										vector<int> &reachable,
										vector<int> &shortest) {

	vector<long long> last_distance ;
	queue<int> negative_cycle_vertices;

	distance[s] = 0; // let node 0 be source

	// run for |V| - 1 times
	for (int i = 1; i < adj.size(); i++) {
		// for all edges
		for (int u = 0; u < adj.size(); u++) {
			for (int v = 0; v < adj[u].size(); v++) {
				int uu = u;
				int vv = adj[uu][v];

				if (distance[uu] == llmax)
					continue;

				long long cur_sum = (long long) distance[uu] + (long long) cost[u][v];

				if (cur_sum < distance[vv]) {
					distance[vv] = cur_sum;
				}
			}
		}
	}

	last_distance = distance;

	// for all edges
	for (int u = 0; u < adj.size(); u++) {
		for (int v = 0; v < adj[u].size(); v++) {
			int uu = u;
			int vv = adj[uu][v];

			if (last_distance[uu] == llmax)
				continue;

			long long cur_sum = (long long) last_distance[uu] + (long long) cost[u][v];

			if (cur_sum < last_distance[vv]) {
				last_distance[vv] = cur_sum;
			}
		}
	}

	for (int i = 0; i < distance.size(); i++) {
		if (distance[i] != last_distance[i]) {
			negative_cycle_vertices.push(i);
		}
	}

	vector<bool> visited(adj.size(), false);
	while(negative_cycle_vertices.size() > 0) {
		int v = negative_cycle_vertices.front();
		negative_cycle_vertices.pop();

		for (int i = 0; i < adj[v].size(); i++) {
			int target = adj[v][i];

			if (!visited[target]) {
				negative_cycle_vertices.push(target);
			}
		}

		visited[v] = true;
	}

	for (int i = 0; i < distance.size(); i++) {
		//cout << "dist[" << i << "] = " << dist[i] << endl;
		if (distance[i] != llmax) {
			// reachable
			reachable[i] = 1;

			if (visited[i]) {
				shortest[i] = 0;
			} else if (last_distance[i] < distance[i]) {
				shortest[i] = 0; // no shortest path
			}
		}
	}
}

int main() {
	int n, m, s;
	std::cin >> n >> m;
	vector<vector<int> > adj(n, vector<int>());
	vector<vector<int> > cost(n, vector<int>());
	for (int i = 0; i < m; i++) {
		int x, y, w;
		std::cin >> x >> y >> w;
		adj[x - 1].push_back(y - 1);
		cost[x - 1].push_back(w);
	}
	std::cin >> s;
	s--;
	vector<long long> distance(n, std::numeric_limits<long long>::max());
	vector<int> reachable(n, 0);
	vector<int> shortest(n, 1);
	shortest_paths(adj, cost, s, distance, reachable, shortest);
	for (int i = 0; i < n; i++) {
		if (!reachable[i]) {
			std::cout << "*\n";
		} else if (!shortest[i]) {
			std::cout << "-\n";
		} else {
			std::cout << distance[i] << "\n";
		}
	}
}
