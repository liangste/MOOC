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

//#define DEBUG
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

// use Reads and PrefixMap to build the Overlap Graph and store it in AdjList
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

// print the overlap graph for debugging
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

// helper function for printing a vector of integers
void PrintVectorInt(const vector<int>& v) {
  for (int j = 0; j < v.size(); j++) {
    cout << v[j] << " ";
  }
  cout << endl;
}

// use vector of int to reconstruct the genome from overlap graph
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

// helper function
// recursively calls itself in search of the one Hamiltonian cycle, stops all
// recurences if such cycle is found
void OverlapGraphGreedyTraversal_Helper(int i, vector<int>& path, vector<bool>& visited, vector<int>& sol)
{
  if (sol.size() > 0) return;
  if (path.size() == Reads.size()) {
    sol = path;
  } else {
    for (int j = 0; j < AdjList[i].size(); ++j) {
      int dst = AdjList[i][j].dst;
      if (!visited[dst]) {
        visited[dst] = true;
        path.push_back(dst);
        OverlapGraphGreedyTraversal_Helper(dst, path, visited, sol);
        visited[dst] = false;
        path.pop_back();
      }
    }
  }
}

// Master function to call into a recursive greedy traversal
void DoGreedyHamiltonian(vector<int>& sol)
{
  vector<bool> visited(Reads.size(), false);
  vector<int> path;

  path.push_back(0);
  visited[0] = true;

  // if there's a Hamiltonian cycle, it visits every vertex once, and it has
  // to be the same cycle regarless of where we start the traversal
  OverlapGraphGreedyTraversal_Helper(0, path, visited, sol);
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
  DumpOverlapGraph();
#endif

  vector<int> sol;

  DoGreedyHamiltonian(sol);

  cout << ReassembleGenomeByPath(sol) << endl;

  return 0;
}
