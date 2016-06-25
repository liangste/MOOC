#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int distance(vector<vector<int> > &adj, int s, int t) {
	//write your code here
	vector<int> discoverLevels;
	queue<int> discoverQueue;

	discoverLevels.resize(adj.size());
	for (int i = 0; i < discoverLevels.size(); i++) {
		discoverLevels[i] = 0;
	}

	discoverQueue.push(s);

	int cur, next;
	while (discoverQueue.size() > 0) {
		cur = discoverQueue.front();
		discoverQueue.pop();
		for (int i = 0; i < adj[cur].size(); i++) {
			next = adj[cur][i];
			if (0 == discoverLevels[next]) {
				discoverLevels[next] = discoverLevels[cur] + 1;
				discoverQueue.push(next);
			}
		}
	}

	if (discoverLevels[t] == 0)
	 return -1;
	else return discoverLevels[t];
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
	int s, t;
	std::cin >> s >> t;
	s--, t--;
	std::cout << distance(adj, s, t);
}
