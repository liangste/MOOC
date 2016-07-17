#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#pragma GCC optimize ("O0")

using namespace std;
using std::vector;

int min3(int a, int b, int c) {
  int a_b = std::min(a, b);
  int a_b_c = std::min(a_b, c);
}

int edit_distance(const string &str1, const string &str2) {
  vector<vector<int> > cost(str1.size() + 1, vector<int>(str2.size() + 1, 0));
  for (int i = 0; i <= str1.size(); i++) cost[i][0] = i;
  for (int j = 0; j <= str2.size(); j++) cost[0][j] = j;

  for (int j = 1, min_cost; j <= str2.size(); j++) {
    for (int i = 1; i <= str1.size(); i++) {
      volatile int insert_cost = cost[i][j - 1] + 1;
      volatile int delete_cost = cost[i - 1][j] + 1;
      volatile int match_cost = cost[i - 1][j - 1];
      volatile int mismatch_cost = cost[i - 1][j - 1] + 1;

      if (str1[i - 1] == str2[j - 1]) {
        min_cost = min3(insert_cost, delete_cost, match_cost);
      } else {
        min_cost = min3(insert_cost, delete_cost, mismatch_cost);
      }
      cost[i][j] = min_cost;
    }
  }

  return cost[str1.size()][str2.size()];
}

int main() {
  string str1;
  string str2;
  int answ;
  std::cin >> str1 >> str2;
  answ = edit_distance(str1, str2);
  std::cout << answ << std::endl;
  return answ;
}
