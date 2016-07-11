#include <cstdio>
#include <iostream>

#define DEBUG 0

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

// Code that uses splay tree to solve the problem

void insert(int x) {
#if DEBUG
	cout << "### add " << x << endl;
#endif
	Vertex* left = NULL;
	Vertex* right = NULL;
	Vertex* new_vertex = NULL;
	split(g_root, x, left, right);
	if (right == NULL || right->key != x) {
		new_vertex = new Vertex(x, x, NULL, NULL, NULL);
	}
	g_root = merge(merge(left, new_vertex), right);
}

void erase(int x) {
#if DEBUG
	cout << "### del " << x << endl;
#endif
	// Implement erase yourself
	Vertex* target = find(g_root, x);
	Vertex* next = NULL;

	if (target == NULL) {
		return;
	}
	if (target->key != x) {
		return;
	}

	next = get_next(target);
	splay(g_root, next);
	splay(g_root, target);
	delete_vertex(target);
}

bool find(int x) {
#if DEBUG
	cout << "### find " << x << endl;
#endif
	// Implement find yourself

	Vertex* ret = find(g_root, x);

	if (ret == NULL)
		return false;

	return ret->key == x;
}

long long sum(int from, int to) {
#if DEBUG
	cout << "### sum from " << from << " to " << to << endl;
#endif
	Vertex* left = NULL;
	Vertex* middle = NULL;
	Vertex* right = NULL;
	split(g_root, from, left, middle);
	split(middle, to + 1, middle, right);
	long long ans = 0ll;
	// Complete the implementation of sum

	// sum all elements in middle
	if (middle != NULL) {
		update(middle);
		ans = middle->sum;
	}

	g_root = merge(merge(left, middle), right);

	return ans;
}

const int MODULO = 1000000001;

int main(){
	int n;
	scanf("%d", &n);
	int last_sum_result = 0;
	for (int i = 0; i < n; i++) {
		char buffer[10];
		scanf("%s", buffer);
		char type = buffer[0];
		switch (type) {
			case '+' : {
				int x;
				scanf("%d", &x);
				insert((x + last_sum_result) % MODULO);
			} break;
			case '-' : {
				int x;
				scanf("%d", &x);
				erase((x + last_sum_result) % MODULO);
			} break;
			case '?' : {
				int x;
				scanf("%d", &x);
				printf(find((x + last_sum_result) % MODULO) ? "Found\n" : "Not found\n");
			} break;
			case 's' : {
				int l, r;
				scanf("%d %d", &l, &r);
				long long res = sum((l + last_sum_result) % MODULO, (r + last_sum_result) % MODULO);
				printf("%lld\n", res);
				last_sum_result = int(res % MODULO);
			}
		}

#if DEBUG
		do_pre_traversal();
#endif
	}
	return 0;
}
