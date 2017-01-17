// Dataset Problem 1 : Assembling the phi X174 Genome from Error-Free Reads Using
// Overlap Graphs

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef vector<vector<pair<int, int>>> vvii;


// get max length of suffix of suffixSource that match prefix of prefixSource
// returns 0 if no such suffix-prefix match is found between the two inputs
int GetPrefixSuffixMatch(const string& suffixSource, const string& prefixSource) {
  int n = suffixSource.size() - 1;
  for (int i = 1; i < suffixSource.size(); i++, n--) {
    if (suffixSource.substr(i) == prefixSource.substr(0, n))
      return n;
  }
  return 0;
}

// build overlap graph from reads
void BuildOverlapGraph(const vector<string>& reads, vvii& adjList) {
  for (int i = 0; i < reads.size(); i++) {
    for (int j = 0; j < reads.size(); j++) {
      if (i != j) {
        int m = GetPrefixSuffixMatch(reads[i], reads[j]);
        if (m > 0) {
          adjList[i].push_back({m, j});
        }
      }
    }
  }
}

// dump overlap graph for debugging
void DumpOverlapGraph(const vector<string>& reads, vvii& adjList) {
  for (int i = 0; i < reads.size(); i++) {
    cout << (i) << " " << reads[i] << endl;
  }

  for (int i = 0; i < adjList.size(); i++) {
    for (int j = 0; j < adjList[i].size(); j++) {
      cout << i << "->" << adjList[i][j].second << " w=" << adjList[i][j].first << endl;
    }
  }
}

// find Hamiltonian cycle from the overlap graph in a greedy fashion, traverse it and return
// string spelled out by its path
string DoGreedyHamiltonian(const vector<string>& reads, vvii& adjList) {
  return "";
}

int main(void) {
  vector<string> reads;
  string read;
  vvii adjList; // edge weight, read target

  while(cin >> read)
  {
    reads.push_back(read);
  }

  adjList.resize(reads.size() + 1);

  BuildOverlapGraph(reads, adjList);
  DumpOverlapGraph(reads, adjList);

  return 0;
}
