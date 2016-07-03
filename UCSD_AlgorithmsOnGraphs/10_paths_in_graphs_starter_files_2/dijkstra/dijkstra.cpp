#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>

using namespace std;

struct Vertex {
	int distance;
	int id;

	Vertex(int distance, int id) : distance(distance), id(id) {};

	bool operator<(const Vertex& v) const {
		return this->distance < v.distance;
	};

	bool operator>(const Vertex& v) const {
		return this->distance > v.distance;
	};

	bool operator<=(const Vertex& v) const {
		return this->distance <= v.distance;
	};

	bool operator>=(const Vertex& v) const {
		return this->distance >= v.distance;
	};
};

int distance(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, int t) {
	std::vector<int> distances(adj.size()); // shortest distances for all nodes
	std::vector<bool> visited(adj.size());
	priority_queue<Vertex, vector<Vertex>, greater<Vertex> > pqueue;

	for (int i = 0; i < adj.size(); i++) {
		distances[i] = INT_MAX/2;
		visited[i] = false;
	}
	distances[s] = 0;

	for (int i = 0; i < adj.size(); i++) {
		pqueue.push(Vertex(distances[i], i));
	}

	while(!pqueue.empty()) {
		Vertex cur = pqueue.top();
		pqueue.pop();

		if (visited[cur.id]) {
			continue;
		}

		//cout << "cur id = " << cur.id << " distance = " << cur.distance << endl;

		for (int i = 0; i < adj[cur.id].size(); i++) {
			if (distances[adj[cur.id][i]] > (distances[cur.id] + cost[cur.id][i])) {
				distances[adj[cur.id][i]] = distances[cur.id] + cost[cur.id][i];
				pqueue.push(Vertex(distances[adj[cur.id][i]], adj[cur.id][i]));
			}
		}

		visited[cur.id] = true;
	}

	if (INT_MAX/2 == distances[t])
		return -1;
	return distances[t];
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
	int s, t;
	std::cin >> s >> t;
	s--, t--;
	std::cout << distance(adj, cost, s, t);
}
