/// \file 2016Day03.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-03.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <array>

using SideLengths = std::array<int, 3>;

std::vector<SideLengths>
readInput ()
{
  std::vector<SideLengths> results;
  SideLengths current;
  while (std::cin >> current[0] >> current[1] >> current[2]) {
    results.push_back (current);
  }
  return results;
}

bool
couldBeTriangle (const SideLengths& tri)
{
  return tri[0] + tri[1] > tri[2] && tri[0] + tri[2] > tri[1] && tri[1] + tri[2] > tri[0];
}

int
countPossibleTriangles (const std::vector<SideLengths>& things)
{
  int count = 0;
  for (const SideLengths& thing : things) {
    if (couldBeTriangle (thing)) { ++count; }
  }
  return count;
}

int
countPossibleTriangles2 (const std::vector<SideLengths>& things)
{
  int count = 0;
  for (std::size_t row = 0; row < things.size (); row += 3) {
    for (std::size_t col = 0; col < 3; col++) {
      if (couldBeTriangle ({things[row][col], things[row + 1][col], things[row + 2][col]})) { ++count; }
    }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<SideLengths> things = readInput ();
  std::cout << countPossibleTriangles (things) << "\n";
  std::cout << countPossibleTriangles2 (things) << "\n";
  return 0;
}
