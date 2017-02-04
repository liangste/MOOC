// refs : http://www.graph-magics.com/articles/euler.php

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <vector>

//#define DEBUG1
#define LOOP(x, m) for(int x = 0; x < m; x++)

using namespace std;

class Graph {
public:
  // n : number of vertices, vertices are 1-indexed
  Graph(int n)
    : m_vertices(n)
    , m_adjList(n)
    , m_inDegrees(n) {
  }

  void AddEdge(int f, int t) {
    m_adjList[f - 1].push_back(t - 1);
    m_inDegrees[t - 1]++;
  }

  // check for Eulerian cycle conditions
  bool CheckEulerian() {
    // all vertices with non-zero degree belong to the same SCC
    GetSCCs_(m_adjList);
    int targetScc = -1;
    LOOP(i, m_vertices) {
      // if in-degree != out-degree, not an Eulerian cycle
      if (m_adjList[i].size() != m_inDegrees[i]) return false;
      if (m_adjList[i].size() > 0) {
        if (targetScc == -1) targetScc = m_sccNums[i];
        else if (targetScc != m_sccNums[i]) return false;
      }
    }
    return true;
  }

  void GetEulerianCycle(vector<int>& circuit) {
    // edge_count represents the number of edges
    // emerging from a vertex
    unordered_map<int,int> edge_count;
    stack<int> curr_path;
    circuit.clear();

    if (!m_vertices)
        return; //empty graph

    LOOP(i, m_vertices)
        edge_count[i] = m_adjList[i].size();

    // start from any vertex
    curr_path.push(0);
    int curr_v = 0; // Current vertex

    while (!curr_path.empty())
    {
        // If there's remaining edge
        if (edge_count[curr_v])
        {
            // Push the vertex
            curr_path.push(curr_v);

            // Find the next vertex using an edge
            int next_v = m_adjList[curr_v].back();

            // and remove that edge
            edge_count[curr_v]--;
            m_adjList[curr_v].pop_back();

            // Move to next vertex
            curr_v = next_v;
        } else {
          // back-track to find remaining circuit
          circuit.push_back(curr_v);

          // Back-tracking
          curr_v = curr_path.top();
          curr_path.pop();
        }
    }

    // we've got the cycle in reverse
    reverse(circuit.begin(), circuit.end());
    circuit.pop_back();
    for_each(circuit.begin(), circuit.end(), [](int& i) {i++;});
  }

private:
  void Explore_(vector<vector<int>>& adj, vector<int>& used, int x) {
  	used[x] = 1;
    m_sccNums[x] = m_curSccNum;
    LOOP(i, adj[x].size())
  		if (used[adj[x][i]] == 0) // not visited
  			Explore_(adj, used, adj[x][i]);
  }

  void DFS_(vector<vector<int>>& adj, vector<int>& used, vector<int>& order, int x) {
  	used[x] = 1;
    LOOP(i, adj[x].size())
  		if (used[adj[x][i]] == 0) // not visited
  			DFS_(adj, used, order, adj[x][i]);
  	order.push_back(x);
  }

  void GetSCCs_(vector<vector<int>>& adj) {
  	// create reverse graph
  	vector<vector<int> > adj_R(m_vertices, vector<int>());
  	vector<int> used(m_vertices, 0);
  	vector<int> postOrder;

    LOOP(i, m_vertices)
      LOOP(j, adj[i].size())
  			adj_R[adj[i][j]].push_back(i);

    LOOP(i, m_vertices)
  		if (used[i] == 0)
  			DFS_(adj_R, used, postOrder, i);

  	std::reverse(postOrder.begin(), postOrder.end());

    fill(used.begin(), used.end(), 0);

    m_curSccNum = 0;
    m_sccNums.resize(m_vertices);
    LOOP(i, postOrder.size()) {
  		int v = postOrder[i];
  		if (used[v] == 0) {
  			Explore_(adj, used, v);
        m_curSccNum++;
  	  }
    }

#ifdef DEBUG1
    cout << "Printing SCC Numbers : ";
    LOOP(i, m_vertices)
      cout << m_sccNums[i] << " ";
    cout << endl;
#endif
  }

  int                       m_vertices;
  vector<vector<int>>       m_adjList;
  vector<int>               m_inDegrees;
  int                       m_curSccNum;
  vector<int>               m_sccNums;
};

int main(void) {
  int V, E;
  cin >> V >> E;
  Graph testGraph(V);
  int f, t;
  while (E--) {
    cin >> f >> t;
    testGraph.AddEdge(f, t);
  }

  if (testGraph.CheckEulerian()) {
    cout << "1" << endl;
    vector<int> eulerianPath;

    // get the eulerian cycle
    testGraph.GetEulerianCycle(eulerianPath);

    // print it out
    for_each(eulerianPath.begin(), eulerianPath.end(), [](int& i){cout << i << " ";});
    cout << endl;
  } else {
    cout << "0" << endl;
  }

  return 0;
}
