// re-implemented with back-tracking dfs

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

//#define DEBUG

using namespace std;

const string BorderColor = "black";
int TopLeftIdx = 0;
int BorderColorIdx = 0;

// use the word "border", since "edge" refers better to graphs
enum BorderPos {
  Top = 0,
  Left,
  Bottom,
  Right,
};

std::map<string, int> ColorStringToIdMap;
vector<string> AllColors; // max here is 25 * 4 - 25

// Top edges, *EdgeColorList[Color] contains vector of square positions with that color
vector<vector<int>> TopEdgeColorList;
vector<vector<int>> LeftEdgeColorList;
vector<vector<int>> BottomEdgeColorList;
vector<vector<int>> RightEdgeColorList;

// Square class
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

// All squares used
vector<Square> Squares;

void PrintIntVector(const vector<int>& v) {
  cout << "[ ";
  for (auto& val : v) {
    cout << val << " ";
  }
  cout << "]" << endl;
}

// can also do partial arrangements
bool IsValidArrangement(const vector<int>& v) {
  int wh = sqrt(Squares.size());

  for (int i = 1; i < v.size(); i++) {
    if (i == (wh - 1)) { // top right corner
      if (Squares[v[i]].BorderColors[Top] != BorderColorIdx
        || Squares[v[i]].BorderColors[Right] != BorderColorIdx) {
#ifdef DEBUG
          cout << "top right corner violation at index " << i << endl;
#endif
          return false;
        }
    } else if (i == (Squares.size() - wh)) {
      if (Squares[v[i]].BorderColors[Left] != BorderColorIdx
        || Squares[v[i]].BorderColors[Bottom] != BorderColorIdx){
#ifdef DEBUG
          cout << "bottom left corner violation at index " << i << endl;
#endif
          return false;
        }
    } else if (i == (Squares.size() - 1)) {
      if (Squares[v[i]].BorderColors[Right] != BorderColorIdx
        || Squares[v[i]].BorderColors[Bottom] != BorderColorIdx) {
#ifdef DEBUG
          cout << "bottom right corner violation at index " << i << endl;
#endif
          return false;
        }
    }

    if (i < wh) { // squares alone top edge
      if (Squares[v[i]].BorderColors[Top] != BorderColorIdx) {
#ifdef DEBUG
        cout << "top edge violation at index " << i << endl;
#endif
        return false;
      }
    } else if ((i % wh) == 0) { // left edge
      if (Squares[v[i]].BorderColors[Left] != BorderColorIdx) {
#ifdef DEBUG
        cout << "left edge violation at index " << i << endl;
#endif
        return false;
      }
    } else if ((i % wh) == (wh - 1)) { // right edge
      if (Squares[v[i]].BorderColors[Right] != BorderColorIdx) {
#ifdef DEBUG
        cout << "right edge violation at index " << i << endl;
#endif
        return false;
      }
    } else if (i >= (Squares.size() - wh)) { // bottom edge
      if (Squares[v[i]].BorderColors[Bottom] != BorderColorIdx) {
#ifdef DEBUG
        cout << "bottom edge violation at index " << i << endl;
#endif
        return false;
      }
    }

    // make sure bottom edge match
    if ((i + wh) < v.size()) {
      if (Squares[v[i]].BorderColors[Bottom] != Squares[v[i + wh]].BorderColors[Top]) {
#ifdef DEBUG
        cout << "neighbour mismatch at index " << i << endl;
#endif
        return false;
      }
    }
  }

  return true;
}

void FindNextValidArrangement(vector<int>& arrgm, vector<bool>& visited, vector<int>& sol) {

#ifdef DEBUG
  PrintIntVector(arrgm);
#endif

  if (sol.size()) {
    return; // we're done here
  }

  if (!IsValidArrangement(arrgm)) {
    return; // not valid, try again
  }

  if (arrgm.size() == Squares.size()) { // valid and we've just reached the end
    sol = arrgm;
    return;
  }

  // otherwise recurse
  int sq = sqrt(Squares.size());
  int nextIdx = arrgm.size();
  int color;
  vector<int>& EdgeList = LeftEdgeColorList[0];

  if (nextIdx % sq) { // grow right, same row
    color = Squares[arrgm.back()].BorderColors[Right];

#ifdef DEBUG
    cout << "grow right, looking for color " << AllColors[color] << endl;
#endif

    for (auto cl : LeftEdgeColorList[color]) {
      if (!visited[cl]) {
        visited[cl] = true;
        arrgm.push_back(cl);
        FindNextValidArrangement(arrgm, visited, sol);
        arrgm.pop_back();
        visited[cl] = false;
      }
    }
  } else { // first of new row
    color = Squares[arrgm[nextIdx - sq]].BorderColors[Bottom];

#ifdef DEBUG
    cout << "grow new row, looking for color " << AllColors[color] << endl;
#endif

    for (auto cl : TopEdgeColorList[color]) {
      if (!visited[cl]) {
        visited[cl] = true;
        arrgm.push_back(cl);
        FindNextValidArrangement(arrgm, visited, sol);
        arrgm.pop_back();
        visited[cl] = false;
      }
    }
  }
};

