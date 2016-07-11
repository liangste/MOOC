#include <cstdio>
#include <string>
#include <iostream>

using namespace std;


// Splay tree implementation

// Vertex of a splay tree
struct Vertex {
	int key;
	// Sum of all the keys in the subtree - remember to update
	// it after each operation that changes the tree.
	long long sum;
	Vertex* left;
	Vertex* right;
	Vertex* parent;

	Vertex(int key, long long sum, Vertex* left, Vertex* right, Vertex* parent)
	: key(key), sum(sum), left(left), right(right), parent(parent) {}
};

Vertex* g_root = NULL;

void pre_traversal(Vertex*& root)
{
	Vertex* v = root;
	if (v == NULL)
		return;

	cout << "(" << v->key << "," << v->sum << ") " ;
	pre_traversal(v->left);
	pre_traversal(v->right);
}

void do_pre_traversal()
{
	cout << "doing pre-order traversal " << endl;
	pre_traversal(g_root);
	cout << "done" << endl;
}

void update(Vertex* v) {
	if (v == NULL) return;
	v->sum = v->key + (v->left != NULL ? v->left->sum : 0ll) + (v->right != NULL ? v->right->sum : 0ll);
	if (v->left != NULL) {
		v->left->parent = v;
	}
	if (v->right != NULL) {
		v->right->parent = v;
	}
}

void small_rotation(Vertex* v) {
	Vertex* parent = v->parent;
	if (parent == NULL) {
		return;
	}
	Vertex* grandparent = v->parent->parent;
	if (parent->left == v) {
		Vertex* m = v->right;
		v->right = parent;
		parent->left = m;
	} else {
		Vertex* m = v->left;
		v->left = parent;
		parent->right = m;
	}

	// update order statistics
	update(parent);
	update(v);
	v->parent = grandparent;
	if (grandparent != NULL) {
		if (grandparent->left == parent) {
			grandparent->left = v;
		} else {
			grandparent->right = v;
		}
	}
}

void big_rotation(Vertex* v) {
	if (v->parent->left == v && v->parent->parent->left == v->parent) {
		// Zig-zig
		small_rotation(v->parent);
		small_rotation(v);
	} else if (v->parent->right == v && v->parent->parent->right == v->parent) {
		// Zig-zig
		small_rotation(v->parent);
		small_rotation(v);
	} else {
		// Zig-zag
		small_rotation(v);
		small_rotation(v);
	}
}

// Makes splay of the given vertex and makes
// it the new root.
void splay(Vertex*& root, Vertex* v) {
	if (v == NULL) return;
	while (v->parent != NULL) {
		if (v->parent->parent == NULL) {
			small_rotation(v);
			break;
		}
		big_rotation(v);
	}
	root = v;
}

// Searches for the given key in the tree with the given root
// and calls splay for the deepest visited node after that.
// If found, returns a pointer to the node with the given key.
// Otherwise, returns a pointer to the node with the smallest
// bigger key (next value in the order).
// If the key is bigger than all keys in the tree,
// returns NULL.
Vertex* find(Vertex*& root, int key) {
	Vertex* v = root;
	Vertex* last = root;
	Vertex* next = NULL;
	while (v != NULL) {
		if (v->key >= key && (next == NULL || v->key < next->key)) {
			next = v;
		}
		last = v;
		if (v->key == key) {
			break;
		}
		if (v->key < key) {
			v = v->right;
		} else {
			v = v->left;
		}
	}
	splay(root, last);
	return next;
}

Vertex* left_descendent(Vertex*& node) {
	if (node->left == NULL)
		return node;
	else
		return left_descendent(node->left);
}

Vertex* right_ancestor(Vertex*& node) {
	if (node->parent == NULL)
		return NULL;

	if (node->key < node->parent->key)
		return node->parent;
	else
		return right_ancestor(node->parent);
}

// return the next node, or NULL if no such node exists
Vertex* get_next(Vertex*& node) {
	if (node->right != NULL)
		return left_descendent(node->right);
	else
		return right_ancestor(node);
}

void split(Vertex* root, int key, Vertex*& left, Vertex*& right) {
	right = find(root, key);
	splay(root, right);
	if (right == NULL) {
		left = root;
		return;
	}
	left = right->left;
	right->left = NULL;
	if (left != NULL) {
		left->parent = NULL;
	}
	update(left);
	update(right);
}

Vertex* merge(Vertex* left, Vertex* right) {
	if (left == NULL) return right;
	if (right == NULL) return left;
	Vertex* min_right = right;
	while (min_right->left != NULL) {
		min_right = min_right->left;
	}
	splay(right, min_right);
	right->left = left;
	update(right);
	return right;
}

void promote(Vertex*& old, Vertex*& newer) {
	Vertex* oldold = old;
	old->key = newer->key;
	old->sum = newer->sum;
	old->left = newer->left;
	old->right = newer->right;
}

// assume node is at root already
void delete_vertex(Vertex*& node) {
	if (node->parent) {
		cout << "SOMETHING IS WRONG, deleting a vertex that still has parent" << endl;
		return;
	}

	Vertex* promoted = NULL;

	if (node->right == NULL) {
		if (node->left !=NULL) {
			node->left->parent = NULL;
		}
		g_root = node->left;
	} else {
		promoted = get_next(node);
		promoted->left = node->left;
		if (node->left != NULL) {
			node->left->parent = promoted;
		}
		//splay(g_root, promoted);
		promoted->parent = NULL;
		g_root = promoted;
	}
	update(g_root);
}

class Rope {
	std::string s;
public:
	Rope(const std::string &s) : s(s) {
	}

	void process_naive( int i, int j, int k ) {
		// Replace this code with a faster implementation
		std::string t = s.substr(0, i) + s.substr(j + 1);
		s = t.substr(0, k) + s.substr(i, j - i + 1) + t.substr(k);
	}

	void process( int i, int j, int k ) {
	}

	std::string result() {
		return s;
	}
};

int main() {
	std::ios_base::sync_with_stdio(0);
	std::string s;
	std::cin >> s;
	Rope rope(s);
	int actions;
	std::cin >> actions;
				for (int action_index = 0; action_index < actions; ++action_index) {
								int i, j, k;
		std::cin >> i >> j >> k;
		rope.process(i, j, k);
	}
	std::cout << rope.result() << std::endl;
}
