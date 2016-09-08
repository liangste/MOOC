#include <iostream>
#include <vector>

using namespace std;

void solve(vector<vector<float> >& matrix) {

}

void show_solution(vector<vector<float> >& matrix) {

}

int main() {
	int n;
	cin >> n;
	vector<vector<float> > matrix(n);
	for (int r = 0; r < n; r++) {
		matrix[r].resize(n + 1);
		for (int c = 0; c <= n; c++)
			cin >> matrix[r][c];
	}

	solve(matrix);
	show_solution(matrix);

	return 0;
}
