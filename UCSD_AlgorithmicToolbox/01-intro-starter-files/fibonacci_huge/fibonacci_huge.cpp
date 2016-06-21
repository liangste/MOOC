#include <iostream>
#include <vector>

using namespace std;

long long huge_store_1;
long long huge_store_2;

bool find_pisano_period(long long m, vector<long long>& period, long long max) {
	period.push_back(0);
	period.push_back(1);

	long long l_2 = 0;
	long long l_1 = 0;

	for (int i = 2; i > 0; i++) {
		period.push_back((period[i - 2] + period[i - 1]) % m);
		if (period[i] == 1 && period[i - 1] == 0) {
			period.pop_back();
			period.pop_back();
			return true;
		}
	}

	return false;
}

long long get_fibonaccihuge(long long n, long long m, vector<long long>& period) {
	long long index = n % period.size();
	/*
	cout << "index = " << index << " period size = " << period.size() << endl;
	for (int i = 0; i < period.size(); i++) {
		cout << " " << period[i];
	}
	cout << endl;
	*/
	return period[index];
}

int main() {
		long long n, m;
		std::cin >> n >> m;
		vector<long long> pisano_period;

		find_pisano_period(m, pisano_period, n);

		std::cout << get_fibonaccihuge(n, m, pisano_period) << '\n';
}
