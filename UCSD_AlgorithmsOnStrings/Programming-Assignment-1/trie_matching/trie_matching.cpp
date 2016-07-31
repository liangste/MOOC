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

int to_idx (char letter)
{
	switch (letter)
	{
		case 'A': return 0; break;
		case 'C': return 1; break;
		case 'G': return 2; break;
		case 'T': return 3; break;
		default: return -1;
	}
}

bool prefix_trie_matching(string text,
													const vector<Node>& trie,
													vector<int>& result) {
	uint32_t symbolIdx = 0;
	char symbol = text[symbolIdx];
	uint32_t v = 0;
	while(1) {
		if (trie[v].isLeaf()) {
			return true;
		} else if (to_idx(symbol) != NA && trie[v].next[to_idx(symbol)] != NA) {
			symbolIdx++;
			v = trie[v].next[to_idx(symbol)];
			symbol = text[symbolIdx];
		} else {
			return false;
		}
	}
}

void trie_matching(string text,
									 const vector<Node>& trie,
									 vector<int>& result) {
	for (uint32_t u = 0; u < text.size(); u++) {
		string prefix = text.substr(u);
		if (prefix_trie_matching(prefix, trie, result)) {
			result.push_back(u);
		}
	}
}

void print_trie(const vector<Node>& trie) {
	for (int i = 0; i < trie.size(); i++) {
		cout << "Node[" << i << "] ";
		cout << "(A,C,G,T) = (";
		cout << trie[i].next[0] << ",";
		cout << trie[i].next[1] << ",";
		cout << trie[i].next[2] << ",";
		cout << trie[i].next[3];
		cout << ")"  << endl;
	}
}

vector<Node> build_trie(vector<string>& patterns) {
	vector<Node> trie;
	Node root;
	trie.push_back(root);
	uint32_t currentNodeId;

	for (auto& p : patterns) {
		currentNodeId = 0;
		for (int i = 0; i < p.size(); i++) {
			char symbol = p[i];
			// if there's an edge leading to symbol
			if (to_idx(symbol) != NA &&
					trie[currentNodeId].next[to_idx(symbol)] != NA) {
				currentNodeId = trie[currentNodeId].next[to_idx(symbol)];
			} else { // construct a new Node
				Node newNode;
				trie[currentNodeId].next[to_idx(symbol)] = trie.size();
				trie.push_back(newNode);
				currentNodeId = trie.size() - 1;
			}
		}
	}

	return trie;
}

vector<int> solve(string text,
									int n,
									vector <string> patterns) {
	vector<Node> trie;
	vector<int> result;

	// first we build a trie structure using all pattern in patterns
	trie = build_trie(patterns);
	//print_trie(trie);

	// perform trie-matching and store results
	trie_matching(text, trie, result);

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
