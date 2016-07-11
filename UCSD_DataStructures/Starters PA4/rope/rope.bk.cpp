#include <cstdio>
#include <string>
#include <iostream>
#include <math.h>
#include <vector>

// TODO run mega_large_test under 2.7 seconds

#define DEBUG 0

using namespace std;

typedef struct rope_node rope;

// invariants
// either data is non-empty or left and right are both non-empty
// left and right must both be set, in which case the rope_node is a
// meta-node. Otherwise it is a data node
struct rope_node {
	int l_len; // length of string under left sub-tree
	int len; // length of string under both left and right sub-trees
	int height;
	rope* left;
	rope* right;
	rope* parent;
	string data;

	// construct data-rope_node
	rope_node(const string& d)
		: l_len(d.size())
		, len(d.size())
		, height(0)
		, left(NULL)
		, right(NULL)
		, parent(NULL)
		, data(d) {
	};

	// construct meta-data-rope_node
	rope_node(rope* ll, rope* rr)
		: l_len(0)
		, len(0)
		, left(ll)
		, right(rr)
		, parent(NULL)
		, data("") {

		// update height
		int left_height = 0;
		int right_height = 0;

		if (ll != NULL) {
			l_len = ll->len;
			len = ll->len;
			ll->parent = this;
			left_height = ll->height;
		}
		if (rr != NULL) {
			len += rr->len;
			rr->parent = this;
			right_height = rr->height;
		}

		if (left_height > right_height) {
			height = left_height + 1;
		} else {
			height = right_height + 1;
		}
	};
};

// create a rope object from string
rope* ropify(const string& str);

// convert a rope to sring
void stringfy(rope*& r, string& str);

void report(rope*& r);

// print out string of rope from pos i to j
void report(rope*& r, int i, int j);

// simplify a rope structure
void reduce(rope*& r);

// recursively update stats in a bottom-up manner
void update_stats(rope*& r);

void rotate_left(rope*& r);

void rotate_right(rope*& r);

// concatenates two ropes into one, and return pointer to the new rope
rope* concat(rope*& left, rope*& right);

// split an original rope into left and right ropes at position i, character
// at position i goes to the newly formed right rope
void split(rope*& original, int i, rope*& right);

// insert substring at pos i
void insert(rope*& target, rope*& sub, int i);

rope* ropify(const string& str) {
	int len = str.size();
	int seg_len = (int) log2(len);
	if (seg_len == 0) {
		seg_len = 1;
	}
	int cur_idx = 0;
	string str_seg = "";
	rope* new_rope = NULL;
	rope* root_rope = NULL;

	while (cur_idx < len) {
		str_seg = str.substr(cur_idx, seg_len);
		new_rope = new rope(str_seg);
		root_rope = concat(root_rope, new_rope);
		cur_idx += seg_len;
	}

	return root_rope;
}

void stringfy(rope*& r, string& str) {
	// do post-order traversal

	if (r == NULL)
		return;

	stringfy(r->left, str);
	stringfy(r->right, str);
	str += r->data;
}

void report_helper(rope*& r) {
	if (r == NULL) {
		cout << "NULL" << endl;
	} else {
		cout << "(" << r->l_len << "," << r->len << ") " << r->data << endl;
		cout << "left ";
		report_helper(r->left);
		cout << "right " ;
		report_helper(r->right);
	}
}

void report(rope*& r) {
	string str = "";
	stringfy(r, str);
	cout << "Reporting string '" << str << "'" << endl;
	//cout << "Printing nodes" << endl;
	//report_helper(r);
}

void reduce(rope*& r) {
	if (r == NULL)
		return;
	reduce(r->left);
	reduce(r->right);
	if (r->right == NULL && r->left == NULL && r->data.size() == 0)
	{
		delete r;
		r = NULL;
	}
}

void update_stats(rope*& r) {
	if (r == NULL) {
		return;
	}
	if (r->left != NULL) {
		r->l_len = r->left->len;
		r->len = r->left->len;
	}
	if (r->right != NULL) {
		r->len += r->right->len;
	}
	update_stats(r->parent);
}

rope* concat(rope*& left, rope*& right) {
	if (left == NULL) return right;
	if (right == NULL) return left;
	rope* new_rope = new rope(left, right);
	reduce(new_rope);
	return new_rope;
}

