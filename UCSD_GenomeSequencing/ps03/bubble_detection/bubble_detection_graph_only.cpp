// bubble_detection.cpp with De Bruijn graph replaced with simple DAG
//
// first line of input should be 3 positive integers V, E and T denoting the number
// of vertices, the number of edges and bubble threshold
// the following E lines contain integers FROM and TO denoting a directed edge
// from vertex FROM to vertex TO. Vertices should be 0-indexed

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

//#define DEBUG
#define LOOP(i, m) for(int i = 0; i < m; i++)

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

void PrintVectorInt(vi& v) {
  for (auto& elm : v)
    cout << elm << " ";
  cout << endl;
}

class BubbleGraph {
public:
  // k-mer size and threshold t
  BubbleGraph(int v, int e, int t) : v_(v), e_(e), t_(t) {
      adjList_.resize(v);
  }

  void AddEdge(int u, int v) {
      adjList_[u].push_back(v);
  }

  int CountBubbles() {
    int cnt = 0;
    LOOP(i, adjList_.size())
      if (adjList_[i].size() >= 2)
        cnt += CountBubblesFromSource(i, t_);
    return cnt;
  }

private:
  void GetNonOverLappingPaths(vi& path, set<int>& visited, vvi& allPaths, vector<set<int>>& allSets, int l) {
    if (path.size() == l) {
      allPaths.push_back(path);
      allSets.push_back(visited);
      return;
    }

    int s = path.back();

    // nore more, push back and return
    if (adjList_[s].size() == 0) {
        allPaths.push_back(path);
        allSets.push_back(visited);
        return;
    }

    LOOP(i, adjList_[s].size()) {
      int v = adjList_[s][i];

      if (visited.find(v) == visited.end()) {
        visited.insert(v);
        path.push_back(v);
        GetNonOverLappingPaths(path, visited, allPaths, allSets, l);
        path.pop_back();
        visited.erase(v);
      } else {
        // we're visiting a vertex we've previously visited before
        continue;
      }
    }
  }

  // this function fails with test case 4 -> 7
  int CountBubblesFromLeftRightPaths(vector<set<int>>& leftSet, vvi& rightPaths) {
      set<int> globalLeftSet;
      set<int> mergeVertices;

      for (auto& s : leftSet) {
          for (auto& p : rightPaths) {
              for (auto& v : p) {
                  if (s.find(v) != s.end()) {
                      mergeVertices.insert(v);
                      break; // end current path
                  }
              }
          }
      }

      return mergeVertices.size();
  }

  int CountBubblesFromSource(int s, int t) {
    int count = 0;
    vi path;
    vvi leftPaths, rightPaths;
    vector<set<int>> leftSet, rightSet;
    set<int> visited;
    int nPath = adjList_[s].size();

    for (int i = 0; i < (nPath - 1); i++) {
        for (int j = i + 1; j < nPath; j++) {
            int bbl = 0;
#ifdef DEBUG
            cout << "src " << s << " left " << i << ", right " << j << endl;
#endif
            path.clear();
            leftPaths.clear();
            visited.clear();
            leftSet.clear();
            path.push_back(adjList_[s][i]);
            visited.insert(adjList_[s][i]);
            GetNonOverLappingPaths(path, visited, leftPaths, leftSet, t);

#ifdef DEBUG
            cout << "Left paths" << endl;
            for (auto& v : leftPaths) {
                PrintVectorInt(v);
            }
#endif

            path.clear();
            rightPaths.clear();
            visited.clear();
            rightSet.clear();
            path.push_back(adjList_[s][j]);
            visited.insert(adjList_[s][j]);
            GetNonOverLappingPaths(path, visited, rightPaths, rightSet, t);

#ifdef DEBUG
            cout << "Right paths" << endl;
            for (auto& v : rightPaths) {
                PrintVectorInt(v);
            }
#endif
            bbl = CountBubblesFromLeftRightPaths(leftSet, rightPaths);
#ifdef DEBUG
            cout << "bbl " << bbl << endl;
#endif
            count += bbl;
        }
    }

    return count;
  }

  // k-mer size and bubble threshold
  int                         v_;
  int                         e_;
  int                         t_;

  // graph
  vvi                         adjList_;
};

int main(void) {
  int v, e, t;

  cin >> v >> e >> t;
  BubbleGraph graph(v, e, t);
  int to, from;
  while (e--) {
    cin >> from >> to;
    graph.AddEdge(from, to);
  }

  cout << graph.CountBubbles() << endl;

  return 0;
}
