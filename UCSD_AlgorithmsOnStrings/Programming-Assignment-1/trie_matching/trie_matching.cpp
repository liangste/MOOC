#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int const Letters =    4;
int const NA      =   -1;

struct Node {
	int next[Letters];

	Node() {fill(next, next + Letters, NA);}

	bool isLeaf() const {
	    return (next[0] == NA && next[1] == NA && next[2] == NA && next[3] == NA);
	}
};

int letterToIndex (char letter)
{
	switch (letter)
	{
		case 'A': return 0; break;
		case 'C': return 1; break;
		case 'G': return 2; break;
		case 'T': return 3; break;
		default: assert (false); return -1;
	}
}

vector<int> solve(string text, int n, vector <string> patterns) {
	vector<Node> trie;
	vector<int> result;

	Node root;
	trie.push_back(root);
	int currentNodeId;

	// make a trie structure from suffixes to text
	for (int i = 0; i < text.length(); i++) {
		string suffix = text.substr(i);
		currentNodeId = 0;
		cout << "suffix = " << suffix << endl;
		for (int j = 0; j < suffix.length(); j++) {
			char currentSymbol = suffix[j];
			if (trie[currentNodeId].isLeaf()) {
				Node newNode;
        trie[currentNodeId].next[letterToIndex(currentSymbol)] = trie.size();
        trie.push_back(newNode);
        currentNodeId = trie.size() - 1;
      } else {
				currentNodeId = trie[currentNodeId].next[letterToIndex(currentSymbol)];
      }
		}
	}

	cout << "constucted trie vector of size = " << trie.size() << endl;

	// for each pattern in patterns, traverse the trie
	unsigned searchIndex;
	for (auto& p : patterns) {
		searchIndex = 0;
		bool inTrie = true;
		for (unsigned u = 0; u < p.size(); u++) {
			cout << "looking for " << p[u] << endl;
			if (trie[searchIndex].next[letterToIndex(p[u])] != NA) {
				cout << "1" << endl;
				searchIndex = trie[searchIndex].next[letterToIndex(p[u])];
			} else {
				inTrie = false;
			}
			if (u == (p.size() - 1) && inTrie) {
				cout << "found" << endl;
			}
		}
	}
	searchIndex = 0;
	cout << "next[A] = " << trie[searchIndex].next[0] << endl;
	cout << "next[C] = " << trie[searchIndex].next[1] << endl;
	cout << "next[G] = " << trie[searchIndex].next[2] << endl;
	cout << "next[T] = " << trie[searchIndex].next[3] << endl;

	return result;
}

int main (void)
{
	string text;
	cin >> text;

	int n;
	cin >> n;

	vector<string> patterns(n);
	for (int i = 0; i < n; i++)
	{
		cin >> patterns[i];
	}

	vector <int> ans;
	ans = solve(text, n, patterns);

	for (int i = 0; i < (int) ans.size (); i++)
	{
		cout << ans[i];
		if (i + 1 < (int) ans.size ())
		{
			cout << " ";
		}
		else
		{
			cout << endl;
		}
	}

	return 0;
}
