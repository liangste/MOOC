#include <iostream>
#include <vector>
#include <climits>

using namespace std;

#define DEBUG 1

// This class implements a bit unusual scheme for storing edges of the graph,
// in order to retrieve the backward edge for a given edge quickly
class FlowGraph {
public:
	// we can use 1 instance of FlowGraph to store both original and residual
	// network. But we do need to interpret things differently...
	//
	// for original graph:
	//   concerned with actual flow and capacty only in forward edge
	//
	// for residual graph
	//   forward edge obtained by capacity - flow
	//   backward edge is equal to flow
	struct Edge {
		int from, to, capacity, flow;
	};

private:
	// List of all - forward and backward - edges
	vector<Edge> edges;

	// These adjacency lists store only indices of edges in the edges list
	vector<vector<size_t> > graph;

public:
	explicit FlowGraph(size_t n): graph(n) {}

	void add_edge(int from, int to, int capacity) {
		// Note that we first append a forward edge and then a backward edge,
		// so all forward edges are stored at even indices (starting from 0),
		// whereas backward edges are stored at odd indices in the list edges
		Edge forward_edge = {from, to, capacity, 0};
		Edge backward_edge = {to, from, 0, 0};
		graph[from].push_back(edges.size());
		edges.push_back(forward_edge);
		graph[to].push_back(edges.size());
		edges.push_back(backward_edge);
	}

	size_t size() const {
		return graph.size();
	}

	const vector<size_t>& get_ids(int from) const {
		return graph[from];
	}

	const Edge& get_edge(size_t id) const {
		return edges[id];
	}

	void print_edge(size_t id) {
		Edge edge = get_edge(id);
		cout << "Edge from " << edge.from << "->" << edge.to << ", c=" << edge.capacity << ", flow=" << edge.flow << endl;
	}

	bool dfs_path_helper(int vertex, vector<int>& path, vector<int>& caps) {
		cout << "dfs_path_helper on vertex=" << vertex << endl;
		if (vertex < 0 || vertex >= graph.size()) {
			return false;
		}

		if (vertex == (graph.size() - 1)) {
			path.push_back(vertex);
			return true;
		}

		vector<size_t> ids = get_ids(vertex);
		for (auto id : ids) {
			Edge e = edges[id];
			if (e.capacity > 0) {
				cout << "check to " << e.to << endl;
				if (dfs_path_helper(e.to, path, caps)) {
					path.push_back(vertex);
					caps.push_back(e.capacity);
					return true;
				}
			}
		}

		return false;
	}

	void find_dfs_path(vector<int>& path, vector<int>& caps) {
		// find a path from vertex index 0 to vertex index graph.size() - 1 using
		// Depth-First-Search algorithm. Then return indices of vertexes of this
		// path

		// for each edge from current vertex
		//   if DFS_Helper(edge, path) returns true
		//     add this vertex to path
		//     return true
		dfs_path_helper(0, path, caps);
	}

	void add_flow(size_t id, int flow) {
		// To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
		// due to the described above scheme. On the other hand, when we have to get a "backward"
		// edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
		// should be taken.
		//
		// It turns out that id ^ 1 works for both cases. Think this through!
		edges[id].flow += flow;
		edges[id ^ 1].flow -= flow; // toggles the first bit
	}
};

FlowGraph read_data() {
	int vertex_count, edge_count;
	std::cin >> vertex_count >> edge_count;
	FlowGraph graph(vertex_count);
	for (int i = 0; i < edge_count; ++i) {
		int u, v, capacity;
		std::cin >> u >> v >> capacity;
		graph.add_edge(u - 1, v - 1, capacity);
	}
	return graph;
}



int max_flow(FlowGraph& graph, int from, int to) {
	int flow = 0;

#if DEBUG
	cout << "Printing original graph edges" << endl;
	for (int i = 0; i < graph.size(); i++) {
		vector<size_t> ids = graph.get_ids(i);
		cout << i << "\n";
		for (int j = 0; j < ids.size(); j++) {
			cout << "\t" << ids[j] << " ";
			graph.print_edge(ids[j]);
		}
		cout << endl;
	}
#endif

	// NOTE we can reuse graph as the residual graph
	// f <- 0
	// repeat:
	//   Compuete G_f
	//   Find s-t path P in G_f
	//   if no path: return f
	//   X <- min C_e
	//   g flow with g_e = X for e in P
	//   f <- f + g

	vector<int> path;
	vector<int> caps;
	graph.find_dfs_path(path, caps);
	for (auto v : path) {
		cout << v << " ";
	} cout << endl;

	for (auto c : caps) {
		cout << c << " ";
	} cout << endl;

	if (path.size() == 0) {
		return flow;
	}

	int min_cap = INT_MAX;
	for (auto c : caps) {
		if (c < min_cap) {
			min_cap = c;
		}
	}

	// get X ... minimum capacity alone the path
	cout << "minimum capcacity alone the path is " << min_cap << endl;

	return flow;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	FlowGraph graph = read_data();

	std::cout << max_flow(graph, 0, graph.size() - 1) << "\n";
	return 0;
}
