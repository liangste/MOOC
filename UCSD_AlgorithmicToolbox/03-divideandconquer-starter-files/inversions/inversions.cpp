#include <iostream>
#include <vector>

#define DEBUG 0

using namespace std;

// left elements from left to ave inclusive
// right elements from ave + 1 to right inclusive
long long combine(vector<int>& input, vector<int>& output, size_t left, size_t ave, size_t right) {
  long long inversions = 0;
  size_t output_index = left;
  size_t left_index = left;
  size_t right_index = ave + 1;

  int right_largest = input[right];

  while (output_index <= right) {
    if (left_index <= ave && right_index <= right) {
      // take lower of the two
      if (input[left_index] > input[right_index]) {
        output[output_index] = input[right_index];
        right_index++;

        inversions += (ave - left_index + 1);

      } else {
        output[output_index] = input[left_index];
        left_index++;
      }
    } else if (left_index <= ave) {
      // take from left
      output[output_index] = input[left_index];
      left_index++;
    } else {
      // take from right
      output[output_index] = input[right_index];
      right_index++;
    }

    output_index++;
  }

  return inversions;
}

// input - unsorted array before merge_sort works its magic
// output - output after merge_sort worked its magic
// left and right indices are inclusive
long long merge_sort(vector<int> &input, vector<int> &output, size_t left, size_t right) {
  long long number_of_inversions = 0;
  if (left >= right) return number_of_inversions;
  size_t ave = left + (right - left) / 2;
  number_of_inversions += merge_sort(input, output, left, ave);
  number_of_inversions += merge_sort(input, output, ave + 1, right);
  // output has 2 segments, left and right. Each sorted

  number_of_inversions += combine(input, output, left, ave, right);

  for (int i = left; i <= right; i++) {
    input[i] = output[i];
  }

  // output must be sorted
  return number_of_inversions;
}

int main() {
  int n;
  std::cin >> n;
  vector<int> a(n);
  for (size_t i = 0; i < a.size(); i++) {
    std::cin >> a[i];
  }
  vector<int> b(a.size());
  std::cout << merge_sort(a, b, 0, a.size() - 1) << '\n';

#if DEBUG
  for (int i = 0; i < b.size(); i ++) {
    cout << " " << b[i];
  }
  cout << endl;
#endif
}