vector<int> FindSolution() {
  vector<int> working, sol;
  vector<bool> visited(Squares.size(), false);

  working.push_back(TopLeftIdx);
  visited[TopLeftIdx] = true;

  FindNextValidArrangement(working, visited, sol);

  return sol;
}

void PrintGoodPerm(vector<int>& goodPerm) {
  int sq = sqrt(goodPerm.size());
  for (int i = 0; i < goodPerm.size(); i++) {
    cout << Squares[goodPerm[i]].ToString();
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
  if (ColorStringToIdMap.find(c) == ColorStringToIdMap.end()) {
    ColorStringToIdMap[c] = AllColors.size();
    AllColors.push_back(c);
  }

  return ColorStringToIdMap[c];
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

  while (cin >> sqrDesc) {
    sqrDesc = sqrDesc.substr(1, sqrDesc.size() - 2); // remove '(' and ')'
    vector<string> colors = split(sqrDesc, ',');
    int top_i = AddColor(colors[0]);
    int left_i = AddColor(colors[1]);
    int bottom_i = AddColor(colors[2]);
    int right_i = AddColor(colors[3]);
    Squares.push_back(Square(top_i, left_i, bottom_i, right_i));

    if (colors[0] == colors[1] && colors[1] == BorderColor)
      TopLeftIdx = Squares.size() - 1;
  }
  BorderColorIdx = ColorStringToIdMap[BorderColor];


#ifdef DEBUG
  cout << "Scanned " << Squares.size() << " squares" << endl;
  for (auto& s : Squares) {
    cout << s.ToString() << endl;
  }

  cout << "Added " << AllColors.size() << " colors" << endl;
  cout << "Top Left Square is " << TopLeftIdx << endl;
  cout << "Border color index is " << BorderColorIdx << endl;
#endif

  TopEdgeColorList.resize(AllColors.size());
  LeftEdgeColorList.resize(AllColors.size());
  BottomEdgeColorList.resize(AllColors.size());
  RightEdgeColorList.resize(AllColors.size());

  for (int i = 0; i < Squares.size(); i++) {
    TopEdgeColorList[Squares[i].BorderColors[Top]].push_back(i);
    LeftEdgeColorList[Squares[i].BorderColors[Left]].push_back(i);
    BottomEdgeColorList[Squares[i].BorderColors[Bottom]].push_back(i);
    RightEdgeColorList[Squares[i].BorderColors[Right]].push_back(i);
  }

#ifdef DEBUG
  cout << "-- Printing TopEdgeColorList --" << endl;
  for (int i = 0; i < TopEdgeColorList.size(); i++) {
    cout << AllColors[i] << " : ";
    PrintIntVector(TopEdgeColorList[i]);
  }

  cout << "-- Printing LeftEdgeColorList --" << endl;
  for (int i = 0; i < LeftEdgeColorList.size(); i++) {
    cout << AllColors[i] << " : ";
    PrintIntVector(LeftEdgeColorList[i]);
  }

  cout << "-- Printing BottomEdgeColorList --" << endl;
  for (int i = 0; i < BottomEdgeColorList.size(); i++) {
    cout << AllColors[i] << " : ";
    PrintIntVector(BottomEdgeColorList[i]);
  }

  cout << "-- Printing RightEdgeColorList --" << endl;
  for (int i = 0; i < RightEdgeColorList.size(); i++) {
    cout << AllColors[i] << " : ";
    PrintIntVector(RightEdgeColorList[i]);
  }
#endif

  int sqrRtVal = sqrt(Squares.size());
  if (pow(sqrRtVal, 2) != Squares.size()) {
    cout << "Input squares do not form a perfect root" << endl;
    return 1;
  }

  vector<int> goodPerm = FindSolution();
  PrintGoodPerm(goodPerm);

  return 0;
}
