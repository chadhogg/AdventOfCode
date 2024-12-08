/// \file 2024Day08.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-08.

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>
#include <cmath>

const char BLANK = '.';

using Grid = std::vector<std::vector<char>>;

Grid
readInput ()
{
  Grid grid;
  std::string line;
  while (std::getline (std::cin, line)) {
    grid.push_back (std::vector<char> (line.begin (), line.end ()));
  }
  return grid;
}

int
manhattan (int y, int x, int y2, int x2)
{
  return abs (y2 - y) + abs (x2 - x);
}

int
part1 (const Grid& grid)
{
  std::vector<bool> empty (grid.at (0).size (), false);
  std::vector<std::vector<bool>> hasAntinode (grid.size (), empty);

  for (int row = 0; row < (int)grid.size (); ++row) {
    for (int col  = 0; col < (int)grid.at (0).size (); ++col) {
      if (grid.at (row).at (col) != BLANK) {
        // (row, col) has an antenna
        for (int row2 = 0; row2 < (int)grid.size (); ++row2) {
          for (int col2 = 0; col2 < (int)grid.at (0).size (); ++col2) {
            if (grid.at (row2).at (col2) == grid.at (row).at (col)
            && (row != row2 || col != col2)) {
              // (row2, col2) has the same kind of antenna
              int rise = row - row2;
              int run = col - col2;
              std::vector<std::pair<int, int>> points = {{row + rise, col + run}, {row - rise, col - run}, {row2 + rise, col2 + run}, {row2 - rise, col2 - run}};
              for (std::pair<int, int> point : points) {
                int dist1 = manhattan (row, col, point.first, point.second);
                int dist2 = manhattan (row2, col2, point.first, point.second);
                if (std::max (dist1, dist2) == 2 * std::min (dist1, dist2)
                && point.first >= 0 && point.first < (int)grid.size ()
                && point.second >= 0 && point.second < (int)grid.at (0).size ()) {
                  hasAntinode[point.first][point.second] = true;
                }
              } 
            }
          }
        }
      }
    }
  }
  int count = 0;
  for (unsigned int y = 0; y < hasAntinode.size (); ++y) {
    for (unsigned int x = 0; x < hasAntinode.size (); ++x) {
      if (hasAntinode.at (y).at (x)) {
        ++count;
      }
    }
  }
  return count;
}

int
part2 (const Grid& grid)
{
  std::vector<bool> empty (grid.at (0).size (), false);
  std::vector<std::vector<bool>> hasAntinode (grid.size (), empty);

  for (int row = 0; row < (int)grid.size (); ++row) {
    for (int col  = 0; col < (int)grid.at (0).size (); ++col) {
      if (grid.at (row).at (col) != BLANK) {
        // (row, col) has an antenna
        for (int row2 = 0; row2 < (int)grid.size (); ++row2) {
          for (int col2 = 0; col2 < (int)grid.at (0).size (); ++col2) {
            if (grid.at (row2).at (col2) == grid.at (row).at (col)
            && (row != row2 || col != col2)) {
              // (row2, col2) has the same kind of antenna
              int rise = row - row2;
              int run = col - col2;
              bool anyInBounds = true;
              for (int mult = 1; anyInBounds; ++mult) {
                anyInBounds = false;
                std::vector<std::pair<int, int>> points = {{row + rise * mult, col + run * mult}, {row - rise * mult, col - run * mult}, {row2 + rise * mult, col2 + run * mult}, {row2 - rise * mult, col2 - run * mult}};
                for (std::pair<int, int> point : points) {
                  if (point.first >= 0 && point.first < (int)grid.size ()
                  && point.second >= 0 && point.second < (int)grid.at (0).size ()) {
                    hasAntinode[point.first][point.second] = true;
                    anyInBounds = true;
                  }
                } 
              }
            }
          }
        }
      }
    }
  }
  int count = 0;
  for (unsigned int y = 0; y < hasAntinode.size (); ++y) {
    for (unsigned int x = 0; x < hasAntinode.size (); ++x) {
      if (hasAntinode.at (y).at (x)) {
        ++count;
      }
    }
  }
  return count;
}


/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Grid grid = readInput ();
  std::cout << part1 (grid) << "\n";
  std::cout << part2 (grid) << "\n";
  return 0;
}
