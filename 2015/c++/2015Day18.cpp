/// \file 2015Day18.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-18.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <array>

const std::size_t TRUE_ROWS = 100;
const std::size_t TRUE_COLS = 100;
const std::size_t FAKE_ROWS = TRUE_ROWS + 2;
const std::size_t FAKE_COLS = TRUE_COLS + 2;

using Grid = std::array<std::array<bool, FAKE_COLS>, FAKE_ROWS>;

Grid
readInput ()
{
  Grid grid;
  for (std::size_t row = 0; row < FAKE_ROWS; ++row) {
    for (std::size_t col = 0; col < FAKE_COLS; ++col) {
      grid[row][col] = false;
    }
  }
  std::string line;
  for (std::size_t row = 0; row < TRUE_ROWS; ++row) {
    std::getline (std::cin, line);
    assert (line.size () == TRUE_COLS);
    for (std::size_t col = 0; col < TRUE_COLS; ++col) {
      assert (line[col] == '#' || line[col] == '.');
      grid[row + 1][col + 1] = line[col] == '#';
    }
  }
  return grid;
}

std::ostream&
operator<< (std::ostream& out, const Grid& grid)
{
  for (std::size_t row = 1; row < FAKE_ROWS - 1; ++row) {
    for (std::size_t col = 1; col < FAKE_COLS - 1; ++col) {
      out << (grid.at (row).at (col) ? '#' : '.');
    }
    out << "\n";
  }
  return out;
}

int
countNeighbors (const Grid& grid, int row, int col)
{
  int count = 0;
  for (int r = row - 1; r <= row + 1; ++r) {
    for (int c = col - 1; c <= col + 1; ++c) {
      if ((r != row || c != col) && grid[r][c]) { ++count; }
    }
  }
  return count;
}

bool
isCorner (int row, int col) {
  return (row == 1 || row == FAKE_ROWS - 2) && (col == 1 || col == FAKE_COLS - 2);
}

Grid
update (const Grid& grid, const std::set<int>& stayOn, const std::set<int>& turnOn, bool stuckCorners)
{
  Grid copy = grid;
  for (std::size_t row = 1; row < FAKE_ROWS - 1; ++row) {
    for (std::size_t col = 1; col < FAKE_COLS - 1; ++col) {
      int count = countNeighbors (grid, row, col);
      if (grid.at (row).at (col)) {
        if (!stuckCorners || !isCorner (row, col)) {
          if (!stayOn.contains (count)) { copy[row][col] = false; }
        }
      }
      else {
        if (turnOn.contains (count)) { copy[row][col] = true; }
      }
    }
  }
  return copy;
}

Grid
multiUpdate (const Grid& grid, const std::set<int>& stayOn, const std::set<int>& turnOn, int times, bool stuckCorners)
{
  Grid current = grid;
  if (stuckCorners) {
    current[1][1] = true;
    current[1][FAKE_COLS - 2] = true;
    current[FAKE_ROWS - 2][1] = true;
    current[FAKE_ROWS - 2][FAKE_COLS - 2] = true;
  }
  for (int i = 0; i < times; ++i) {
    current = update (current, stayOn, turnOn, stuckCorners);
  }
  return current;
}

int
countOn (const Grid& grid)
{
  int count = 0;
  for (std::size_t row = 1; row < FAKE_ROWS - 1; ++row) {
    for (std::size_t col = 1; col < FAKE_COLS - 1; ++col) {
      if (grid.at (row).at (col)) { ++count; }
    }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Grid initial = readInput ();
  Grid after100 = multiUpdate (initial, {2, 3}, {3}, 100, false);
  std::cout << countOn (after100) << "\n";
  std::cout << countOn (multiUpdate (initial, {2, 3}, {3}, 100, true)) << "\n";
  return 0;
}
