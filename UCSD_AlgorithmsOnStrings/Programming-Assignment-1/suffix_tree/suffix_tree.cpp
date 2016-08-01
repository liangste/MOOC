#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

struct Node {
	int idx; // to keep track of beginning string index if leaf, or -1
	map<string, unsigned> edges;
};

class SuffixTree {
	vector<Node> tree;
	// insert string subStr into SuffixTree at node index nodeIdx
	void _Insert(string subStr, unsigned nodeIdx) {
		Node& node = tree[nodeIdx];

		map<string, unsigned>::iterator it;
		for (auto& e : node.edges) {
			//cout << "check edge " << e.first << endl;
			unsigned u;
			for (u = 0; u < e.first.size() && u < subStr.size() && e.first[u] == subStr[u]; u++) {}
			if (u > 0) {
				string originalString = e.first;
				unsigned originalIndex = e.second;

				Node nonLeafNode;
				Node& originalNode = tree[originalIndex];

				string commonString = subStr.substr(0, u);
				string edgeString = originalString.substr(u);
				string toInsertSubstr = subStr.substr(u);

				// check wether original index points to a non-leaf node, in which case
				// we just need to insert remaining string to this node
				if (edgeString.size() == 0) {
					_Insert(toInsertSubstr, originalIndex);
					return;
				}

				// otherwise we need to refactor and extend the current edge
				node.edges.erase(originalString);

				// insert a new node
				nonLeafNode.edges[edgeString] = originalIndex;
				unsigned new_idx = tree.size();
				node.edges[commonString] = new_idx;
				tree.push_back(nonLeafNode);

				_Insert(toInsertSubstr, new_idx);
				return;
			}
		}

		// if we didn't have to split any edges
		Node newNode;
		tree[nodeIdx].edges[subStr] = tree.size();
		tree.push_back(newNode);
	}
public:
	SuffixTree(const string& inputStr) {
		Node root;
		tree.push_back(root);
		for (unsigned u = 0; u < inputStr.size(); u++) {
			//cout << "inserting " << inputStr.substr(u) << endl;
			_Insert(inputStr.substr(u), 0);
			//PrintTree();
		}
		//PrintTree();
	}

	void GetLabelEdges(vector<string>& edges) {
		for (unsigned u = 0; u < tree.size(); u++) {
			for (auto& e : tree[u].edges) {
				edges.push_back(e.first);
			}
		}
	}

	void PrintTree() {
		cout << "++++ Printing Tree ++++" << endl;
		for (unsigned u = 0; u < tree.size(); u++) {
			cout << "node[" << u << "]" << endl;
			if (tree[u].edges.empty()) {
				cout << "\tEmpty" << endl;
			}
			for (auto& e : tree[u].edges) {
				cout << "\t" << e.first << " -> " << "node[" << e.second << "]" << endl;
			}
		}
		cout << "++++ Printing Tree - done ++++" << endl;
	}
};

// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding
// substrings of the text) in any order.
vector<string> ComputeSuffixTreeEdges(const string& text) {
	vector<string> result;
	SuffixTree suffixTree(text);
	suffixTree.GetLabelEdges(result);
	return result;
}

int main() {
	string text;
	cin >> text;
	vector<string> edges = ComputeSuffixTreeEdges(text);
	for (int i = 0; i < edges.size(); ++i) {
		cout << edges[i] << endl;
	}
	return 0;
}
