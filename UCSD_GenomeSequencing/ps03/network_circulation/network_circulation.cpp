#include <climits>
#include <iostream>
#include <vector>

using namespace std;

#define DEBUG

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
	bool dfs_path_helper(int vertex, vector<int>& path, vector<size_t>& edgeIndices) {
    dfs_visited_vertices[vertex] = true;

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
			int residual_flow = 0;
      residual_flow = e.getResidual();
			if (residual_flow > 0) {
        if (!dfs_visited_vertices[e.to]) {
  				if (dfs_path_helper(e.to, path, edgeIndices)) {
  					path.push_back(vertex);
  					edgeIndices.push_back(id);
  					return true;
  				}
        }
			}
		}

		return false;
	}

	void find_dfs_path(vector<int>& path, vector<size_t>& edgeIndices) {
		// find a path from vertex index 0 to vertex index graph.size() - 1 using
		// Depth-First-Search algorithm. Then return indices of vertexes of this
		// path

    dfs_visited_vertices.clear();
    dfs_visited_vertices = vector<bool>(graph.size(), false);
		dfs_path_helper(0, path, edgeIndices);
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

  int iteration = 0;
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

		graph.find_dfs_path(path, edges);

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
    iteration++;
	}

	return flow;
}

struct BoundedEdge {
  int from, to, capacity, flow;

  // calculate residual capacity
  int getResidual() const {return capacity - flow;};
};

int main(void) {
  int n_vertices, n_edges;
  cin >> n_vertices >> n_edges;
  FlowGraph fg(n_vertices + 2);
  vector<vector<int>> adj_list;
  vector<BoundedEdge> edges;
  vector<int> outgoing_flow, incoming_flow;
  int master_source, master_sink;

  int u, v, l, c;
  int m = n_edges;

  adj_list.resize(n_vertices);
  outgoing_flow.resize(n_vertices);
  incoming_flow.resize(n_vertices);
  master_source = n_vertices;
  master_sink = n_vertices + 1;

  while(m--) {
    cin >> u >> v >> l >> c;
    BoundedEdge e;
    e.from = u - 1;
    e.to = v - 1;
    e.flow = l;
    e.capacity = c;
    adj_list[u - 1].push_back(edges.size());
    edges.push_back(e);

    outgoing_flow[e.from] += e.flow;
    incoming_flow[e.to] += e.flow;
  }

#ifdef DEBUG
  cout << "showing total incoming and outgoing flows" << endl;
  for (int i = 0; i < n_vertices; i++) {
    cout << i << ":" << " in[" << incoming_flow[i] << "], out[" << outgoing_flow[i] << "]" << endl;
  }
#endif

  // add edges to flowgraph
  for (int i = 0; i < adj_list.size(); i++) {
    for (int j = 0; j < adj_list[i].size(); j++) {
      int e = adj_list[i][j];
      fg.add_edge(edges[e].from, edges[e].to, edges[e].capacity - edges[e].flow);
    }
  }

  int source_total, sink_total;
  for (int i = 0; i < n_vertices; i++) {
    if (incoming_flow[i] > outgoing_flow[i]) { // source
      source_total += (incoming_flow[i] - outgoing_flow[i]);
    } else if (incoming_flow[i] < outgoing_flow[i]) { // sink
      sink_total += (outgoing_flow[i] - incoming_flow[i]);
    }
  }

  for (int i = 0; i < n_vertices; i++) {
    if (incoming_flow[i] > outgoing_flow[i]) { // source
      fg.add_edge(master_source, i, source_total);
    } else if (incoming_flow[i] < outgoing_flow[i]) { // sink
      fg.add_edge(i, master_sink, sink_total);
    }
  }

  std::cout << max_flow(fg, master_source, master_sink) << "\n";

  for (int i = 0; i < n_vertices; i++) {
    vector<size_t> ids = fg.get_ids(i);
    for (auto& id : ids) {
      fg.print_edge(id);
    }
  }

  return 0;
}
