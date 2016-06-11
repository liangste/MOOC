#include <iostream>
#include <vector>
#include <algorithm>

class TreeHeight {
	int n;
	std::vector<int> parent;
	std::vector<int> heights;
	std::vector<bool> visited;

public:
	void read() {
		std::cin >> n;
		parent.resize(n);
		heights.resize(n);
		visited.resize(n);
		for (int i = 0; i < n; i++) {
			std::cin >> parent[i];
			heights[i] = 0;
			visited[i] = false;
		}
	}

	int get_height(int n) {
		if (visited[n]) {
			return heights[n];
		}

		if (parent[n] == -1) {
			// root is height 1
			heights[n] = 1;
		} else {
			heights[n] = get_height(parent[n]) + 1;
		}
		visited[n] = true;
		return heights[n];
	}

	int compute_height() {
		// Replace this code with a faster implementation
		int maxHeight = 0;
		int height;
		for (int vertex = 0; vertex < n; vertex++) {
			height = 0;
			if (!visited[vertex]) {
				height = get_height(vertex);
			}
			maxHeight = std::max(maxHeight, height);
		}
		return maxHeight;
	}
};

int main() {
	std::ios_base::sync_with_stdio(0);
	TreeHeight tree;
	tree.read();
	std::cout << tree.compute_height() << std::endl;
}
