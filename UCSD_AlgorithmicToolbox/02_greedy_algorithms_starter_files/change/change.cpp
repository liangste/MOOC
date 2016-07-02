#include <iostream>

int get_change(int n) {

	if (n < 1)
		return 0;

	//write your code here
	int change_cnt = 0;
	int rem;
	int quotient;

	quotient = n / 10;
	n %= 10;
	change_cnt += quotient;

	quotient = n / 5;
	n %= 5;
	change_cnt += quotient;
	change_cnt += n;

	return change_cnt;
}

int main() {
	int n;
	std::cin >> n;
	std::cout << get_change(n) << '\n';
}
