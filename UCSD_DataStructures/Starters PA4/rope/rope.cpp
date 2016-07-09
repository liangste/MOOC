#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

typedef struct rope_node rope;

// invariants
// either data is non-empty or left and right are both non-empty
// left and right must both be set, in which case the rope_node is a
// meta-node. Otherwise it is a data node
struct rope_node {
	int len;
	rope* left;
	rope* right;
	string data;
};

// return length of rope R
//
// return R->len
int rope_length(rope* R);

// insert rope S after rope R, return newly joined rope
//
// create a new rope newR
// set newR->left = R
// set newR->right = S
// update newR->len
// return newR
rope* rope_join(rope* R, rope* S);

void rope_cut(rope* R, rope* left, rope* right, int pos);

// returns char at location i of rope
//
// NOTE implement recursive search
// if data != empty
//   if left->len >= i
//     search left
//   otherwise search right
// else
//   data[i]
char rope_char_at(rope* R, int i);

// extract substring from location lo to hi from rope, return it as new rope
rope rope_sub(rope R, int lo, int hi);

// insert rope S into rope R at offset
void rope_insert_at(rope R, rope S, int offset);

// create a new rope from string
rope rope_new(string s);

// convert rope to string
string rope_to_string(rope R);



class RopeTest {
	rope* root_rope;
public:
	RopeTest(const std::string &s)
		: root_rope(NULL) {

	}

	void process( int i, int j, int k ) {
		// chop into three pieces, left, mid, right
		// merge left and right
		// insert mid into merged at pos k
	}

	std::string result() {
		return "";
	}
};

int main() {
	std::ios_base::sync_with_stdio(0);
	std::string s;
	std::cin >> s;
	RopeTest restTest(s);
	int actions;
	std::cin >> actions;
  for (int action_index = 0; action_index < actions; ++action_index) {
    int i, j, k;
		std::cin >> i >> j >> k;
		rope.process(i, j, k);
	}
	std::cout << rope.result() << std::endl;
}
