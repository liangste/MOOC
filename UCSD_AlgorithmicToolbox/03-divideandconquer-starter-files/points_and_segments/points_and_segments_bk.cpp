#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#define DEBUG 0

using namespace std;

struct Segment {
  int start;
  int end;

  Segment(int s, int e) : start(s), end(e) {};

  bool operator<(const Segment& other) const {
    if (this->start < other.start)
      return true;
    if (this->start == other.start) {
      return this->end < other.end;
    }
    return false;
  }

  bool operator>(const Segment& other) const {
    if (this->start > other.start)
      return true;
    if (this->start == other.start) {
      return this->end > other.end;
    }
    return false;
  }

  bool operator==(const Segment& other) const {
    if (this->start == other.start && this->end == other.end) {
      return true;
    }
    return false;
  }

  bool operator<=(const Segment& other) const {
    if (*this == other || *this < other)
      return true;
    return false;
  }

  bool operator>=(const Segment& other) const {
    if (*this == other || *this > other)
      return true;
    return false;
  }

  bool Contains(int point) const {
    if (start <= point && point <= end)
      return true;
    return false;
  }
};

// Get index an a segment taht contains point p
// returns index of a segment
// return -1 if no segment can contain p
int binary_segment_search(const vector<Segment>& segments, int point) {
  if (segments.size() == 0)
    return -1;

  int left = 0, right = (int) segments.size() - 1;

  while (left <= right) {
    int mid = (left + right) / 2;

    if (segments[mid].Contains(point)) {
      return mid;
    } else if (segments[mid].start > point) {
      // search before mid point
      right = mid - 1;
    } else {
      // search after mid point
      left = mid + 1;
    }
  }

  return -1;
}

// Given hint index idx into segments, find how many segments contain point
// assume segments is already sorted
// return number of segments containing point around index idx
int count_segment_neighbours(const vector<Segment>& segments, int idx, int point) {
  int count = 0;
  int tmp_idx;

  if (segments[idx].Contains(point)) {
    count = 1;

    tmp_idx = idx - 1;
    while (tmp_idx >= 0) {
      if (segments[tmp_idx].Contains(point)) {
        count++;
      }
      tmp_idx--;
    }

    tmp_idx = idx + 1;
    while (tmp_idx < segments.size()) {
      if (segments[tmp_idx].Contains(point)) {
        count++;
      } else if (segments[tmp_idx].start > point) {
        break;
      }
      tmp_idx++;
    }
  }

  return count;
}

vector<int> fast_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size(), 0);

  vector<Segment> segVector;

  // sort segments
  for (int i = 0; i < starts.size(); i++) {
    segVector.push_back(Segment(starts[i], ends[i]));
  }
  sort(segVector.begin(), segVector.end());

#if DEBUG
  for (int i = 0; i < segVector.size(); i++) {
    cout << "Seg{" << segVector[i].start << "," << segVector[i].end << "}" << endl;
  }
#endif

  int idx;

  for (int p = 0; p < points.size(); p++) {
    idx = binary_segment_search(segVector, points[p]);
    if (-1 == idx) {
#if DEBUG
      cout << "no segments contain point " << points[p] << endl;
#endif
      cnt[p] = 0;
    } else {
      // search neighbours
      cnt[p] += count_segment_neighbours(segVector, idx, points[p]);
    }
  }

  return cnt;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<int> starts(n), ends(n);
  for (size_t i = 0; i < starts.size(); i++) {
    std::cin >> starts[i] >> ends[i];
  }
  vector<int> points(m);
  for (size_t i = 0; i < points.size(); i++) {
    std::cin >> points[i];
  }
  //use fast_count_segments
  vector<int> cnt = fast_count_segments(starts, ends, points);
  for (size_t i = 0; i < cnt.size(); i++) {
    std::cout << cnt[i] << ' ';
  }
}
