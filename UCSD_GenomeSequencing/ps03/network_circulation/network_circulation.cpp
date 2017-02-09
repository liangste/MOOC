#include <climits>
#include <iostream>
#include <vector>

using namespace std;

#define DEBUG
#define LOOP(i, m) for(int i = 0; i < m; i++)

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

		// calculate residual capacity
		int getResidual() const {return capacity - flow;};
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


  vector<bool> dfs_visited_vertices;
	bool dfs_path_helper(int vertex, int target, vector<int>& path, vector<size_t>& edgeIndices) {
    dfs_visited_vertices[vertex] = true;

		if (vertex < 0 || vertex >= graph.size()) {
			return false;
		}

		if (vertex == target) {
			path.push_back(vertex);
			return true;
		}

		vector<size_t> ids = get_ids(vertex);
		for (auto id : ids) {
			Edge e = edges[id];
			int residual_flow = 0;
      residual_flow = e.getResidual();
			if (residual_flow > 0) {
        if (!dfs_visited_vertices[e.to]) {
  				if (dfs_path_helper(e.to, target, path, edgeIndices)) {
  					path.push_back(vertex);
  					edgeIndices.push_back(id);
  					return true;
  				}
        }
			}
		}

		return false;
	}

	void find_dfs_path(vector<int>& path, vector<size_t>& edgeIndices, int start, int end) {
		// find a path from vertex index 0 to vertex index graph.size() - 1 using
		// Depth-First-Search algorithm. Then return indices of vertexes of this
		// path

    dfs_visited_vertices.clear();
    dfs_visited_vertices = vector<bool>(graph.size(), false);
		dfs_path_helper(start, end, path, edgeIndices);
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

int max_flow(FlowGraph& graph, int from, int to) {
	int flow = 0;

	// NOTE we can reuse graph as the residual graph
	// f <- 0
	// repeat:
	//   Compuete G_f
	//   Find s-t path P in G_f
	//   if no path: return f
	//   X <- min C_e
	//   g flow with g_e = X for e in P
	//   f <- f + g

	while(true) {
		vector<int> path;
		vector<size_t> edges;

#ifdef DEBUG
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

		graph.find_dfs_path(path, edges, from, to);

		if (path.size() == 0) {
			return flow;
		}

		int min_cap = INT_MAX;
		for (auto eid : edges) {
			if (graph.get_edge(eid).getResidual() < min_cap) {
				min_cap = graph.get_edge(eid).getResidual();
			}
		}

		// get X ... minimum capacity alone the path

		for (auto e : edges) {
			graph.add_flow(e, min_cap);
		}

		flow += min_cap;
	}

	return flow;
}

struct RawBoundedEdge {
  int u, v, l, c;
};

void PrintVectorInt(vector<int>& v) {
	for (auto& v_ : v) {
		cout << v_ << " ";
	}
	cout << endl;
}

int main(void) {

	int n_vertices, n_edges;
	cin >> n_vertices >> n_edges;
	vector<RawBoundedEdge> raw_edges;
	vector<int> source_vertices;
	vector<int> sink_vertices;

	LOOP(i, n_edges) {
		RawBoundedEdge e;
		cin >> e.u >> e.v >> e.l >> e.c;
		e.u--;
		e.v--;
		raw_edges.push_back(e);
	}

	vector<int> t_out(n_vertices, 0);
	vector<int> t_in(n_vertices, 0);

	// find source and sink vertices
	for (auto& e : raw_edges) {
		t_out[e.u] += e.l;
		t_in[e.v] += e.l;
	}

	LOOP(i, n_vertices) {
		if (t_out[i] == t_in[i]) continue;
		if (t_out[i] > t_in[i]) {
			sink_vertices.push_back(i);
		} else {
			source_vertices.push_back(i);
		}
	}

#ifdef DEBUG
	cout << "source vertices" << endl;
	PrintVectorInt(source_vertices);
	cout << "sink vertices" << endl;
	PrintVectorInt(sink_vertices);
#endif

	// translate to max-flow problem
	int global_src = n_vertices;
	int global_snk = n_vertices + 1;
	FlowGraph fg(n_vertices + 2); // add global source and global sink
	// find source and sink vertices
	for (auto& e : raw_edges) {
		fg.add_edge(e.u, e.v, e.c - e.l);
	}

	for (auto& sc : source_vertices) {
		fg.add_edge(global_src, sc, INT_MAX);
	}

	for (auto& snk : sink_vertices) {
		fg.add_edge(snk, global_snk, INT_MAX);
	}

	cout << "max flow = " << max_flow(fg, global_src, global_snk) << endl;

	for (auto& re : raw_edges) {
		vector<size_t> edge_ids = fg.get_ids(re.u);
		for (auto& id : edge_ids) {
			if (0 == id % 2) {
				FlowGraph::Edge fg_edge = fg.get_edge(id);
				cout << "added flow " << fg_edge.from << "->" << fg_edge.to << ", " << fg_edge.flow << endl;
			}
		}
	}

  return 0;
}
