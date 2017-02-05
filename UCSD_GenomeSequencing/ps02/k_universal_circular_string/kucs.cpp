#include <algorithm>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

// #define DEBUG
#define LOOP(i, m) for(int i = 0; i < m; i++)

typedef vector<vector<int>> vvi;

class Graph {
public:
  // K : k-mer for the De Bruijin Graph
  // NOTE that all edges are simply 0 -> (1 << K)
  // and all vertices are 0 -> (1 << (K - 1))
  Graph(uint32_t K) : m_k(K) {
    m_edges = (1 << m_k);
    m_vertices = m_edges >> 1;
    m_edgeMask = m_edges - 1;
    m_vertexMask = m_vertices - 1;
#ifdef DEBUG
    printf("edge mask = 0x%04x\n", m_edgeMask);
    printf("vertex mask = 0x%04x\n", m_vertexMask);
    cout << "Number of edges = " << m_edges << " and number of vertices = " << m_vertices << endl;
#endif
    m_adjList.resize(m_vertices);
    m_inDegrees.resize(m_vertices);
  };

  // each vertex with pattern [a_0 .. a_n] can only connect to vertices
  // [a_1 .. a_n 0] or [a_1 .. a_n 1]
  void BuildDeBruijnGraph() {
    LOOP(i, m_vertices) {
      uint32_t to_0, to_1;
      to_0 = (i << 1) & m_vertexMask;
      to_1 = (i << 1) & m_vertexMask | 1;
#ifdef DEBUG
      cout << i << " -> " << to_0 << " and " << to_1 << endl;
#endif
      m_adjList[i].push_back(to_0);
      m_adjList[i].push_back(to_1);
      m_inDegrees[to_0]++;
      m_inDegrees[to_1]++;
    }
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
  }

  //
  bool Overlaps_(uint32_t suffix, uint32_t prefix) {
    return false;
  }

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

  uint32_t          m_k;
  uint32_t          m_edgeMask;
  uint32_t          m_vertexMask;
  uint32_t          m_edges;
  uint32_t          m_vertices;
  vvi               m_adjList;
  vector<int>       m_inDegrees;
  int               m_curSccNum;
  vector<int>       m_sccNums;
};

int main(void) {
  uint32_t K;
  cin >> K;
  Graph testGraph(K);
  testGraph.BuildDeBruijnGraph();
  if(testGraph.CheckEulerian()) {
#ifdef DEBUG
    cout << "Eulerian cycle exists" << endl;
#endif

    vector<int> eulerianPath;
    // get the eulerian cycle
    testGraph.GetEulerianCycle(eulerianPath);

    if (eulerianPath.size() == 0) return 2;

#ifdef DEBUG
    // print it out
    for_each(eulerianPath.begin(), eulerianPath.end(), [](int& i){cout << i << " ";});
    cout << endl;
#endif

    string result;
    uint32_t idx = 1 << (K - 1);

    for (int i = idx; idx > 1; idx = idx >> 1) {
      result += (eulerianPath[0] & idx)? "1" : "0";
    }

    for (int i = 1; i < eulerianPath.size(); i++) {
      result += (eulerianPath[i] & 1)? "1" : "0";
    }

    if (eulerianPath[0] == eulerianPath[eulerianPath.size() - 1]) {
      // trim back
      result = result.substr(0, result.size() - (K - 2));
    }

    cout << result << endl;

    return 0;
  }

  return 1;
}
