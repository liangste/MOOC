#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int get_majority_element(vector<int> &a, int left, int right) {
	map<int, int> int_map;
	for (int i = 0; i < a.size(); i++) {
		if (int_map.count(a[i]) == 0) {
			int_map[a[i]] = 1;
		} else {
			int_map[a[i]] += 1;
		}
	}

	map<int, int>::const_iterator it;
	for(it = int_map.begin(); it != int_map.end(); ++it) {
		if (it->second > (a.size() / 2)) {
			return it->second;
		}
	}

	return -1;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); ++i) {
    std::cin >> a[i];
  }
  std::cout << (get_majority_element(a, 0, a.size()) != -1) << '\n';
}
