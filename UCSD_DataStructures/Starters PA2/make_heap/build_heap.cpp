#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

int Parent_(int i) {
	if (i == 0)
		return 0;
	if (i > 0) {
		return (i - 1) / 2;
	}
	return -1;
}

int LeftChild_(int i) {
	return 2 * i + 1;
}

int RightChild_(int i) {
	return 2 * i + 2;
}

class HeapBuilder {
 private:
	vector<int> data_;
	vector< pair<int, int> > swaps_;

	void WriteResponse() const {
		cout << swaps_.size() << "\n";
		for (int i = 0; i < swaps_.size(); ++i) {
			cout << swaps_[i].first << " " << swaps_[i].second << "\n";
		}
	}

	void ReadData() {
		int n;
		cin >> n;
		data_.resize(n);
		for(int i = 0; i < n; ++i)
			cin >> data_[i];
	}

	void SiftDown(int i) {
		int minIndex;
		int left;
		int right;
		int tmp;
		minIndex = i;
		left = LeftChild_(i);
		if (left <= (data_.size() - 1) && data_[left] < data_[minIndex])
			minIndex = left;
		right = RightChild_(i);
		if (right <= (data_.size() - 1) && data_[right] < data_[minIndex])
			minIndex = right;
		if (i != minIndex) {
			tmp = data_[i];
			data_[i] = data_[minIndex];
			data_[minIndex] = tmp;
			swaps_.push_back(make_pair(i, minIndex));
			SiftDown(minIndex);
		}
	}

	void GenerateSwaps() {
		swaps_.clear();
		// The following naive implementation just sorts
		// the given sequence using selection sort algorithm
		// and saves the resulting sequence of swaps.
		// This turns the given array into a heap,
		// but in the worst case gives a quadratic number of swaps.
		//
		for (int i = (data_.size() - 1) / 2; i >= 0; --i) {
			SiftDown(i);
		}
	}

 public:
	void Solve() {
		ReadData();
		GenerateSwaps();
		WriteResponse();
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	HeapBuilder heap_builder;
	heap_builder.Solve();
	return 0;
}
