#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
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

std::map<string, int> ColorToIdMap;
vector<string> AllColors; // max here is 25 * 4 - 25

class Square {
public:
  Square(int top, int left, int bottom, int right)
    : BorderColors(vector<int>(4))
  {
    BorderColors[Top] = top;
    BorderColors[Left] = left;
    BorderColors[Bottom] = bottom;
    BorderColors[Right] = right;
  }

  string ToString() {
    return "(" + AllColors[BorderColors[Top]] + "," + AllColors[BorderColors[Left]] + ","
      + AllColors[BorderColors[Bottom]] + "," + AllColors[BorderColors[Right]] + ")";
  }
  vector<int> BorderColors;
};

const string BorderColor = "black";
int BorderColorIdx = 0;

void PrintIntVector(const vector<int>& v) {
  for (auto& val : v) {
    cout << val << " ";
  }
  cout << endl;
}

vector<int> FilterValidArrangement(vector<Square>& squares) {
	vector<int> ret;
	int cnt;
  int wh = sqrt(squares.size());
  int whoff = wh - 1;
  bool fail;

  vector<int> p(squares.size());
  iota(p.begin(), p.end(), 0);

  do {
    //PrintIntVector(p);
    // check all corners
    if (squares[p[0]].BorderColors[Top] == BorderColorIdx
      && squares[p[0]].BorderColors[Left] == BorderColorIdx
      && squares[p[whoff]].BorderColors[Top] == BorderColorIdx
      && squares[p[whoff]].BorderColors[Right] == BorderColorIdx
      && squares[p[squares.size() - wh]].BorderColors[Bottom] == BorderColorIdx
      && squares[p[squares.size() - wh]].BorderColors[Left] == BorderColorIdx
      && squares[p[squares.size() - 1]].BorderColors[Bottom] == BorderColorIdx
      && squares[p[squares.size() - 1]].BorderColors[Right] == BorderColorIdx)
    {
      /*
      fail = false;
      for (int i = 0; i < squares.size() - wh; i++) {
        // check Bottom
        if (squares[p[i]].BorderColors[Bottom] != squares[p[i + wh]].BorderColors[Top]) {
          fail = true;
          break;
        }
        if ((i % wh) != (wh - 1)) { // if not on right edge
          // check right edge
          if (squares[p[i]].BorderColors[Right] != squares[p[i + 1]].BorderColors[Left]) {
            fail = true;
            break;
          }
        }
      }

      if (fail) continue;

      ret = p;
      break;
      */
      return p;
    }
  } while (next_permutation(p.begin(), p.end()));

	return ret;
}

void PrintGoodPerm(vector<int>& goodPerm, vector<Square>& squares) {
  int sq = sqrt(goodPerm.size());
  for (int i = 0; i < goodPerm.size(); i++) {
    cout << squares[goodPerm[i]].ToString();
    if ((i % sq) == (sq - 1)) {
      cout << endl;
    } else {
      cout << ";";
    }
  }
}

// add color, return index
int AddColor(const string& c) {
#ifdef DEBUG
  cout << "Adding color: " << c << endl;
#endif
  if (ColorToIdMap.find(c) == ColorToIdMap.end()) {
    ColorToIdMap[c] = AllColors.size();
    AllColors.push_back(c);
  }

  return ColorToIdMap[c];
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int main(void) {
  string sqrDesc;
  vector<Square> Squares;

  while (cin >> sqrDesc) {
    sqrDesc = sqrDesc.substr(1, sqrDesc.size() - 2); // remove '(' and ')'
    vector<string> colors = split(sqrDesc, ',');
    int top_i = AddColor(colors[0]);
    int left_i = AddColor(colors[1]);
    int bottom_i = AddColor(colors[2]);
    int right_i = AddColor(colors[3]);
    Squares.push_back(Square(top_i, left_i, bottom_i, right_i));
  }
  BorderColorIdx = ColorToIdMap[BorderColor];


#ifdef DEBUG
  cout << "Scanned " << Squares.size() << " squares" << endl;
  for (auto& s : Squares) {
    cout << s.ToString() << endl;
  }
#endif


  int sqrRtVal = sqrt(Squares.size());
  if (pow(sqrRtVal, 2) != Squares.size()) {
    cout << "Input squares do not form a perfect root" << endl;
    return 1;
  }

  vector<int> goodPerm = FilterValidArrangement(Squares);
  PrintGoodPerm(goodPerm, Squares);

  return 0;
}
