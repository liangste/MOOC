// Dataset Problem 1 : Assembling the phi X174 Genome from Error-Free Reads Using
// Overlap Graphs

// TODO Overlap graph is pretty sparse, use adjacency list!

#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#define DEBUG
//#define DEBUG2
#define MIN_OVERLAP_LENGTH 1 // minimum is 1

using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

typedef struct OverlapGraphEdge {
  int src;
  int dst;
  int wt;
} OGEdge_t;

vector<string> Reads;
vector<vector<OGEdge_t>> AdjList;
// maps from string -> set of indices of Reads containing the string as prefix
multimap<string, int> PrefixMap;
vi MaxEdgeValues;
char * AdjMatrix;
#define MATRIX_VALUE(row, col) (AdjMatrix[row + col * Reads.size()])

// get max length of suffix of suffixSource that match prefix of prefixSource
// returns 0 if no such suffix-prefix match is found between the two inputs
int GetPrefixSuffixMatch(const string& suffixSource, const string& prefixSource) {
  int max = INT_MIN;
  int sslen = suffixSource.length();
  for (int i = sslen; i >= MIN_OVERLAP_LENGTH; i--)
  {
      if (suffixSource.compare(sslen - i, i, prefixSource, 0, i) == 0)
      {
          if (max < i)
          {
              max = i;
          }
      }
  }

  if (max == INT_MIN) return -1;
  return max;
}

void BuildOverlapGraph() {
  MaxEdgeValues = vi(Reads.size(), 0);

  // build prefixes
  for (int i = 0; i < Reads.size(); i++) {
    for (int p = MIN_OVERLAP_LENGTH; p < Reads[i].size(); p++) {
      PrefixMap.insert({Reads[i].substr(0, p), i});
    }
  }

  // iterate over suffixes and make directed edges for the overlap graph
  for (int i = 0; i < Reads.size(); i++) {
    for (int s = 1; s <= (Reads[i].size() - MIN_OVERLAP_LENGTH); s++) {
      string suffix_i = Reads[i].substr(s);

      // we're looking at suffix that doesn't contribute much to Reads[i], to next
      //if (suffix_i.size() < MaxEdgeValues[i])
      //  continue;

      if (PrefixMap.find(suffix_i) != PrefixMap.end()) {
        // build directed edges from i to everything that's mapped here
        auto range = PrefixMap.equal_range(suffix_i);
        for (auto r = range.first; r != range.second; ++r) {
          int len = suffix_i.size();
          if (len > MaxEdgeValues[i]) {
            MaxEdgeValues[i] = len;

          }
          AdjList[i].push_back({.src=i, .dst=r->second, .wt=len});
        }
      }
    }
  }
}

void DumpOverlapGraph() {
  for (int i = 0; i < Reads.size(); i++) {
    cout << (i) << " " << Reads[i] << endl;
  }

  for (int i = 0; i < AdjList.size(); i++) {
    cout << "Read " << i;
    for (int j = 0; j < AdjList[i].size(); j++) {
      cout << " {src=" <<  (int) AdjList[i][j].src
        << " dst="<<  (int) AdjList[i][j].dst
        << " wt="<<  (int) AdjList[i][j].wt << "} ";
    }
    cout << endl;
  }
}

// return -1 if greedy path from source s does not complete the graph
// otherwise return greedy sum of edges from this source
int GetGreedyOverlapPathValueFromSource(int s, vector<int>& path) {
  vector<bool> visited(Reads.size(), false);
  if (Reads.size() == 0) return -1;

  path.push_back(s);
  visited[s] = true;

  int nv ;
  char wt;
  int wt_sum = 0;

  int cnt = Reads.size();
  while (cnt--) {
    nv = -1;
    wt = 0;

    for (int i = 0; i < AdjList[s].size(); i++) {
      if (AdjList[s][i].wt > wt && !visited[AdjList[s][i].dst]) {
        nv = AdjList[s][i].dst;
        wt = AdjList[s][i].wt;
      }
    }

    if (nv == -1) {
      return -1;
    }

    visited[nv] = true;
    path.push_back(nv);
    s = nv;
    wt_sum += wt;
  }

  return wt_sum;
}

void PrintVectorInt(const vector<int>& v) {
  for (int j = 0; j < v.size(); j++) {
    cout << v[j] << " ";
  }
  cout << endl;
}

