#include <iostream>
#include <map>
#include <string>

using namespace std;

enum Edge {
  Top = 0,
  Left,
  Bottom,
  Right,
};

enum Edge OppositeEdge(enum Edge e) {
  switch(e) {
    case Top:
      return Bottom;
    case Left:
      return Right;
    case Bottom:
      return Top;
    case Right:
      return Left;
  };
};

const string BorderColor = "black";

int main(void) {
  string sqrDesc;
  while (cin >> sqrDesc) {
    cout << sqrDesc << endl;
  }
  return 0;
}
