#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cfloat>
#include <cmath>
#include <queue>

using namespace std;

double GetDistance(int x1, int x2, int y1, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

struct Edge {
	int pt0_;
	int pt1_;
	double dist_;

	Edge(int pt0, int pt1, double dist)
		: pt0_(pt0), pt1_(pt1), dist_(dist) {};

	bool operator<(const Edge& other) const {
		return this->dist_ < other.dist_;
	};

	bool operator>(const Edge& other) const {
		return this->dist_ > other.dist_;
	};

	bool operator==(const Edge& other) const {
		return this->dist_ == other.dist_;
	};

	bool operator>=(const Edge& other) const {
		return *this == other || *this > other;
	};

	bool operator<=(const Edge& other) const {
		return *this == other || *this < other;
	}
};

struct DisjointSetsElement {
	// size is how many elements in total are pointing to this element
	// parent is self or other elements
	// rank heigt of this element
	int size, parent, rank;

	DisjointSetsElement(int size = 0, int parent = -1, int rank = 0):
			size(size), parent(parent), rank(rank) {}
};

struct DisjointSets {
	int size;
	int max_table_size;
	vector <DisjointSetsElement> sets;

	DisjointSets(int size): size(size), max_table_size(0), sets(size) {
		for (int i = 0; i < size; i++) {
			sets[i].size = 1;
			sets[i].parent = i;
			sets[i].rank = 0;
		}
	}

	// return number of distinct sets
	int CountSets() {
		return size;
	}

	bool SameSet(int i, int j) {
		return GetParent(i) == GetParent(j);
	}

	int GetParent(int table) {
		// find parent and compress path
		if (sets[table].parent != table)
			sets[table].parent = GetParent(sets[table].parent);
		return sets[table].parent;
	}

	void Merge(int destination, int source) {
		int d = GetParent(destination);
		int s = GetParent(source);

		if (d == s)
			return;
		size--;
		if (sets[d].rank > sets[s].rank) {
			sets[s].parent = d;
			sets[d].size += sets[s].size;
		} else {
			sets[d].parent = s;
			sets[s].size += sets[d].size;
			if (sets[s].rank == sets[d].rank) {
				sets[s].rank++;
			}
		}
	}
};

// look up points by index
double clustering(vector<int> x, vector<int> y, int k) {
	int num_points = x.size();
	double min_set_distance = DBL_MAX;
	DisjointSets sets(num_points);
	priority_queue<Edge, vector<Edge>, greater<Edge> > pQ;
	//write your code here
	//
	//make V * V edges

	for (int i = 0; i < num_points; i++) {
		for (int j = 0; j < num_points; j++) {
			pQ.push(Edge(i, j, GetDistance(x[i], x[j], y[i], y[j])));
		}
	}
	//
	//sort those edges by weight'
	//merge sets until only k sets left

	while (sets.CountSets() > k) {
		Edge e = pQ.top(); pQ.pop();
		//cout << "examining edge with pt0 = " << e.pt0_ << " and pt1 = " << e.pt0_ << ", distance = " << e.dist_ << endl;
		sets.Merge(e.pt0_, e.pt1_);
	}

	// now we have k distinct sets
	//cout << "now we have " << sets.CountSets() << " sets and " << pQ.size() << " edges in queue left" << endl;

	while (!pQ.empty()) {
		Edge e = pQ.top(); pQ.pop();
		if (!sets.SameSet(e.pt0_, e.pt1_)) {
			double cur_distance = GetDistance(x[e.pt0_], x[e.pt1_], y[e.pt0_], y[e.pt1_]);
			if (cur_distance < min_set_distance) {
				min_set_distance = cur_distance;
			}
		}
	}
	//
	//NOTE need to trim edges too
	//
	//calculate min distance between clusters
	return min_set_distance;
}

int main() {
	size_t n;
	int k;
	std::cin >> n;
	vector<int> x(n), y(n);
	for (size_t i = 0; i < n; i++) {
		std::cin >> x[i] >> y[i];
	}
	std::cin >> k;
	std::cout << std::setprecision(10) << clustering(x, y, k) << std::endl;
}
