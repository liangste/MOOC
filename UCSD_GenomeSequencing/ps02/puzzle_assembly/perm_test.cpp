#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {
  int s;
  if (argc < 2) {
    cout << "not enough arguments" << endl;
    return 1;
  }
  s = atoi(argv[1]);

  vector<int> p(s);
  std::iota(p.begin(), p.end(), 0);
  do {
  } while (next_permutation(p.begin(), p.end()));

  return 0;
}
