#include <iostream>
#include <vector>

using namespace std;

int get_fibonacci_last_digit(int n, vector<int>& lasts) {
	//write your code here

	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;

	lasts[0] = 0;
	lasts[1] = 1;

	for (int i = 2; i <= n; i++) {
		lasts[i] = (lasts[i - 1] + lasts[i - 2]) % 10;
	}
	return lasts[n];
}

int main() {
	int n;
	vector<int> lasts;
	std::cin >> n;
	lasts.resize(n + 1);
	int c = get_fibonacci_last_digit(n, lasts);
	std::cout << c << '\n';
}
