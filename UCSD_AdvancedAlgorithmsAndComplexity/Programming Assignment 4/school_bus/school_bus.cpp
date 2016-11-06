#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

#define LOOP(i, m) for(int i = 0; i < m; i++)

typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef vector<vector<pair<int, int>>> vvpii;

int Mask;
int N; // number of vertices
int M; // number of edges
vvpii Edges;
vvi Cost;

int main(void) {
  cin >> N >> M;

  Mask = (1 << N) - 1;
  Edges.resize(M);
  Cost.resize(Mask, vi(N, INT_MAX));

  int a, b, w;
  LOOP(i, M) {
    cin >> a >> b >> w;
    Edges[a - 1].push_back({b - 1, w});
    Edges[b - 1].push_back({a - 1, w});
  }

  cout << std::hex << (Mask) << endl;
}
