#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

#define DEBUG 0

// Splay tree implementation

// Vertex of a splay tree
struct Vertex {
	char c; // character on this node
	int left_sum; // number of character under left subtree
	int sum; // number of characters under left + right subtrees
	Vertex* left;
	Vertex* right;
	Vertex* parent;

	Vertex(char chara, Vertex* left, Vertex* right)
		: c(chara)
		, left_sum(0)
		, sum(1)
		, left(left)
		, right(right)
		, parent(NULL) {
		if (left != NULL) {
			left->parent = this;
			left_sum = left->sum;
			sum += left->sum;
		}
		if (right != NULL) {
			right->parent = this;
			sum += right->sum;
		}
	}
};

Vertex* g_root = NULL;

void inorder_traversal(Vertex* v) {
	if (v == NULL)
		return;

	inorder_traversal(v->left);
	cout << "(" << v->c << "," << v->left_sum << "," << v->sum << ") " ;
	inorder_traversal(v->right);
}

void pre_traversal(Vertex*& root)
{
	Vertex* v = root;
	if (v == NULL)
		return;

	cout << "(" << v->c << "," << v->left_sum << "," << v->sum << ") " ;
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
	v->left_sum = 0;
	v->sum = 1; // v itself has 1 character
	if (v->left != NULL) {
		v->left->parent = v;
		v->left_sum = v->left->sum;
		v->sum += v->left->sum;
	}
	if (v->right != NULL) {
		v->right->parent = v;
		v->sum += v->right->sum;
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

// Searches for the given index in the tree with the given root
// and calls splay for the deepest visited node after that.
// If found, returns a pointer to the node with the given index.
// Otherwise, returns NULL.
// i.e. if we have a tree built up that spells "algorithms", and we call
//   find(..., 0) on it, it should return a pointer to the node with character
//   'a'
Vertex* find(Vertex*& root, int index) {
	Vertex* v = root;
	Vertex* last = root;

	if (index < 0) {
		return NULL;
	}

	if (root == NULL) {
		return NULL;
	}

	if (index >= root->sum) { // because of 0-index, largest index is root->sum - 1
		return NULL;
	}

	while (v != NULL) {
		last = v;
		if (v->left_sum == index) {
			// found .. stored as last
			break;
		}
		if (v->left_sum < index) {
			index -= (v->left_sum + 1); // account for v itself
			v = v->right;
		} else { // v->left_sum > index
			v = v->left;
		}
	}
	splay(root, last);
	return last;
}

/*
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

//
return the next node, or NULL if no such node exists
Vertex* get_next(Vertex*& node) {
	if (node->right != NULL)
		return left_descendent(node->right);
	else
		return right_ancestor(node);
}
*/

void split(Vertex* root, int index, Vertex*& left, Vertex*& right) {
	right = find(root, index);
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

/*
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
*/

Vertex* ropefy(const std::string& s) {
	Vertex* root = NULL;
	Vertex* newV = NULL;

	for (int i = 0; i < s.size(); i++) {
		newV = new Vertex(s[i], NULL, NULL);
		root = merge(root, newV);
	}

	return root;
}

void to_string_helper(Vertex* v, string& str) {
	if (v == NULL)
		return;
	to_string_helper(v->left, str);
	str += v->c;
	to_string_helper(v->right, str);
}

string tostring(Vertex* v) {
	string ret_string = "";
	to_string_helper(v, ret_string);
	return ret_string;
}

class Rope {
	std::string s;
	Vertex* rope_vertex;
public:
	Rope(const std::string &s) : s(s), rope_vertex(NULL) {
		rope_vertex = ropefy(s);
	}

	void process_naive( int i, int j, int k ) {
		// Replace this code with a faster implementation
		std::string t = s.substr(0, i) + s.substr(j + 1);
		s = t.substr(0, k) + s.substr(i, j - i + 1) + t.substr(k);
	}

	void process( int i, int j, int k ) {
		Vertex* left, *middle, *right;
		split(rope_vertex, i, left, right); // i
		split(right, j - i + 1, middle, right); // j
		Vertex* left_right = merge(left, right);
		split(left_right, k, left, right); // k
		middle = merge(left, middle);
		rope_vertex = merge(middle, right);
	}

	std::string result() {
		return tostring(rope_vertex);
	}
};

void do_dev_tests() {
	cout << "Doing development tests" << endl;

	Vertex* a = new Vertex('A', NULL, NULL);
	Vertex* g = new Vertex('G', NULL, NULL);
	Vertex* l = new Vertex('L', a, g);
	Vertex* r = new Vertex('R', NULL, NULL);
	Vertex* o = new Vertex('O', l, r);
	Vertex* t = new Vertex('T', NULL, NULL);
	Vertex* s = new Vertex('S', NULL, NULL);
	Vertex* m = new Vertex('M', NULL, s);
	Vertex* h = new Vertex('H', t, m);
	Vertex* i = new Vertex('I', o, h);

	//inorder_traversal(i);
	//update(i);
	//inorder_traversal(i);

	splay(i, r);
	inorder_traversal(i);

	Vertex* last_vertex = i;
	Vertex* test_vertex = i;

	for (int xxx = -1 ; xxx < 11; xxx++) {
		cout << "char at index " << xxx;
		test_vertex = find(last_vertex, xxx);
		if (test_vertex == NULL) {
			cout << "... EMPTY" << endl;
			last_vertex = i;
		} else {
			cout << "... " << test_vertex->c << endl;
			inorder_traversal(test_vertex);
			last_vertex = test_vertex;
		}
	}

	Vertex* my_name = ropefy("Hello my name is Stephen");
	string my_name_str = tostring(my_name);
	cout << "'" << my_name_str << "'" << endl;

	Vertex* test_string = ropefy("hlelowrold");
	Vertex* left, *middle, *right;
	split(test_string, 1, left, right); // i
	cout << "left: " << tostring(left) << endl;
	cout << "right: " << tostring(right) << endl;
	split(right, 1, middle, right); // j
	cout << "middle: " << tostring(middle) << endl;
	cout << "right: " << tostring(right) << endl;
	Vertex* left_right = merge(left, right);
	cout << "left_right: " << tostring(left_right) << endl;
	split(left_right, 2, left, right); // k
	middle = merge(left, middle);
	middle = merge(middle, right);
	cout << "middle: " << tostring(middle) << endl;
}

int main() {
#if DEBUG
	do_dev_tests();
	return 0;
#endif
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
