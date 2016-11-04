#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <climits>
#include <vector>
#include <unistd.h>

using namespace std;

typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<vector<int>> vvi;

#define LOOP(x, n) for(int x = 0; x < n; x++)

struct Node {
  int n;
  int w;
  set<Node *> children;
  Node(int N, int W) : n(N), w(W) {};
};

int N;
vi FunFactor;
vvi AdjList;
vi DP;

Node * BuildTree() {
  vb visited(N, false);

  vector<Node*> nodes;
  LOOP(i, N)
    nodes.push_back(new Node(i, FunFactor[i]));

  // do a traversal
  queue<int> visitQueue;
  visitQueue.push(0);

  while (!visitQueue.empty()) {
    int f = visitQueue.front();
    visitQueue.pop();
    visited[f] = true;
    LOOP(i, AdjList[f].size()) {
      int n = AdjList[f][i];
      if (!visited[n]) {
        nodes[f]->children.insert(nodes[n]);
        visitQueue.push(n);
      }
    }
  }

  return nodes[0];
}

int SolveIndependetSet(Node * node) {
  if (node == NULL) return 0;

  int n = node->n;
  int w = node->w;
  if (DP[n] == INT_MAX) {
    // if has no children
    if (node->children.empty()) {
      DP[n] = w;
    } else {
      int v_grandchildren = w;
      int v_children = 0;

      for (auto children : node->children) {
        v_children += SolveIndependetSet(children);
        for (auto cchildren : children->children) {
          v_grandchildren += SolveIndependetSet(cchildren);
        }
      }

      DP[n] = max(v_children, v_grandchildren);
    }
  }
  return DP[n];
}

int main(void) {
  cin >> N;

  int n;
  LOOP(i, N) {
    cin >> n;
    FunFactor.push_back(n);
  }

  AdjList.resize(N);
  int a, b;
  LOOP(i, N - 1) {
    cin >> a >> b;
    AdjList[a - 1].push_back(b - 1);
    AdjList[b - 1].push_back(a - 1);
  }

  DP = vi(N, INT_MAX);
  cout << SolveIndependetSet(BuildTree()) << endl;

  return 0;
}
