/// \file 2015Day25.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-25.


#include <iostream>
#include <cassert>

struct Position
{
  int row;
  int col;
  auto operator<=> (const Position& pos) const = default;
};

const long long START_VALUE = 20151125;
const long long MULTIPLICAND = 252533;
const long long DIVISOR = 33554393;


Position readInput ()
{
  Position pos;
  if (scanf ("To continue, please consult the code grid in the manual.  Enter the code at row %d, column %d.", &pos.row, &pos.col) != 2) {
    throw "Uh-oh!";
  }
  return pos;
}

/*
int
posToNumber (const Position& pos)
{
  // 1x1 => 1
  // 2x1 => 2
  // 1x2 => 3
  // 3x1 => 4
  // 2x2 => 5
  // 1x3 => 6
  // 4x1 => 7
  // Not seeing the pattern here.  Whatever; don't need it.
}
*/

long long
compute (const Position& desired, long long start)
{
  Position currentPos = {1, 1};
  long long currentVal = start;
  while (currentPos != desired) {
    if (currentPos.row == 1) {
      currentPos = {currentPos.col + 1, 1};
    }
    else {
      currentPos = {currentPos.row - 1, currentPos.col + 1};
    }
    currentVal = (currentVal * MULTIPLICAND) % DIVISOR;
  }
  return currentVal;
}


/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Position pos = readInput ();
  assert (compute ({1, 1}, START_VALUE) == START_VALUE);
  assert (compute ({2, 1}, START_VALUE) == 31916031);
  assert (compute ({1, 2}, START_VALUE) == 18749137);
  assert (compute ({3, 1}, START_VALUE) == 16080970);
  assert (compute ({2, 2}, START_VALUE) == 21629792);
  assert (compute ({1, 3}, START_VALUE) == 17289845);
  std::cout << compute (pos, START_VALUE) << "\n";
  return 0;
}
