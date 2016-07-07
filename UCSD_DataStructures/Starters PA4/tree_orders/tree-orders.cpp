#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::ios_base;
using std::cin;
using std::cout;

class TreeOrders {
  int n;
  vector <int> key;
  vector <int> left;
  vector <int> right;

public:
  void read() {
    cin >> n;
    key.resize(n);
    left.resize(n);
    right.resize(n);
    for (int i = 0; i < n; i++) {
      cin >> key[i] >> left[i] >> right[i];
    }
  }

  void do_in_order(int node_index, vector<int>& result) {
    if (node_index >= key.size() || node_index < 0)
      return;

    do_in_order(left[node_index], result);
    result.push_back(key[node_index]);
    do_in_order(right[node_index], result);
  }


  vector <int> in_order() {
    vector<int> result;
    // Finish the implementation
    do_in_order(0, result);

    return result;
  }

  void do_pre_order(int node_index, vector<int>& result) {
    if (node_index >= key.size() || node_index < 0)
      return;

    result.push_back(key[node_index]);
    do_pre_order(left[node_index], result);
    do_pre_order(right[node_index], result);
  }

  vector <int> pre_order() {
    vector<int> result;
    // Finish the implementation
    // You may need to add a new recursive method to do that

    do_pre_order(0, result);

    return result;
  }

  void do_post_order(int node_index, vector<int>& result) {
    if (node_index >= key.size() || node_index < 0)
      return;

    do_post_order(left[node_index], result);
    do_post_order(right[node_index], result);
    result.push_back(key[node_index]);
  }

  vector <int> post_order() {
    vector<int> result;
    // Finish the implementation
    // You may need to add a new recursive method to do that

    do_post_order(0, result);

    return result;
  }
};

void print(vector <int> a) {
  for (size_t i = 0; i < a.size(); i++) {
    if (i > 0) {
      cout << ' ';
    }
    cout << a[i];
  }
  cout << '\n';
}

int main() {
  ios_base::sync_with_stdio(0);
  TreeOrders t;
  t.read();
  print(t.in_order());
  print(t.pre_order());
  print(t.post_order());
  return 0;
}
