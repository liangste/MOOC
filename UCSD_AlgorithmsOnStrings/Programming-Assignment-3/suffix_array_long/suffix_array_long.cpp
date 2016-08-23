// works only with letters A, C, G, T, and $

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>

using namespace std;

#define DEBUG 1

void SortCharacters(const std::string& S, vector<int>& order) {
  order.resize(S.size());
  map<char, int> count;

  count['$'] = 0;
  count['A'] = 0;
  count['C'] = 0;
  count['G'] = 0;
  count['T'] = 0;

  for (int i = 0; i < S.size(); i++) {
    count[S[i]]++;
  }

  count['A'] += count ['$'];
  count['C'] += count ['A'];
  count['G'] += count ['C'];
  count['T'] += count ['G'];

  for (int i = S.size() - 1; i >= 0; i--) {
    count[S[i]]--;
    order[count[S[i]]] = i;
  }
}

void ComputeClassOrder(const std::string& S, const vector<int>& order, vector<int>& Class) {
  Class.resize(S.size());
  Class[order[0]] = 0;
  for (int i = 1; i < S.size(); i++) {
    if (S[order[i]] != S[order[i - 1]]) {
      Class[order[i]] = Class[order[i - 1]] + 1;
    } else {
      Class[order[i]] = Class[order[i - 1]];
    }
  }
}

vector<int> SortDoubled(const std::string& S, int L, const vector<int>& order, vector<int>& Class) {
  vector<int> count(S.size(), 0);
  vector<int> newOrder(S.size());

  for (int i = 0; i < S.size(); i++) {
    count[Class[i]]++;
  }

  for (int i = 1; i < S.size(); i++) {
    count[i] += count[i - 1];
  }

  for (int i = (S.size() - 1); i >= 0; i--) {
    int start = (order[i] - L + S.size()) % S.size();
    int cl = Class[start];
    count[cl]--;
    newOrder[count[cl]] = start;
  }

  return newOrder;
}

vector<int> UpdateClasses(const vector<int>& newOrder, vector<int>& Class, int L) {
  int n = newOrder.size();
  vector<int> newClass(n);
  newClass[newOrder[0]] = 0;

  for (int i = 1; i < n; i++) {
    int cur = newOrder[i];
    int prev = newOrder[i - 1];
    int mid = cur + L;
    int midPrev = (prev + L) % n;
    if (Class[cur] != Class[prev] || Class[mid] != Class[midPrev]) {
      newClass[cur] = newClass[prev] + 1;
    } else {
      newClass[cur] = newClass[prev];
    }
  }

  return newClass;
}

vector<int> BuildSuffixArray(const string& text) {
  //vector<int> result;
  vector<int> order;
  vector<int> Class;

  // Implement this function yourself
  SortCharacters(text, order);

#if DEBUG
  cout << " == Printing Order == " << endl;
  for (auto& o : order) {
    cout << o << " ";
  }
  cout << endl;
#endif

  ComputeClassOrder(text, order, Class);

#if DEBUG
  cout << " == Printing Class == " << endl;
  for (auto& o : Class) {
    cout << o << " ";
  }
  cout << endl;
#endif

  int L = 1;

  while (L < text.size()) {
#if DEBUG
    cout << "L = " << L << endl;
#endif
    vector<int> newOrder = SortDoubled(text, L, order, Class);
    order = newOrder;
#if DEBUG
    cout << " == Printing newOrder == " << endl;
    for (auto& o : order) {
      cout << o << " ";
    }
    cout << endl;
#endif
    vector<int> newClass = UpdateClasses(order, Class, L);
    Class = newClass;
#if DEBUG
    cout << " == Printing newClass == " << endl;
    for (auto& o : Class) {
      cout << o << " ";
    }
    cout << endl;
#endif
    L *= 2;
  }

  return order;
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