string ReassembleGenomeByPath(vector<int>& path) {
  if (path.size() == 0) return "";
  int cur = path[0];
  string ret = Reads[cur];

  for (int i = 1; i < path.size(); i++) {
    int overlap_weight;
    for (auto& e : AdjList[cur]) {
      if (e.dst == path[i]) {
        overlap_weight = e.wt;
        break;
      }
    }
    ret += Reads[path[i]].substr(overlap_weight);
    cur = path[i];
  }

  int trim_weight = 0;
  for (auto& e : AdjList[path[path.size() - 1]]) {
    if (e.dst == path[0]) {
      trim_weight = e.wt;
      break;
    }
  }

  // if last vertex links back to the first
  if (trim_weight) {
    ret = ret.substr(0, ret.size() - trim_weight);
  }

  return ret;
}

// return -1 if path is not a valid path in the overlap graph
// otherwise returns sum of edges on this path
int GetOverlapPathValue(vi& path) {
  if (path.size() == 0) return -1;

  int cur = path[0];
  int edgeSum = 0;
  for (int i = 1; i < path.size(); i++) {
    // TODO use adjacency matrix instead of edge
    if (MATRIX_VALUE(cur, path[i]) > 0) {
      edgeSum += MATRIX_VALUE(cur, path[i]);
      cur = path[i];
    } else {
      return -1;
    }
  }

  return edgeSum;
}

// Hamiltonian Path is a path in a graph that traverses all vertices. But depending
// on the starting vertex, a greedy path may not encounter all vertices.
// This can happen if 1) starting vertex is wrong) or 2) at any given point
// when making a decision, we encounter multiple edges with same weight
// https://www.hackerearth.com/practice/algorithms/graphs/hamiltonian-path/tutorial/
string DoGreedyHamiltonian() {
  cout << "test 0" << endl;
  int m;
  int bestPathWeight = 0;
  vector<int> path;

  cout << "test 1" << endl;
  for (int s = 0; s < Reads.size(); s++) {
    cout << "check source " << s << endl;
    path.clear();
    m = GetGreedyOverlapPathValueFromSource(s, path);
    if (m > 0) {
      cout << "path with weight " << m << " found" << endl;
      if (m > bestPathWeight) {
        bestPathWeight = m;
        break;
      }
    }
  }
  cout << "test 2" << endl;

#ifdef DEBUG
  if (bestPathWeight > 0) {
    PrintVectorInt(path);
    cout << "best path has value " << bestPathWeight << endl;
  }
#endif

  cout << "test 3" << endl;
  return (bestPathWeight > 0) ? ReassembleGenomeByPath(path) : "";
}

vector<vector<int>> AllPaths;

void BuildAllPaths_Helper(int i, vector<int>& path, vector<bool>& visited, vector<vector<int>>& allPaths)
{
  if (allPaths.size() > 0) return;
  if (path.size() == Reads.size()) {
    allPaths.push_back(path);
  } else {
    for (int j = 0; j < AdjList[i].size(); ++j) {
      int dst = AdjList[i][j].dst;
      if (!visited[dst]) {
        visited[dst] = true;
        path.push_back(dst);
        BuildAllPaths_Helper(dst, path, visited, allPaths);
        visited[dst] = false;
        path.pop_back();
      }
    }
  }
}

void BuildAllPaths(vector<vector<int>>& allPaths)
{
  vector<bool> visited(Reads.size(), false);
  vector<int> path;

  path.push_back(0);
  visited[0] = true;
  BuildAllPaths_Helper(0, path, visited, allPaths);
}

int main(void) {
  string read;
  set<string> readSet; // remove duplicate reads
  while(cin >> read)
  {
    if (readSet.find(read) == readSet.end()) {
      readSet.insert(read);
      Reads.push_back(read);
    }
  }

  AdjList.resize(Reads.size());

  BuildOverlapGraph();
#ifdef DEBUG
  //DumpOverlapGraph();
#endif

  vector<vector<int>> allPaths;

  BuildAllPaths(allPaths);

  //PrintVectorInt(allPaths[0]);
  cout << ReassembleGenomeByPath(allPaths[0]) << endl;
  // cout << DoGreedyHamiltonian() << endl;

  return 0;
}
