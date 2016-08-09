#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <stdint.h>
#include <algorithm>

using namespace std;

#define DEBUG 0

typedef map<char, int> edges;
typedef vector<edges> trie;

/*
trie build_trie(const string& pattern, vector<int>& parents) {
  trie t;
  edges root;
  t.push_back(root);
  uint32_t currentNodeId;
  parents.clear();
  parents.push_back(-1); // root

	for (int j = 0; j < pattern.size(); j++) {
		string p = pattern.substr(j);
		currentNodeId = 0;
		for (int i = 0; i < p.size(); i++) {
	    char currentSymbol = p[i];
	    if (t[currentNodeId].find(currentSymbol) != t[currentNodeId].end()) {
	      currentNodeId = t[currentNodeId][currentSymbol];
	    } else {
	      edges newNode;
	      t[currentNodeId][currentSymbol] = t.size();
	      t.push_back(newNode);
        parents.push_back(currentNodeId);
	      currentNodeId = t.size() - 1;
	    }
	  }
	}

  return t;
}
*/

trie build_trie(const string& pattern1, const string& pattern2, vector<int>& parents,
    vector<bool>& firsts, vector<bool>& seconds) {
  trie t;
  edges root;
  t.push_back(root);
  uint32_t currentNodeId;
  parents.clear();
  parents.push_back(-1); // root
  firsts.clear();
  seconds.clear();
  firsts.push_back(true);
  seconds.push_back(true);

	for (int j = 0; j < pattern1.size(); j++) {
		string p = pattern1.substr(j);
		currentNodeId = 0;
		for (int i = 0; i < p.size(); i++) {
	    char currentSymbol = p[i];
	    if (t[currentNodeId].find(currentSymbol) != t[currentNodeId].end()) {
	      currentNodeId = t[currentNodeId][currentSymbol];
        firsts[currentNodeId] = true;
	    } else {
	      edges newNode;
	      t[currentNodeId][currentSymbol] = t.size();
	      t.push_back(newNode);
        parents.push_back(currentNodeId);
	      currentNodeId = t.size() - 1;
        firsts.push_back(true);
        seconds.push_back(false);
	    }
	  }
	}

  for (int j = 0; j < pattern2.size(); j++) {
		string p = pattern2.substr(j);
		currentNodeId = 0;
		for (int i = 0; i < p.size(); i++) {
	    char currentSymbol = p[i];
	    if (t[currentNodeId].find(currentSymbol) != t[currentNodeId].end()) {
	      currentNodeId = t[currentNodeId][currentSymbol];
        seconds[currentNodeId] = true;
	    } else {
	      edges newNode;
	      t[currentNodeId][currentSymbol] = t.size();
	      t.push_back(newNode);
        parents.push_back(currentNodeId);
	      currentNodeId = t.size() - 1;
        firsts.push_back(false);
        seconds.push_back(true);
	    }
	  }
	}

  return t;
}

void traverse_and_mark_helper(const trie& t, const string& pattern, vector<bool>& marks) {
	int trie_pos = 0;
	int pattern_pos = 0;
  marks[0] = true;
	for (pattern_pos; pattern_pos < pattern.size(); pattern_pos++) {
    if (t[trie_pos].find(pattern[pattern_pos]) != t[trie_pos].end()) {
      trie_pos = t[trie_pos].find(pattern[pattern_pos])->second;
      marks[trie_pos] = true;
    } else {
      break;
    }
	}
}

void traverse_and_mark(const trie& t, const string& pattern, vector<bool>& marks) {
	for (int i = 0; i < pattern.size(); i++) {
		string sub = pattern.substr(i);
		traverse_and_mark_helper(t, sub, marks);
	}
}

int do_bfs(const trie& t, vector<bool>& first_marks, vector<bool>& second_marks) {
  int trie_pos = 0;
  queue<int> Q;
  Q.push(trie_pos);
  while (!Q.empty()) {
    int pos = Q.front();
    Q.pop();
    for (auto& e : t[pos]) {
      if (first_marks[e.second]) {
        if (second_marks[e.second]) {
          Q.push(e.second);
        } else {
          return e.second;
        }
      }
    }
  }

  return 0;
}

string solve (string p, string q)
{
  string result;
  vector<int> parents;

	//trie t = build_trie(p + "#" + q + "$", parents);
  vector<bool> firsts;
  vector<bool> seconds;
  trie t = build_trie(p, q, parents, firsts, seconds);

	//traverse_and_mark(t, p, firsts);
	//traverse_and_mark(t, q, seconds);

  int last_pos = do_bfs(t, firsts, seconds);

#if DEBUG
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << " ";
			if (firsts[i]) {
				cout << "1";
			} else {
				cout << ".";
			}
			if (seconds[i]) {
				cout << "2";
			} else {
				cout << ".";
			}
			cout << endl;
    }
  }
#endif

  //do_dfs(t, 0, last_pos, result);
  int parent = parents[last_pos];
  int child = last_pos;
  bool found = true;
  result.clear();
  while (found && parent != -1) {
    found = false;
    for (auto& e : t[parent]) {
      if (e.second == child) {
        result = e.first + result;
        child = parent;
        parent = parents[child];
        found = true;
      }
    }
  }

	return result;
}

int main (void)
{
	string p;
	cin >> p;
	string q;
	cin >> q;

	string ans = solve (p, q);

	cout << ans << endl;

	return 0;
}
