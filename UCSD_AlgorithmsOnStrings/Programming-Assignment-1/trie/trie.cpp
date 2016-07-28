#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
  trie t;
  edges root;
  t.push_back(root);
  uint32_t currentNodeId;
  for (auto& p : patterns) {
    currentNodeId = 0;
    for (int i = 0; i < p.size(); i++) {
      char currentSymbol = p[i];
      if (t[currentNodeId].find(currentSymbol) != t[currentNodeId].end()) {
        currentNodeId = t[currentNodeId][currentSymbol];
      } else {
        edges newNode;
        t[currentNodeId][currentSymbol] = t.size();
        t.push_back(newNode);
        currentNodeId = t.size() - 1;
      }
    }
  }

  return t;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++) {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  trie t = build_trie(patterns);
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << "\n";
    }
  }

  return 0;
}
