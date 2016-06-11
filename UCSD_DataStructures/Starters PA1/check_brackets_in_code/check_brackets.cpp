#include <iostream>
#include <stack>
#include <string>

using namespace std;

struct Bracket {
		Bracket(char type, int position):
				type(type),
				position(position)
		{}

		bool Matchc(char c) {
				if (type == '[' && c == ']')
						return true;
				if (type == '{' && c == '}')
						return true;
				if (type == '(' && c == ')')
						return true;
				return false;
		}

		char type;
		int position;
};

int main() {
		int position;
		std::string text;
		getline(std::cin, text);

		std::stack <Bracket> bstack;
		for (position = 0; position < text.length(); ++position) {
				char next = text[position];

				if (next == '(' || next == '[' || next == '{') {
						bstack.push(Bracket(next, position));
						continue;
				}

				if (next == ')' || next == ']' || next == '}') {
						// Process closing bracket, write your code here
						if (bstack.empty()) {
							// unbalanced, got closing bracket without opening bracket
							cout << position  + 1 << endl;
							return 0;
						} else {
							Bracket openb = bstack.top();
							bstack.pop();
							if (openb.Matchc(next)) {
								// matched
								continue;
							} else {
								// closing bracket didn't match
								cout << position + 1 << endl;
								return 0;
							}
						}
						continue;
				}
		}

		if (!bstack.empty()) {
			Bracket lastb = bstack.top();
			cout << lastb.position + 1 << endl;
			return 0;
		}

		std::cout << "Success" << std::endl;

		return 0;
}
