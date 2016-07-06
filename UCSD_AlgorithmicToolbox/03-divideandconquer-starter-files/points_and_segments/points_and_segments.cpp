#include <iostream>
#include <vector>
#include <queue>

#define DEBUG 0

using namespace std;

enum TwoDElementType_t {
  SegmentStart = 0,
  Point,
  SegmentEnd,
};

struct TwoDElement {
  int point;
  TwoDElementType_t type;
  size_t index;

  TwoDElement(int p, TwoDElementType_t t, size_t i)
    : point(p)
    , type(t)
    , index(i) {};

  // segment start comes before point, which in turn comes beforce segment end
  bool operator<(const TwoDElement& other) const {
    if (this->point < other.point)
      return true;
    else if (this->point == other.point) {
      return (unsigned) this->type < (unsigned) other.type;
    }
    return false;
  }

  bool operator>(const TwoDElement& other) const {
    if (this->point > other.point)
      return true;
    else if (this->point == other.point) {
      return (unsigned) this->type > (unsigned) other.type;
    }
    return false;
  }

  bool operator==(const TwoDElement& other) const {
    return this->point == other.point && this->type == other.type && this->index == other.index;
  }

  bool operator<=(const TwoDElement& other) const {
    return *this < other || *this == other;
  }

  bool operator>=(const TwoDElement& other) const {
    return *this > other || *this == other;
  }
};

vector<int> fast_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size(), 0);
  priority_queue<TwoDElement, vector<TwoDElement>, greater<TwoDElement> > elements;

  //write your code here
  for (int i = 0; i < starts.size(); i++) {
    elements.push(TwoDElement(starts[i], SegmentStart, i));
    elements.push(TwoDElement(ends[i], SegmentEnd, i));
  }

  for (int i = 0; i < points.size(); i++) {
    elements.push(TwoDElement(points[i], Point, i));
  }

  int cur_seg_count = 0;
  while (!elements.empty()) {
    TwoDElement elem = elements.top();
    elements.pop();

#if DEBUG
    cout << "element " << elem.type << " point " << elem.point << " index " << elem.index << endl;
#endif

    if (elem.type == SegmentStart) {
      cur_seg_count++;
    } else if (elem.type == SegmentEnd) {
      cur_seg_count--;
    } else if (elem.type == Point) {
      cnt[elem.index] = cur_seg_count;
    }
  }

  return cnt;
}

vector<int> naive_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size());
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = 0; j < starts.size(); j++) {
      cnt[i] += starts[j] <= points[i] && points[i] <= ends[j];
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
