#include <algorithm>
#include <iostream>
#include <map> // for multimap
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

//#define DEBUG
#define LOOP(i, m) for(int i = 0; i < m; i++)

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

class Graph {
public:
  Graph() : m_vertices(0) {};

  void AddRead(const string& r) {
    // prefix of size k - 1, since all reads form the k-mer composition
    string pre = r.substr(0, r.size() - 1);
    string suf = r.substr(1);

    m_prefixMap.insert({pre, m_vertices});

    if (m_verticesMap.find(pre) == m_verticesMap.end()) {
      m_vertexStrings.push_back(pre);
      m_verticesMap[pre] = m_vertices++;
    }

    if (m_verticesMap.find(suf) == m_verticesMap.end()) {
      m_vertexStrings.push_back(suf);
      m_verticesMap[suf] = m_vertices++;
    }

    m_reads.push_back(r);
  }

  void BuildDeBruijnGraph() {
    m_edges = m_reads.size();

#ifdef DEBUG
    cout << "Number of edges = " << m_edges << " and number of vertices = " << m_vertices << endl;
    LOOP(i, m_vertices) {
      cout << i << " : " << m_vertexStrings[i] << endl;
    }
#endif
    m_adjList.resize(m_vertices);
    m_inDegrees.resize(m_vertices);

    LOOP(i, m_edges) {
#ifdef DEBUG
      cout << "read " << m_reads[i] << endl;
#endif
      string pre = m_reads[i].substr(0, m_reads[i].size() - 1);
      string suf = m_reads[i].substr(1);
      int to_vertex_index = m_verticesMap[suf];
      int from_vertex_index = m_verticesMap[pre];

#ifdef DEBUG
      cout << "  " << from_vertex_index << " -> " << to_vertex_index << endl;
#endif
      m_adjList[from_vertex_index].push_back(to_vertex_index);
      m_inDegrees[to_vertex_index]++;
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

  void PrintPath(vi& path) {
    if (!path.size()) return;

    int first = path[0];
    int last = path[path.size() - 1];

    string result = m_vertexStrings[first];

    for (int i = 1; i < path.size(); i++) {
      result += m_vertexStrings[path[i]].back();
    }

    // trim back
    result = result.substr(0, result.size() - (m_reads[first].size() - 2));

    cout << result << endl;
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

  uint32_t              m_k;
  uint32_t              m_edges;
  uint32_t              m_vertices;
  vvi                   m_adjList;
  vector<int>           m_inDegrees;
  int                   m_curSccNum;
  vector<int>           m_sccNums;
  vector<string>        m_reads;
  vector<string>        m_vertexStrings;
  unordered_map<string, int>  m_verticesMap; // vertex value to index
  multimap<string, int> m_prefixMap;
};

int main(void) {
  string read;

  Graph testGraph;
  while (cin >> read) {
    testGraph.AddRead(read);
  }

  testGraph.BuildDeBruijnGraph();;
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

    testGraph.PrintPath(eulerianPath);

    return 0;
  }

  return 1;
}
