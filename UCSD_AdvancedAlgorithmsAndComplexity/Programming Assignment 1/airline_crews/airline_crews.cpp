#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <climits>

using namespace std;

#define DEBUG 0

class FlowGraph {
public:
	struct Edge {
		int from, to, capacity, flow;
		int getResidual() const {return capacity - flow;};
	};

private:
	vector<Edge> edges;
	vector<vector<size_t> > graph;

public:
	explicit FlowGraph(size_t n): graph(n) {}

	void add_edge(int from, int to, int capacity) {
		Edge forward_edge = {from, to, capacity, 0};
		Edge backward_edge = {to, from, 0, 0};
		graph[from].push_back(edges.size());
		edges.push_back(forward_edge);
		graph[to].push_back(edges.size());
		edges.push_back(backward_edge);
	}

	size_t size() const {return graph.size();}

	const vector<size_t>& get_ids(int from) const {return graph[from];}

	const Edge& get_edge(size_t id) const {return edges[id];}

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
#if DEBUG
            cout << vertex << " -> " << e.to << endl;
#endif
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
    dfs_visited_vertices.clear();
    path.clear();
    edgeIndices.clear();
    dfs_visited_vertices = vector<bool>(graph.size(), false);
		dfs_path_helper(0, path, edgeIndices);
	}

	void add_flow(size_t id, int flow) {
		edges[id].flow += flow;
		edges[id ^ 1].flow -= flow; // toggles the first bit
	}
};


class MaxMatching {
public:
  void Solve() {
    vector<vector<bool>> adj_matrix = ReadData();
    vector<int> matching = FindMatching(adj_matrix);
    WriteResponse(matching);
  }

private:
  vector<vector<bool>> ReadData() {
    int num_left, num_right;
    cin >> num_left >> num_right;
    vector<vector<bool>> adj_matrix(num_left, vector<bool>(num_right));
    for (int i = 0; i < num_left; ++i)
      for (int j = 0; j < num_right; ++j) {
        int bit;
        cin >> bit;
        adj_matrix[i][j] = (bit == 1);
      }
    return adj_matrix;
  }

  void WriteResponse(const vector<int>& matching) {
    for (int i = 0; i < matching.size(); ++i) {
      if (i > 0)
        cout << " ";
      if (matching[i] == -1)
        cout << "-1";
      else
        cout << (matching[i] + 1);
    }
    cout << "\n";
  }

  vector<int> FindMatching(const vector<vector<bool>>& adj_matrix) {
    // Replace this code with an algorithm that finds the maximum
    // matching correctly in all cases.
    int num_left = adj_matrix.size();
    int num_right = adj_matrix[0].size();
    vector<int> matching(num_left, -1);

    FlowGraph graph(2 + num_left + num_right);
    // source -> num_left -> num_right -> sink
    // source index = 0
    // left indices, left[i] = index 1 + i
    // right indices, right[j] = index 1 + num_left + j
    // sink index = 1 + num_left + num_right

    // add edges from sink to all vertices on the left
    for (int i = 0; i < num_left; i++) {
      graph.add_edge(0, 1 + i, 1);
    }

    // add edges from all left vertices to right vertices
    for (int i = 0; i < num_left; i++) {
      for (int j = 0; j < num_right; j++) {
        if (adj_matrix[i][j]) {
          graph.add_edge(1 + i, 1 + num_left + j, 1);
        }
      }
    }

    int sink_index = 1 + num_left + num_right;
    // add edges from right vertices to sink
    for (int i = 0; i < num_right; i++) {
      graph.add_edge(1 + num_left + i, sink_index, 1);
    }

#if DEBUG
    cout << "Finished building a network for bipartite matching" << endl;
#endif

    int iteration = 0;
    vector<int> path;
    vector<size_t> edges;
    while(true) {
#if DEBUG
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

      //if (iteration >= 3) break;

      graph.find_dfs_path(path, edges);

  		if (path.size() == 0) {
  			break;
  		}

  		int min_cap = INT_MAX;
  		for (auto eid : edges) {
  			if (graph.get_edge(eid).getResidual() < min_cap) {
  				min_cap = graph.get_edge(eid).getResidual();
  			}
  		}

  		for (auto e : edges) {graph.add_flow(e, min_cap);}

      iteration++;
    }

#if DEBUG
    cout << "Finished solving for maxflow on bipartite network" << endl;
#endif

    // add edges from sink to all vertices on the left
    for (int i = 0; i < num_left; i++) {
      graph.add_edge(0, 1 + i, 1);
      const vector<size_t>& edgeIds = graph.get_ids(1 + i);
      for (auto& eid : edgeIds) {
        const FlowGraph::Edge& edge = graph.get_edge(eid);
        if (edge.flow == 1 && edge.capacity == 1) {
          matching[i] = edge.to - num_left - 1;
        }
      }
    }

    // 3 - get back match info
    return matching;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  MaxMatching max_matching;
  max_matching.Solve();
  return 0;
}
