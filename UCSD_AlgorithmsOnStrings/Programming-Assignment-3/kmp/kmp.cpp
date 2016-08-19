#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define DEBUG 0

void compute_prefix_function(const string& meta, vector<unsigned>& prefixes)
{
  prefixes[0] = 0;
  int border = 0;

  for (int i = 1; i < meta.size(); i++) {
    while (border > 0 && meta[i] != meta[border]) {
      border = prefixes[border - 1];
    }
    if (meta[i] == meta[border]) {
      border++;
    } else {
      border = 0;
    }
    prefixes[i] = border;
  }
}

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where
// the pattern starts in the text.
vector<int> find_pattern(const string& pattern, const string& text) {
  vector<int> result;
  // Implement this function yourself
  string meta_string = pattern + "$" + text;
  vector<unsigned> prefix_function(meta_string.size(), 0);

#if DEBUG
  cout << meta_string << endl;
#endif

  compute_prefix_function(meta_string, prefix_function);

#if DEBUG
  for (int i = 0; i < prefix_function.size(); i++) {
    cout << prefix_function[i];
  }
  cout << endl;
#endif

  for (int i = pattern.size() + 1; i < meta_string.size(); i++) {
    if (prefix_function[i] == pattern.size()) {
      result.push_back(i - 2 * pattern.size());
    }
  }

  return result;
}

int main() {
  string pattern, text;
  cin >> pattern;
  cin >> text;
  vector<int> result = find_pattern(pattern, text);
  for (int i = 0; i < result.size(); ++i) {
    printf("%d ", result[i]);
  }
  printf("\n");
  return 0;
}
