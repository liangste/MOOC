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

long long lcm(int a, int b) {
	//write your code here
	int gcd_var = gcd(a, b);
	long long ll_a = (long long) a;
	long long ll_b = (long long) b;
	long long ll_gcd = (long long) gcd_var;
	return ll_a * ll_b / ll_gcd;
}

int main() {
	int a, b;
	std::cin >> a >> b;
	std::cout << lcm(a, b) << std::endl;
	return 0;
}
