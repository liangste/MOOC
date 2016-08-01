#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <queue>

using namespace std;


struct Node {
	int idx; // to keep track of beginning string index if leaf, or -1
	bool first;
	bool second;
	map<string, unsigned> edges;

	Node() : idx(-1), first(false), second(false) {}
};

class SuffixTree {

	struct BFSPair {
		string str;
		unsigned idx;
	};

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
	}

	void GetSUS(vector<string>& edges) {
		for (unsigned u = 0; u < tree.size(); u++) {
			for (auto& e : tree[u].edges) {
				edges.push_back(e.first);
			}
		}
	}

	void PrintTree() {
		cout << "++++ Printing Tree ++++" << endl;
		for (unsigned u = 0; u < tree.size(); u++) {
			cout << "node[" << u << "] " << tree[u].first << " " << tree[u].second << endl;
			if (tree[u].edges.empty()) {
				cout << "\tEmpty" << endl;
			}
			for (auto& e : tree[u].edges) {
				cout << "\t" << e.first << " -> " << "node[" << e.second << "]" << endl;
			}
		}
		cout << "++++ Printing Tree - done ++++" << endl;
	}

	void LabelFirst(const std::string& string1) {
		queue<BFSPair> Q;
		BFSPair pair;
		pair.str = string1;
		pair.idx = 0;
		Q.push(pair);

		while(!Q.empty()) {
			BFSPair thisPair = Q.front();
			Q.pop();
			Node& node = tree[thisPair.idx];
			for (auto& e : node.edges) {
				size_t sharPos = e.first.find("#");
				string edgeString;
				if (sharPos != string::npos) {
					edgeString = e.first.substr(0, sharPos);
				} else {
					edgeString = e.first;
				}
				unsigned edgeLength = edgeString.size();
				if (edgeLength > 0 && edgeString == thisPair.str.substr(0, edgeLength)) {
					// ...
					tree[e.second].first = true;
					BFSPair newPair;
					newPair.str = thisPair.str.substr(edgeLength);
					newPair.idx = e.second;
					Q.push(newPair);
				}
			}
			node.first = true;
		}
	}

	void LabelSecond(const std::string& string2) {
		queue<BFSPair> Q;
		BFSPair pair;
		pair.str = string2;
		pair.idx = 0;
		Q.push(pair);

		while(!Q.empty()) {
			BFSPair thisPair = Q.front();
			Q.pop();
			Node& node = tree[thisPair.idx];
			for (auto& e : node.edges) {
				size_t sharPos = e.first.find("$");
				string edgeString;
				if (sharPos != string::npos) {
					edgeString = e.first.substr(0, sharPos);
				} else {
					edgeString = e.first;
				}
				unsigned edgeLength = edgeString.size();
				if (edgeLength > 0 && edgeString == thisPair.str.substr(0, edgeLength)) {
					// ...
					tree[e.second].second = true;
					BFSPair newPair;
					newPair.str = thisPair.str.substr(edgeLength);
					newPair.idx = e.second;
					Q.push(newPair);
				}
			}
			node.second = true;
		}
	}
};

string solve (string p, string q)
{
	vector<string> edges;
	string result = p;
	SuffixTree suffixTree(p + "#" + q + "$");
	for (unsigned u = 0; u < p.size(); u++) {
		string sub = p.substr(u);
		suffixTree.LabelFirst(sub);
	}
	for (unsigned u = 0; u < q.size(); u++) {
		string sub = q.substr(u);
		suffixTree.LabelSecond(sub);
	}
	suffixTree.PrintTree();
	suffixTree.GetSUS(edges);

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
