#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#define DEBUG 0

int a_start;
int c_start;
int g_start;
int t_start;

void SortCharacters(const std::string& S, vector<int>& order) {
  order.resize(S.size());
  map<char, int> count;

  count['@'] = 0;
  count['A'] = 0;
  count['C'] = 0;
  count['G'] = 0;
  count['T'] = 0;

  for (int i = 0; i < S.size(); i++) {
    count[S[i]]++;
  }

  count['A'] += count ['@']; a_start = count['@'];
  count['C'] += count ['A']; c_start = count['A'];
  count['G'] += count ['C']; g_start = count['C'];
  count['T'] += count ['G']; t_start = count['G'];

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

  ComputeClassOrder(text, order, Class);

  int L = 1;

  while (L < text.size()) {
    order = SortDoubled(text, L, order, Class);;
    Class = UpdateClasses(order, Class, L);
    L *= 2;
  }

  return order;
}

bool StringLexiLessThan(const std::string& A, const std::string B) {

  if (A == B) {
    return false;
  }

  int i;
  for (i = 0; i < A.size() && i < B.size(); i++) {
    if (A[i] != B[i]) {
      break;
    }
  }

  if (i == A.size()) {
    return false;
  }

  if (A[i] < B[i]) {
    return true;
  } else {
    return false;
  }
}

vector<int> FindOccurrences(const string& pattern, const string& text, const vector<int>& suffix_array) {
  vector<int> result;

  int minIndex = 0;
  int maxIndex = text.size();

#if DEBUG
  int i = 0;
  for (auto & s : suffix_array) {
    cout << i << ":" << s << ":" << text.substr(s) << endl;
    i++;
  }
#endif

  while (minIndex < maxIndex) {
    int midIndex = (minIndex + maxIndex) / 2;
    if (pattern > text.substr(suffix_array[midIndex])) {
      minIndex = midIndex + 1;
    } else {
      maxIndex = midIndex;
    }
  }

  int start = minIndex;
  maxIndex = text.size();

  while(minIndex < maxIndex) {
    int midIndex = (minIndex + maxIndex) / 2;
    if (StringLexiLessThan(pattern, text.substr(suffix_array[midIndex]))) {
      maxIndex = midIndex - 1;
    } else {
      minIndex = midIndex + 1;
    }
  }

  int sstart = start;
  while (sstart < maxIndex) {
    if (pattern > text.substr(suffix_array[start]))
      break;
    sstart++;
  }
  int end = sstart;

#if DEBUG
  cout << "start = " << start << endl;
  cout << "end = " << end << endl;
#endif

  if (start > end) {
    return result;
  } else if (start == end) {
    if (pattern == text.substr(suffix_array[start]).substr(0, pattern.size())) {
      result.push_back(suffix_array[start]);
    }
  } else {
    for (int i = start; i <= end && i < suffix_array.size(); i++) {
      result.push_back(suffix_array[i]);
    }
  }

  return result;
}

int main() {

  char buffer[100001];
  scanf("%s", buffer);
  string text = buffer;
  text += '@';
  vector<int> suffix_array = BuildSuffixArray(text);
  int pattern_count;
  scanf("%d", &pattern_count);
  vector<bool> occurs(text.length(), false);
  for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index) {
    scanf("%s", buffer);
    string pattern = buffer;
    vector<int> occurrences = FindOccurrences(pattern, text, suffix_array);
    for (int j = 0; j < occurrences.size(); ++j) {
      occurs[occurrences[j]] = true;
    }
  }
  for (int i = 0; i < occurs.size(); ++i) {
    if (occurs[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");
  return 0;
}
