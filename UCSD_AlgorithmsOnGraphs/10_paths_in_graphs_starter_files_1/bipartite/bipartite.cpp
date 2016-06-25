#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef enum Color {
	Unpainted = 0,
	Black = 1,
	White = 2
} Color_t;

// return true if violates bipartite properties
bool bfs(vector<vector<int> > &adj, int s, vector<int>& colors) {
	queue<int> discoverQueue;
	discoverQueue.push(s);

	int cur, next;
	while (discoverQueue.size() > 0) {
		cur = discoverQueue.front();
		discoverQueue.pop();
		for (int i = 0; i < adj[cur].size(); i++) {
			next = adj[cur][i];
			if (colors[next] == Unpainted) {
				// paint opposite color
				if (colors[cur] == Black)
					colors[next] = White;
				else colors[next] = Black;
				discoverQueue.push(next);
			} else {
				if (colors[cur] == colors[next])
					return true;
			}
		}
	}

	return false;
}

int bipartite(vector<vector<int> > &adj) {
	//write your code here
	vector<int> Colors;

	Colors.resize(adj.size());
	for (int i = 0; i < Colors.size(); i++) {
		Colors[i] = Unpainted;
	}

	for (int i = 0; i < Colors.size(); i++) {
		if (Colors[i] == Unpainted) {
			if (bfs(adj, i, Colors))
				return 0;
		}
	}

	return 1;
}

int main() {
	int n, m;
	std::cin >> n >> m;
	vector<vector<int> > adj(n, vector<int>());
	for (int i = 0; i < m; i++) {
		int x, y;
		std::cin >> x >> y;
		adj[x - 1].push_back(y - 1);
		adj[y - 1].push_back(x - 1);
	}
	std::cout << bipartite(adj);
}
