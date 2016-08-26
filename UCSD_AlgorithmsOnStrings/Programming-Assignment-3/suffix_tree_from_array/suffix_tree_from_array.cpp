#include <algorithm>
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

using namespace std;

#define DEBUG 1

// Data structure to store edges of a suffix tree.
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

// simple data structure to store nodes of a suffix tree
struct Node {
  // back-pointer to index of parent
  int parent;
  // string depth
  int depth;

  Node(int p, int d) : parent(p), depth(d) {}
};

// create a leaf
void CreateNewLeaf(vector<Node>& nodes,
                   int curNodeIndex,
                   map<int, vector<Edge> >& edges,
                   const std::string& text,
                   int suffix) {
#ifdef DEBUG
  cout << "CreateNewLeaf" << endl;
#endif
  Node newNode(curNodeIndex, text.size() - suffix);
  nodes.push_back(newNode);
  int newNodeIndex = nodes.size() - 1;
  Edge newEdge(newNodeIndex, suffix + nodes[curNodeIndex].depth, text.size());
  edges[curNodeIndex].push_back(newEdge);
}

// break node at given node, return index to new node
int BreakEdge(vector<Node>& nodes,
              int curNodeIndex,
              map<int, vector<Edge> >& edges,
              const std::string& text,
              int start,
              int offset) {
#ifdef DEBUG
  cout << "BreakEdge" << endl;
#endif
  char startChar = text[start];
  char midChar = text[start + offset];

  Node midNode(curNodeIndex, nodes[curNodeIndex].depth + offset);
  nodes.push_back(midNode);
  int midNodeIndex = nodes.size() - 1;

  int childNode = -1;
  int childStart, childEnd;

  cout << "looking for startChar " << startChar << " and midChar " << midChar << endl;
  cout << "examining edges from node idx = " << curNodeIndex << endl;
  // find existing edge from curNodeIndex to child, change child to midNode
  for (auto& edge : edges[curNodeIndex]) {
    cout << "  " << "(node, start, end) = ";
    cout << edge.node << ", " << edge.start << ", " << edge.end;
    cout << " [" << text[edge.start] << "]" << endl;
    if (text[edge.start] == startChar) {
      childNode = edge.node;
      childStart = edge.start;
      childEnd = edge.end;
      edge.node = midNodeIndex;
      // edge.start = start + offset
      edge.end = start + offset;
      cout << "  new " << "(node, start, end) = ";
      cout << edge.node << ", " << edge.start << ", " << edge.end;
      cout << " [" << text[edge.start] << "]" << endl;
      break;
    }
  }

  if (-1 == childNode) {
    cout << "massive fail" << endl;
  } else {
    Edge newEdge(childNode, childStart + offset, childEnd);
    edges[midNodeIndex].push_back(newEdge);
    nodes[childNode].parent = midNodeIndex;
  }

  return midNodeIndex;
}

// Build suffix tree of the string text given its suffix array suffix_array
// and LCP array lcp_array. Return the tree as a mapping from a node ID
// to the vector of all outgoing edges of the corresponding node. The edges in the
// vector must be sorted in the ascending order by the first character of the edge label.
// Root must have node ID = 0, and all other node IDs must be different
// nonnegative integers.
//
// For example, if text = "ACACAA$", an edge with label "$" from root to a node with ID 1
// must be represented by Edge(1, 6, 7). This edge must be present in the vector tree[0]
// (corresponding to the root node), and it should be the first edge in the vector
// (because it has the smallest first character of all edges outgoing from the root).
map<int, vector<Edge> > SuffixTreeFromSuffixArray(const vector<int>& suffix_array,
                                                  const vector<int>& lcp_array,
                                                  const string& text) {

  map<int, vector<Edge> > edges;
  vector<Node> nodes;

  // Implement this function yourself
  int lcpPrev = 0;
  int curNodeIndex = 0;
  Node root = {-1, 0};
  nodes.push_back(root);

  for (int i = 0; i < text.size(); i++) {
    int suffix = suffix_array[i];

#ifdef DEBUG
    cout << "looping with suffix = " << suffix << endl;
#endif

    curNodeIndex = nodes.size() - 1;
    cout << "curNodeIndex = " << curNodeIndex << endl;

    while (nodes[curNodeIndex].depth > lcpPrev) {
      curNodeIndex = nodes[curNodeIndex].parent;
      cout << "UPDATED curNodeIndex = " << curNodeIndex << endl;
    }

    if (nodes[curNodeIndex].depth == lcpPrev) {
      CreateNewLeaf(nodes, curNodeIndex, edges, text, suffix);
    } else {
      int edgeStart = suffix_array[i - 1] + nodes[curNodeIndex].depth;
      int offset = lcpPrev - nodes[curNodeIndex].depth;
      int midNodeIndex = BreakEdge(nodes, curNodeIndex, edges, text, edgeStart, offset);
      CreateNewLeaf(nodes, midNodeIndex, edges, text, suffix);

      // need to prop up
      curNodeIndex = nodes.size() - 1;
      cout << "PROP_UP? curNodeIndex = " << curNodeIndex;
      cout << " offset = " << offset << endl;

      int child = nodes[curNodeIndex].parent;
      int parent = nodes[child].parent;
      while (parent != -1) {
        cout << "prop up parent " << parent << endl;
        for (auto& edge : edges[parent]) {
          if (child == edge.node) {
            cout << "edge from " << parent << " to " << child << endl;
            edge.start -= offset;
            edge.end -= offset;
          }
        }
        child = parent;
        parent = nodes[parent].parent;
      }

    }

    if (i < (text.size() - 1)) {
      lcpPrev = lcp_array[i];
    }

#ifdef DEBUG
    cout << "==== Printing Nodes/Edges ====" << endl;
    for (int i = 0; i < nodes.size(); i++) {
      cout << i << ":" << " .parent = " << nodes[i].parent << " .depth = " <<  nodes[i].depth << endl;
      for (auto& edge : edges[i]) {
        cout << "\t.node = " << edge.node << " .start = " << edge.start << " .end = " << edge.end << endl;
      }
    }
    cout << "==== Printing Nodes/Edges (Done )====" << endl;
#endif
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
