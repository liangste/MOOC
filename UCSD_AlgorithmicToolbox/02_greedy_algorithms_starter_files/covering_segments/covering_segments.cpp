#include <algorithm>
#include <iostream>
#include <climits>
#include <queue>
#include <vector>

using namespace std;

#define DEBUG 0

struct Segment {
	int start, end;

	// sorted first by earliest end, then earliest start
	bool operator==(const Segment& b) const {
		return (this->start == b.start && this->end == b.end);
	}

	bool operator<(const Segment& b) const {
		if (this->end < b.end)
			return true;
		if (this->end == b.end) {
			return this->start < b.start;
		}
		return false;
	}

	bool operator<=(const Segment& b) const {
		if (*this == b || *this < b)
			return true;
		return false;
	}

	bool operator>(const Segment& b) const {
		if (this->end > b.end)
			return true;
		if (this->end == b.end) {
			return this->start > b.start;
		}
		return false;
	}

	bool operator>=(const Segment& b) const {
		if (*this == b || *this > b)
			return true;
		return false;
	}
};

void do_test() {
	Segment t0 = {.start = 0, .end = 0};
	Segment t1 = {.start = 0, .end = 1};
	Segment t2 = {.start = 1, .end = 0};
	Segment t3 = {.start = 1, .end = 1};

	cout << "t0 == t0 ... " << (t0 == t0) << endl;
	cout << "t0 < t1 ... " << (t0 < t1) << endl;
	cout << "t0 <= t1 ... " << (t0 <= t1) << endl;
	cout << "t0 >= t1 ... " << (t0 >= t1) << endl;
	cout << "t0 > t1 ... " << (t0 > t1) << endl;

	cout << "t0 == t0 ... " << (t0 == t0) << endl;
	cout << "t0 < t2 ... " << (t0 < t2) << endl;
	cout << "t0 <= t2 ... " << (t0 <= t2) << endl;
	cout << "t0 >= t2 ... " << (t0 >= t2) << endl;
	cout << "t0 > t2 ... " << (t0 > t2) << endl;
}

vector<int> optimal_points(vector<Segment> &segments) {
	vector<int> points;
	priority_queue<Segment, vector<Segment>, greater<Segment> > q0;

	for (int i = 0; i < segments.size(); i++) {
		q0.push(segments[i]);
	}

	Segment s;
	while (!q0.empty()) {
		s = q0.top();
		//cout << "(start, end) = " << s.start << ", " << s.end << endl;

		if (0 == points.size()) {
			points.push_back(s.end);
		} else {
			if (s.start <= points.back() && points.back() <= s.end) {
				// nothing
			} else {
				points.push_back(s.end);
			}
		}

		q0.pop();
	}
	return points;
}

int main() {
#if DEBUG
	do_test();
	return 0;
#endif

	int n;
	std::cin >> n;

	vector<Segment> segments(n);
	for (size_t i = 0; i < segments.size(); ++i) {
		std::cin >> segments[i].start >> segments[i].end;
	}
	vector<int> points = optimal_points(segments);
	std::cout << points.size() << "\n";
	for (size_t i = 0; i < points.size(); ++i) {
		std::cout << points[i] << " ";
	}
}
