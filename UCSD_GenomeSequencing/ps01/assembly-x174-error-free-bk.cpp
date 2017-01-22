// Dataset Problem 1 : Assembling the phi X174 Genome from Error-Free Reads Using
// Overlap Graphs

#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#define DEBUG
#define DEBUG2
#define MIN_OVERLAP_LENGTH 12

using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;

vector<string> Reads;
// weignt from source->dest, 0 if no edge
// i.e. AdjMatrix[0][1] contains edge weight from 0'th read to 1st read
char * AdjMatrix;
#define MATRIX_VALUE(row, col) (AdjMatrix[row + col * Reads.size()])

// get max length of suffix of suffixSource that match prefix of prefixSource
// returns 0 if no such suffix-prefix match is found between the two inputs
int GetPrefixSuffixMatch(const string& suffixSource, const string& prefixSource) {
  // this step is way too slow
  /*
  return 0;
  int n = suffixSource.size() - 1;
  for (int i = 1; i < suffixSource.size() && n >= MIN_OVERLAP_LENGTH; i++, n--) {
    if (suffixSource.substr(i) == prefixSource.substr(0, n))
      return n;
  }
  */

  int max = INT_MIN;
  int len1 = suffixSource.length();
  int len2 = prefixSource.length();
  for (int i = 1; i <= min(len1, len2); i++)
  {
      // compare last i characters in str1 with first i
      // characters in str2
      if (suffixSource.compare(len1-i, i, prefixSource, 0, i) == 0)
      {
          if (max < i)
          {
              //update max and str
              max = i;
          }
      }
  }

  if (max == INT_MIN) return -1;
  return max;
}

void BuildOverlapGraph() {
  for (int i = 0; i < Reads.size(); i++) {
    for (int j = 0; j < Reads.size(); j++) {
      if (i != j) {
        int m = GetPrefixSuffixMatch(Reads[i], Reads[j]);
        if (m > 0) {
          MATRIX_VALUE(i, j) = m;
        }
      }
    }
  }
}

void DumpOverlapGraph() {
  for (int i = 0; i < Reads.size(); i++) {
    cout << (i) << " " << Reads[i] << endl;
  }

  for (int i = 0; i < Reads.size(); i++) {
    for (int j = 0; j < Reads.size(); j++) {
      cout << (int) MATRIX_VALUE(i, j) << " ";
    }
    cout << endl;
  }
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

// return -1 if greedy path from source s does not complete the graph
// otherwise return greedy sum of edges from this source
int GetGreedyOverlapPathValueFromSource(int s, vector<int>& path) {
  return -1;
}

void PrintVectorInt(vector<int>& v) {
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
    ret += Reads[path[i]].substr(MATRIX_VALUE(cur, path[i]));
    cur = path[i];
  }

  // if last vertex links back to the first
  int trim_value = MATRIX_VALUE(path[path.size() - 1], path[0]);
  if (trim_value) {
    ret = ret.substr(0, ret.size() - trim_value);
  }

  return ret;
}

// Hamiltonian Path is a path in a graph that traverses all vertices. But depending
// on the starting vertex, a greedy path may not encounter all vertices.
// This can happen if 1) starting vertex is wrong) or 2) at any given point
// when making a decision, we encounter multiple edges with same weight
// https://www.hackerearth.com/practice/algorithms/graphs/hamiltonian-path/tutorial/
string DoGreedyHamiltonian() {
  // 1. generate permutations
  // for each permutation, check wehther path is valid
  // return path with largest sum edge weights
  vvi paths;
  //GenPermutationPaths(paths, Reads.size());

  int m;
  int bestPathWeight = 0;
  vector<int> path;

  for (int s = 0; s < Reads.size(); s++) {
    path.clear();
    m = GetGreedyOverlapPathValueFromSource(s, path);
    if (m > 0) {
      if (m > bestPathWeight) {
        bestPathWeight = m;
        break;
      }
    }
  }

  PrintVectorInt(path);

#ifdef DEBUG
  cout << "best path has value " << bestPathWeight << endl;
  PrintVectorInt(path);
#endif

  return ReassembleGenomeByPath(path);
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

  AdjMatrix = new char[Reads.size() * Reads.size()];
  memset(AdjMatrix, 0, Reads.size() * Reads.size());

  BuildOverlapGraph();
#ifdef DEBUG
  //DumpOverlapGraph();
#endif
  cout << DoGreedyHamiltonian() << endl;

  delete[] AdjMatrix;

  return 0;
}
