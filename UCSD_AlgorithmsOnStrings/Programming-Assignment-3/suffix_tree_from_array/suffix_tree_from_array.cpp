#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

using namespace std;

#define DEBUG 1
#define INVALID -1

unsigned Char2Idx(char c) {
	switch(c) {
		case '$':
			return 0;
		case 'A':
			return 1;
		case 'C':
			return 2;
		case 'G':
			return 3;
		case 'T':
		default:
			return 4;
	}
}

struct STNode {
	int parent;
	int children[5] = {INVALID, INVALID, INVALID, INVALID, INVALID};
	int depth;
	int start;
	int end;

	STNode(int p, int d, int s, int e)
		: parent(p), depth(d), start(s), end(e) {};
};

struct Edge {
	// The ending node of this edge.
	int node;
	// Starting position of the substring of the text
	// corresponding to the label of this edge.
	int start;
	// Position right after the end of the substring of the text
	// corresponding to the label of this edge.
	int end;

	Edge(int node_, int start_, int end_)
		: node(node_), start(start_), end(end_) {}
	Edge(const Edge& e) : node(e.node), start(e.start), end(e.end) {}
};

void CreateNewLeaf(vector<STNode>& tree, int curNodeIndex, const string& text, int suffix) {
	STNode& curNode = tree[curNodeIndex];
	STNode leaf(curNodeIndex, text.size() - suffix, suffix + curNode.depth, text.size());
	tree.push_back(leaf);
	int leafIdx = tree.size() - 1;
	tree[curNodeIndex].children[Char2Idx(text[suffix + curNode.depth])] = leafIdx;
}

int BreakEdge(vector<STNode>& tree, int curNodeIndex, const string& text, int start, int offset) {
	cout << "BreakEdge at " << curNodeIndex << " start=" << start << " offset=" << offset << endl;
	char startChar = text[start];
	char midChar = text[start + offset];
	STNode& curNode = tree[curNodeIndex];
	int edgeEnd = tree[curNode.children[Char2Idx(startChar)]].end;
	STNode midNode(curNodeIndex, curNode.depth + offset, start + offset, edgeEnd);
	midNode.children[Char2Idx(midChar)] = curNode.children[Char2Idx(startChar)];
	tree.push_back(midNode);
	int midNodeIndex = tree.size() - 1;
	tree[midNodeIndex].children[Char2Idx(midChar)] = curNode.children[Char2Idx(startChar)];
	cout << "update " << curNode.children[Char2Idx(startChar)] << " parent to " << midNodeIndex << endl;
	tree[curNode.children[Char2Idx(startChar)]].parent = midNodeIndex;
	// update start
	tree[curNodeIndex].children[Char2Idx(startChar)] = midNodeIndex;
	cout << "BrokeEdge ... mid=" << midNodeIndex << endl;
	return midNodeIndex;
}

map<int, vector<Edge> > SuffixTreeFromSuffixArray(const vector<int>& suffix_array,
																									const vector<int>& lcp_array,
																									const string& text) {
	map<int, vector<Edge> > edges;
	vector<STNode> tree;
	STNode root(-1, 0, -1, -1);
	tree.push_back(root);
	int lcpPrev = 0;
	int curNodeIndex = 0;

	for (int i = 0; i < text.size(); i++) {
		int suffix = suffix_array[i];
		curNodeIndex = tree.size() - 1;
		while (tree[curNodeIndex].depth > lcpPrev) {
			curNodeIndex = tree[curNodeIndex].parent;
		}
		STNode& curNode = tree[curNodeIndex];
		if (curNode.depth == lcpPrev) {
			CreateNewLeaf(tree, curNodeIndex, text, suffix);
		} else {
			int edgeStart = suffix_array[i] - curNode.depth;
			int offset = lcpPrev - curNode.depth;
			int midNode = BreakEdge(tree, curNodeIndex, text, edgeStart, offset);
			CreateNewLeaf(tree, midNode, text, suffix);
			for (int i = 1; i < 5; i++) {
				cout << " " << tree[curNodeIndex].children[i];
			} cout << endl;
			for (int i = 1; i < 5; i++) {
				cout << " " << tree[midNode].children[i];
			} cout << endl;
		}
		if (i < (text.size() - 1)) {
			lcpPrev = lcp_array[i];
		}
	}

	// TODO print tree for debugging

	// copy over all nodes to Edge rep
	for (int i = 0; i < tree.size(); i++) {
		cout << "node " << i;;
		cout << ", parent=" << tree[i].parent;
		cout << ", start=" << tree[i].start;
		cout << ", end=" << tree[i].end;
		if (tree[i].end - tree[i].start)
		{
			cout << ", substr=" << text.substr(tree[i].start, tree[i].end - tree[i].start);
		}
		cout << endl;

		STNode& p = tree[i];
		for (int j = 0; j < 5; j++) {
			if (p.children[j] != INVALID) {
				cout << "\tc -> " << p.children[j] << endl;
				STNode& c = tree[p.children[j]];
				Edge edge(p.children[j], c.start, c.end);
				edges[i].push_back(edge);
			}
		}
	}
	return edges;
}


int main() {
	char buffer[200001];
	scanf("%s", buffer);
	string text = buffer;
	vector<int> suffix_array(text.length());
	for (int i = 0; i < text.length(); ++i) {
		scanf("%d", &suffix_array[i]);
	}
	vector<int> lcp_array(text.length() - 1);
	for (int i = 0; i + 1 < text.length(); ++i) {
		scanf("%d", &lcp_array[i]);
	}
	// Build the suffix tree and get a mapping from
	// suffix tree node ID to the list of outgoing Edges.
	map<int, vector<Edge> > tree = SuffixTreeFromSuffixArray(suffix_array, lcp_array, text);
	printf("%s\n", buffer);
	// Output the edges of the suffix tree in the required order.
	// Note that we use here the contract that the root of the tree
	// will have node ID = 0 and that each vector of outgoing edges
	// will be sorted by the first character of the corresponding edge label.
	//
	// The following code avoids recursion to avoid stack overflow issues.
	// It uses a stack to convert recursive function to a while loop.
	// The stack stores pairs (node, edge_index).
	// This code is an equivalent of
	//
	//    OutputEdges(tree, 0);
	//
	// for the following _recursive_ function OutputEdges:
	//
	// void OutputEdges(map<int, vector<Edge> > tree, int node_id) {
	//   const vector<Edge>& edges = tree[node_id];
	//   for (int edge_index = 0; edge_index < edges.size(); ++edge_index) {
	//     printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
	//     OutputEdges(tree, edges[edge_index].node);
	//   }
	// }
	//
	vector<pair<int, int> > stack(1, make_pair(0, 0));
	while (!stack.empty()) {
		pair<int, int> p = stack.back();
		stack.pop_back();
		int node = p.first;
		int edge_index = p.second;
		if (!tree.count(node)) {
			continue;
		}
		const vector<Edge>& edges = tree[node];
		if (edge_index + 1 < edges.size()) {
			stack.push_back(make_pair(node, edge_index + 1));
		}
		printf("%d %d\n", edges[edge_index].start, edges[edge_index].end);
		stack.push_back(make_pair(edges[edge_index].node, 0));
	}
	return 0;
}
