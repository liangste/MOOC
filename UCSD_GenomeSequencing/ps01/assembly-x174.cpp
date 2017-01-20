// Dataset Problem 1 : Assembling the phi X174 Genome from Error-Free Reads Using
// Overlap Graphs

#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

// represents an overlap between 2 reads from the gloabl Reads vector
struct OverlapEdge {
  int source;
  int dest;
  int weight;
  // edge value is simply reads[dest].substr(0, weight)
};

typedef vector<vector<OverlapEdge>> vvOverlapEdge;

vector<string> Reads;
vvOverlapEdge AdjList;

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

void BuildOverlapGraph() {
  for (int i = 0; i < Reads.size(); i++) {
    for (int j = 0; j < Reads.size(); j++) {
      if (i != j) {
        int m = GetPrefixSuffixMatch(Reads[i], Reads[j]);
        if (m > 0) {
          AdjList[i].push_back({i, j, m});
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
    for (int j = 0; j < AdjList[i].size(); j++) {
      cout << AdjList[i][j].source << "->" << AdjList[i][j].dest << " w=" << AdjList[i][j].weight << endl;
    }
  }
}

bool GetNextLargestUnvisitedEdge(vector<bool>& visited, OverlapEdge& nextEdge) {
  // TODO work on this
  bool found;
  for (int i = 0; i < AdjList.size(); i++) {
    for (int j = 0; j < AdjList[i].size(); j++) {
      if (!visited[AdjList[i][j].source] || !visited[AdjList[i][j].dest]) {
        if (found) {
          if (AdjList[i][j].weight > nextEdge.weight) {
            nextEdge.source = AdjList[i][j].source;
            nextEdge.dest = AdjList[i][j].dest;
            nextEdge.weight = AdjList[i][j].weight;
            found = true;
          }
        } else {
          nextEdge.source = AdjList[i][j].source;
          nextEdge.dest = AdjList[i][j].dest;
          nextEdge.weight = AdjList[i][j].weight;
          found = true;
        }
      }
    }
  }
  return found;
}

string DoTraversal(vector<OverlapEdge>& edges) {
  // TODO work on this
  vector<int> sourceIndices(edges.size());
  vector<int> destIndices(edges.size());
  vector<int> sourceWorthy(edges.size(), true);
  for (int i = 0; i < edges.size(); i++) {
    sourceIndices[edges[i].source] = i;
    destIndices[edges[i].dest] = i;
    sourceWorthy[edges[i].dest] = false;
  }

  int originalSource;
  int theSource;
  for (int i = 0; i < edges.size(); i++) {
    if (sourceWorthy[i]) {
      theSource = i;
      break;
    }
  }

  originalSource = theSource;

  string ret = Reads[theSource];
  for (int i = 0; i < edges.size(); i++) {
    int w = edges[sourceIndices[theSource]].weight;
    int d = edges[sourceIndices[theSource]].dest;
    ret += Reads[d].substr(w);
    theSource = edges[sourceIndices[theSource]].dest;
  }

  // if the last Read loops back to the first read, remove its edge from ret string
  int lastRead = edges[sourceIndices[theSource]].dest;
  for (int i = 0; i < AdjList[lastRead].size(); i++) {
    if (AdjList[lastRead][i].dest == originalSource) {
      int trimAmount = AdjList[lastRead][i].weight;
      ret = ret.substr(0, ret.size() - trimAmount);
      break;
    }
  }

  return ret;
}

string DoGreedyHamiltonian() {
  vector<bool> visited(Reads.size(), false);
  vector<OverlapEdge> HamiltonianEdges;
  while(true) {
    OverlapEdge nextEdge;
    bool hasNextEdge = GetNextLargestUnvisitedEdge(visited, nextEdge);
    if (hasNextEdge) {
      HamiltonianEdges.push_back(nextEdge);
      visited[nextEdge.source] = true;
      visited[nextEdge.dest] = true;
    } else {
      break;
    }
  }
  return DoTraversal(HamiltonianEdges);
}

int main(void) {
  string read;
  set<string> readSet;
  while(cin >> read)
  {
    if (readSet.find(read) == readSet.end()) {
      readSet.insert(read);
      Reads.push_back(read);
    }
  }

  AdjList.resize(Reads.size() + 1);

  BuildOverlapGraph();
  DumpOverlapGraph();
  cout << DoGreedyHamiltonian() << endl;

  return 0;
}
