// Dijkstra's algorithm implementation

#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

using namespace std;

#define LOOP(x, m) for(int x = 0; x < m; x++)

typedef long long ll;
// target, weight
typedef vector<vector<pair<int, int>>> vvii;

int N; // number of vertices
int M; // number of edges
vvii AdjList; // directed graph

struct Node {
  int idx;
  ll dist;
};

class HashedNodeQueue {
public:
private:
};

ll SolveDijkstra(int s, int t) {
  return -1;
}

int main(void) {
  cin >> N >> M;
  AdjList.resize(N + 1); // 0th index is not used

  int a, b, w;
  LOOP(x, M) {
    cin >> a >> b >> w;
    AdjList[a].push_back({b, w});
  }

  int n_tests, s, t;
  cin >> n_tests;
  while(n_tests--) {
    cin >> s >> t;
    cout << SolveDijkstra(s, t) << endl;
  }

  return 0;
}
