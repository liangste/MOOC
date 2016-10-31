#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>

using namespace std;

typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<vector<int>> vvi;

#define LOOP(x, m) for(int x = 0; x < m; ++x)

int g_nVars, g_nClauses;
vvi g_adjList;

int ToIndex(int i) {
	return (i > 0) ? i - 1 : g_adjList.size() - 1;
}

void AddImplication(int a, int b) {
	g_adjList[ToIndex(-a)].push_back(ToIndex(b));
	g_adjList[ToIndex(-b)].push_back(ToIndex(a));
}

int main(void) {
	cin >> g_nVars >> g_nClauses;
	g_adjList.resize(g_nVars * 2);

	int a, b;
	while (g_nClauses--) {
		cin >> a >> b;
		AddImplication(a, b);
	}
}