void split_helper(rope*& root, int i, rope*& right) {
	if (NULL == root) {
		cout << "Cannot split string that is NULL" << endl;
		return;
	}

	if (i >= root->len || i < 0) {
		//cout << "Cannot split string with length " << root->len << " at pos " << i << endl;
		return;
	}

	if (i <= root->l_len) {
		if (root->left == NULL && root->right == NULL) {
			// this is the case where we must split a data node into 2
			if (i >= root->data.size()) {
				cout << "Something is very wrong" << endl;
				return;
			}
			string split_right = root->data.substr(i);
			root->data = root->data.substr(0, i);
			root->l_len = root->len = root->data.size();
			rope* new_rope = new rope(split_right);
			right = concat(new_rope, right);

			// TODO need to update tree stats
			update_stats(root);
		} else {
			right = concat(root->right, right);
			root->right = NULL;
			split_helper(root->left, i, right);

			// TODO need to update tree stats
			update_stats(root);
		}
	} else {
		split_helper(root->right, i - root->l_len, right);
	}
}

void split(rope*& root, int i, rope*& right) {
	split_helper(root, i, right);
	reduce(root);
	reduce(right);
}

void insert(rope*& target, rope*& sub, int i) {
	rope* right = NULL;
	split(target, i , right);
	target = concat(target, sub);
	target = concat(target, right);
}

void rope_dev_test() {
	cout << "Starting rope development test" << endl;

	// test concat()
	rope* hello = new rope("hello_");
	rope* my = new rope("my_");
	rope* name = new rope("name_");
	rope* is_simon = new rope("is_Simon");
	hello = concat(hello, my);
	hello = concat(hello, name);
	hello = concat(hello, is_simon);
	//report(hello);

	// test split()
	rope* e = new rope("Hello_");
	rope* f = new rope("my_");
	rope* j = new rope("na");
	rope* k = new rope("me_i");
	rope* m = new rope("s");
	rope* n = new rope("_Simon");
	rope* c = new rope(e, f);
	rope* g = new rope(j, k);
	rope* h = new rope(m, n);
	rope* d = new rope(g, h);
	rope* b = new rope(c, d);
	rope* a = new rope(b, NULL);

	string expected_string, left_string, right_string, result_string;
	stringfy(a, expected_string);
	rope* right;

	report(a);
	rope* n_o_t = new rope("_not");
	insert(a, n_o_t, 16);
	report(a);

	string user_name = "stephen liang";
	rope* my_name = ropify(user_name);
	report(my_name);

	/*
	for (int i = 0; i <= expected_string.size(); i++) { // should be no problem specifying bad offsets
		right = NULL;
		split(a, i, right);
		left_string = "";
		right_string = "";
		result_string = "";
		stringfy(a, left_string);
		stringfy(right, right_string);
		result_string = left_string + right_string;

		if (result_string != expected_string) {
			cout << "Failed, actual string is '" << result_string << "'" << endl;
			return;
		} else {
			cout << "Passed splitting at pos " << i << endl;
		}
		a = concat(a, right);
	}

	report(a);
	*/
}

class RopeTest {
	rope* root_rope;
	string s;
	bool naive_used;
public:
	RopeTest(const std::string &s)
		: root_rope(NULL)
		, s(s)
		, naive_used(false) {
			root_rope = ropify(s);
	}

	void process_naive( int i, int j, int k ) {
		naive_used = true;
		// Replace this code with a faster implementation
		std::string t = s.substr(0, i) + s.substr(j + 1);
		s = t.substr(0, k) + s.substr(i, j - i + 1) + t.substr(k);
	}

	void process( int i, int j, int k ) {
		rope* left = root_rope;
		rope* middle = NULL;
		rope* right = NULL;

		if (i >= root_rope->len || i < 0) {
			return;
		}

		if (j < i) {
			return;
		}

		if (j > root_rope->len) {
			return;
		}

		if (i == k) {
			return;
		}

		if (k < 0) {
			return;
		}

		int extracted_length = root_rope->len - (j - i + 1);
		if (k > extracted_length) {
			return;
		}

		split(left, i, middle);
		split(middle, j - i + 1, right);
		left = concat(left, right);
		insert(left, middle, k);
		root_rope = left;
	}

	std::string result() {
		if (naive_used)
			return s;

		string ret = "";
		stringfy(root_rope, ret);
		return ret;
	}
};

int main() {
#if DEBUG
	rope_dev_test();
	return 0;
#endif
	std::ios_base::sync_with_stdio(0);
	std::string s;
	std::cin >> s;
	RopeTest ropeTest(s);
	int actions;
	std::cin >> actions;
	for (int action_index = 0; action_index < actions; ++action_index) {
		int i, j, k;
		std::cin >> i >> j >> k;
		ropeTest.process(i, j, k);
	}
	std::cout << ropeTest.result() << std::endl;
}
