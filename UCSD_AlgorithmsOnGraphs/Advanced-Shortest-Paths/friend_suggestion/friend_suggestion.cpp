// an implementation of bi-directional Dijkstra search

#include <array>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <vector>

using namespace std;

#define MAX_N 1000000
#define LOOP(x, m) for (int x = 0; x < m; x++)

// destination, weight
typedef long long ll;
typedef vector<vector<pair<int, int>>> vvpii;

int N; // number of nodes
int M; // number of edges
vvpii G, rG;
array<ll, MAX_N + 1> Dist;
array<ll, MAX_N + 1> rDist;
array<int, MAX_N + 1> Prev;
array<int, MAX_N + 1> rPrev;
set<int> Proc;
set<int> rProc;

ll Bidijkstra(int s, int t) {
  Dist[s] = 0;
  rDist[t] = 0;

  // extract min
  // process
  // ...
  return -1;
}

int main(void) {
    cin >> N >> M;
    G.resize(N + 1);
    rG.resize(N + 1);

    int a, b, w;
    LOOP(i, M) {
      cin >> a >> b >> w;
      G[a].push_back({b, w});
      rG[b].push_back({a, w});
    }

    int tests;
    cin >> tests;
    int s, t;
    while (tests--) {
      cin >> s >> t;
      Dist.fill(numeric_limits<ll>::max());
      rDist.fill(numeric_limits<ll>::max());
      Prev.fill(0);
      rPrev.fill(0);
      Proc.clear();
      rProc.clear();
      // initialize data structures before starting out per cycle computation
      cout << Bidijkstra(s, t) << endl;
    }

    return 0;
}
