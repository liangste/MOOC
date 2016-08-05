#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>

using namespace std;

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text) {
  vector<int> result;

  map<string, int> suffixArrayMap;
  for (int i = 0; i < text.size(); i++) {
    string s = text.substr(i);
    suffixArrayMap[s] = i;
  }

  for (auto& a : suffixArrayMap) {
    result.push_back(a.second);
  }

  return result;
}

int main() {
  string text;
  cin >> text;
  vector<int> suffix_array = BuildSuffixArray(text);
  for (int i = 0; i < suffix_array.size(); ++i) {
    cout << suffix_array[i] << ' ';
  }
  cout << endl;
  return 0;
}
