#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int max3(int a, int b, int c) {
  int a_b = std::max(a, b);
  return std::max(a_b, c);
}

int max7(int a, int b, int c, int d, int e, int f, int g) {
  int abc = max3(a, b, c);
  int def = max3(d, e, f);
  return max3(abc, def, g);
}

int lcs3(vector<int> &a, vector<int> &b, vector<int> &c) {
  //write your code here
  vector<vector<vector<int> > > cost(a.size() + 1, vector<vector<int> >(b.size() + 1, vector<int>(c.size() + 1)));

  for (int i = 0; i <= a.size(); i++) cost[i][0][0] = 0;
  for (int j = 0; j <= b.size(); j++) cost[0][j][0] = 0;
  for (int k = 0; k <= c.size(); k++) cost[0][0][k] = 0;

  int max_cost;;
  for (int k = 1; k <= c.size(); k++) {
    for (int j = 1; j <= b.size(); j++) {
      for (int i = 1; i <= a.size(); i++) {
        int c1 = cost[i - 1][j - 1][k - 1];
        int c2 = cost[i - 1][j - 1][k];
        int c3 = cost[i - 1][j][k - 1];
        int c4 = cost[i][j - 1][k - 1];
        int c5 = cost[i - 1][j][k];
        int c6 = cost[i][j - 1][k];
        int c7 = cost[i][j][k - 1];
        int c8_match = cost[i - 1][j - 1][k - 1] + 1;

        if (a[i - 1] == b[j - 1] && a[i - 1] == c[k - 1]) {
#if DEBUG
          cout << "match " << a[i - 1] << " at (i, j, k) = " << i << "," << j << "," << k << endl;
#endif
          max_cost = max7(c8_match, c2, c3, c4, c5, c6, c7);
        } else {
          max_cost = max7(c1, c2, c3, c4, c5, c6, c7);
        }
        cost[i][j][k] = max_cost;
      }
    }
  }

#if DEBUG
  for (int k = 0; k <= c.size(); k++) {
    cout << "======== k = " << k << " =========" << endl;
    for (int j = 0; j <= b.size(); j++) {
      for (int i = 0; i <= a.size(); i++) {
        cout << cost[i][j][k] << " ";
      }
      cout << endl;
    }
  }
#endif

  return cost[a.size()][b.size()][c.size()];
}

int main() {
  size_t an;
  std::cin >> an;
  vector<int> a(an);
  for (size_t i = 0; i < an; i++) {
    std::cin >> a[i];
  }
  size_t bn;
  std::cin >> bn;
  vector<int> b(bn);
  for (size_t i = 0; i < bn; i++) {
    std::cin >> b[i];
  }
  size_t cn;
  std::cin >> cn;
  vector<int> c(cn);
  for (size_t i = 0; i < cn; i++) {
    std::cin >> c[i];
  }
  std::cout << lcs3(a, b, c) << std::endl;
}
