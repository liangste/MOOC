#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#define DEBUG

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

class Square {
public:
  Square(const string& top, const string& left,
    const string& bottom, const string& right)
    : BorderColors(vector<string>(4))
  {
    BorderColors[Top] = top;
    BorderColors[Left] = left;
    BorderColors[Bottom] = bottom;
    BorderColors[Right] = right;
  }
  vector<string> BorderColors;
};

const string BorderColor = "black";

void GenerateSquarePosPermutations(vector<vector<int>>& posPerms, int s) {
	vector<int> perm(s);
	iota(perm.begin(), perm.end(), 0);
	do {
		posPerms.push_back(perm);
	} while (next_permutation(perm.begin(), perm.end()));
}

vector<int> FilterValidArrangement(vector<vector<int>>& posPerms, vector<Square>& squares) {
	vector<int> ret;
	int cnt;

	// check 4 corners
	// check adjacent color matches
	for (auto& p : posPerms) {
		for (int i = 1; i < p.size(); i++) {
			int a = p[i - 1];
			int b = p[i];
			if (squares[a].BorderColors[Right] == squares[b].BorderColors[Left])
				cnt++;
		}
	}

	return ret;
}

int main(void) {
  string sqrDesc;
  char TopColor[20];
  char LeftColor[20];
  char BottomColor[20];
  char RightColor[20];
  vector<Square> Squares;

  while (cin >> sqrDesc) {
    sscanf(sqrDesc.c_str(), "(%s,%s,%s,%s)", TopColor, LeftColor, BottomColor, RightColor);
    Squares.push_back(Square(TopColor, LeftColor, BottomColor, RightColor));
  }

#ifdef DEBUG
  cout << "Scanned " << Squares.size() << " squares" << endl;
#endif

  int sqrRtVal = sqrt(Squares.size());
  if (pow(sqrRtVal, 2) != Squares.size()) {
    cout << "Input squares do not form a perfect root" << endl;
	return 1;
  }

  vector<vector<int>> PosPerms;
  GenerateSquarePosPermutations(PosPerms, Squares.size());
  vector<int> goodPerm = FilterValidArrangement(PosPerms, Squares);

  return 0;
}
