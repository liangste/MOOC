#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define DEBUG 0

using namespace std;

// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position
//       of this character in the sorted array of
//       all characters of the text.
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string& bwt,
                   map<char, int>& starts,
                   map<char, vector<int> >& occ_count_before) {

#if DEBUG
  cout << bwt << endl;
#endif

  int cnt_a, cnt_c, cnt_g, cnt_t, cnt_cashe;
  cnt_a = cnt_c = cnt_g = cnt_t = cnt_cashe = 0;
  occ_count_before['A'].resize(bwt.size());
  occ_count_before['C'].resize(bwt.size());
  occ_count_before['G'].resize(bwt.size());
  occ_count_before['T'].resize(bwt.size());
  occ_count_before['$'].resize(bwt.size());

#if DEBUG
  cout << "=  A, C, G, T, $ =" << endl;
#endif
  for (int i = 0; i < bwt.size(); i++) {
    switch(bwt[i]) {
      case 'A':
        cnt_a++;
        break;
      case 'C':
        cnt_c++; break;
      case 'G':
        cnt_g++; break;
      case 'T':
        cnt_t++; break;
      case '$':
        cnt_cashe++; break;
      default:
        break;
    }

    occ_count_before['A'][i] = cnt_a;
    occ_count_before['C'][i] = cnt_c;
    occ_count_before['G'][i] = cnt_g;
    occ_count_before['T'][i] = cnt_t;
    occ_count_before['$'][i] = cnt_cashe;

#if DEBUG
  cout << i << ": ";
  cout << occ_count_before['A'][i] << ", ";
  cout << occ_count_before['C'][i] << ", ";
  cout << occ_count_before['G'][i] << ", ";
  cout << occ_count_before['T'][i] << ", ";
  cout << occ_count_before['$'][i] << ", ";
  cout << endl;
#endif
  }

  #if DEBUG
    cout << "=  A, C, G, T, $ =" << endl;
  #endif

  starts['$'] = 0;
  starts['A'] = 1;
  starts['C'] = cnt_a + starts['A'];
  starts['G'] = cnt_c + starts['C'];
  starts['T'] = cnt_g + starts['G'];

#if DEBUG
  cout << "start A = " << starts['A'] << ", ";
  cout << "start C = " << starts['C'] << ", ";
  cout << "start G = " << starts['G'] << ", ";
  cout << "start T = " << starts['T'] << endl;
#endif
}

// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrences(const string& pattern,
                     const string& bwt,
                     map<char, int>& starts,
                     map<char, vector<int> >& occ_count_before) {
#if DEBUG
  cout << "pattern = " << pattern << endl;
#endif
  int top = 0;
  int bottom = bwt.size() - 1;
  int symbolIdx = pattern.size() - 1;
  char symbol;
  bool lastColumnContainsSymbol;
  while (top <= bottom) {
#if DEBUG
    cout << "top:bottom = " << top << ":" << bottom << endl;
#endif
    if (symbolIdx < 0) {
      return bottom - top + 1;
    }

    symbol = pattern[symbolIdx--];
#if DEBUG
    cout << "symbol = " << symbol << endl;
#endif
    lastColumnContainsSymbol = false;
    for (int i = top; i <= bottom; i++) {
      if (symbol == bwt[i]) {
        top = i;
        lastColumnContainsSymbol = true;
        break;
      }
    }

    if (lastColumnContainsSymbol) {
      top = starts[symbol] + occ_count_before[symbol][top] - 1;
      bottom = starts[symbol] + occ_count_before[symbol][bottom] - 1;
#if DEBUG
    cout << "(new!) top:bottom = " << top << ":" << bottom << endl;
#endif
    } else {
      return 0;
    }
  }
  return 0;
}


int main() {
  string bwt;
  cin >> bwt;
  int pattern_count;
  cin >> pattern_count;
  // Start of each character in the sorted list of characters of bwt,
  // see the description in the comment about function PreprocessBWT
  map<char, int> starts;
  // Occurrence counts for each character and each position in bwt,
  // see the description in the comment about function PreprocessBWT
  map<char, vector<int> > occ_count_before;
  // Preprocess the BWT once to get starts and occ_count_before.
  // For each pattern, we will then use these precomputed values and
  // spend only O(|pattern|) to find all occurrences of the pattern
  // in the text instead of O(|pattern| + |text|).
  PreprocessBWT(bwt, starts, occ_count_before);
  for (int pi = 0; pi < pattern_count; ++pi) {
    string pattern;
    cin >> pattern;
    int occ_count = CountOccurrences(pattern, bwt, starts, occ_count_before);
    printf("%d ", occ_count);
  }
  printf("\n");
  return 0;
}
