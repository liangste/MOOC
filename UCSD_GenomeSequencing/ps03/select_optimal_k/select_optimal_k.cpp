#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

vector<string> Reads;

bool IsOptimalK(int k) {
  set<string> kmers;
  set<string> prefixes;

  for (auto& read : Reads) {
    for (int i = 0; i <= (read.size() - k); i++) {
      string kmer = read.substr(i, k);
      // cout << "inserting kmer " << kmer << " from read " << read << endl;
      kmers.insert(kmer);
    }
  }

  // add all prefixes
  for (auto& kmer : kmers) {
    prefixes.insert(kmer.substr(0, k - 1));
  }

  // check
  for (auto& kmer : kmers) {
    string suff = kmer.substr(1);
    if (prefixes.find(suff) == prefixes.end()) return false;
  }

  return true;
}

int main(void) {
  int max_size = 0;
  string r;
  while (cin >> r)
  {
    Reads.push_back(r);
  };

  max_size = Reads[0].size();

  for (int i = max_size; i > 0; i--) {
    if (IsOptimalK(i)) {
      cout << i << endl;
      return 0;
    }
  }

  return 1;
}
