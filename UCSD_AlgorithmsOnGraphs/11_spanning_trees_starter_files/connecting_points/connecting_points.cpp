#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cfloat>
#include <cmath>
#include <climits>
#include <queue>

using namespace std;

struct Point {
  int x;
  int y;
  double cost;
  int index; // index into position of original input vector, this way we
  // can still identify Points by an unsigned integer

  Point(int input_x, int input_y, double input_cost, int input_index)
    : x(input_x)
    , y(input_y)
    , cost(input_cost)
    , index(input_index) {};

  double DistToPoint(int other_x, int other_y) {
    return sqrt(pow(((double) x - (double) other_x), 2.0) + pow(((double) y - (double) other_y), 2.0));
  }

  bool operator>(const Point& other) const {
    return this->cost > other.cost;
  }

  bool operator<(const Point& other) const {
    return this->cost < other.cost;
  }

  bool operator==(const Point& other) const {
    return this->cost == other.cost;
  }

  bool operator>=(const Point& other) const {
    return this->cost == other.cost || this->cost > other.cost;
  }

  bool operator<=(const Point& other) const {
    return this->cost == other.cost || this->cost < other.cost;
  }
};


double minimum_distance(vector<int> x, vector<int> y) {
  double result = 0.;
  //write your code here
  int last_index = 0;
  int num_points = x.size();
  double cost[num_points];
  int parent[num_points];
  bool in_tree[num_points]; // point is in a tree
  priority_queue<Point, vector<Point>, greater<Point> > PrioQ;

  // we choose the first node as starting node
  cost[0] = 0.;
  in_tree[0] = false;
  PrioQ.push(Point(x[0], y[0], cost[0], 0));
  parent[0] = -1;

  for (int i = 1; i < num_points; i++) {
    cost[i] = DBL_MAX;
    in_tree[i] = false;
    parent[i] = -1;
    PrioQ.push(Point(x[i], y[i], DBL_MAX, i));
  }

  // TODO build MST
  while (!PrioQ.empty()) {
    Point v = PrioQ.top();
    PrioQ.pop();

    if (in_tree[v.index]) {
      continue;
    }

    //cout << "got point (x,y) = " << v.x << "," << v.y << " at index = " << v.index << " with cost " << v.cost << endl;

    // look at every other nodes,
    for (int z = 0; z < num_points; z++) {
      if (z == v.index) {
        continue;
      }

      double dist = v.DistToPoint(x[z], y[z]);

      if (!in_tree[z] && dist < cost[z]) {
        cost[z] = dist;
        parent[z] = v.index;
        //cout << "set parent of " << z << " to " << v.index << " and cost " << cost[z] << endl;
        PrioQ.push(Point(x[z], y[z], cost[z], z));
        last_index = z;
      }
    }

    in_tree[v.index] = true;
  }

  // TODO traverse MST and calculate weights
  int mom_or_dad;
  for (int i = 0; i < num_points; i++) {
    if (parent[i] != -1) {
      mom_or_dad = parent[i];
      //cout << "parent of " << i << " is " << mom_or_dad << endl;
      Point p(x[i], y[i], 0., i);
      result += p.DistToPoint(x[mom_or_dad], y[mom_or_dad]);
    }
  }

  return result;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<int> x(n), y(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x[i] >> y[i];
  }
  std::cout << std::setprecision(10) << minimum_distance(x, y) << std::endl;
}
