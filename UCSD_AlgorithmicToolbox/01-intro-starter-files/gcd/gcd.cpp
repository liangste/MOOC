#include <iostream>

int gcd(int a, int b) {
	if (b == 0)
		return a;
	//write your code here
	if (b > a)
		return gcd(b, a);
	int q = a / b;
	int r_a = a - b * q;
	return gcd(b, r_a);
}

int main() {
	int a, b;
	std::cin >> a >> b;
	std::cout << gcd(a, b) << std::endl;
	return 0;
}
