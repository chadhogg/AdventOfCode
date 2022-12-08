/// \file 2022Day08.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-08.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>

using Grid = std::vector<std::vector<unsigned>>;

Grid
getInput ()
{
  Grid grid;
  std::string line;
  while (std::getline (std::cin, line))
  {
    if (line.empty ()) { continue; }
    std::vector<unsigned> row;
    for (char c : line) {
      row.push_back (c - '0');
    }
    grid.push_back (row);
  }
  return grid;
}

bool
isVisible (const Grid& grid, int row, int col)
{
  unsigned height = grid[row][col];
  bool success = true;
  for (int i = row - 1; i >= 0; --i)
  {
    if (grid[i][col] >= height) { success = false; }
  }
  if (success) { return true; }

  success = true;
  for (std::size_t i = row + 1; i < grid.size (); ++i)
  {
    if (grid[i][col] >= height) { success = false; }
  }
  if (success) { return true; }

  success = true;
  for (int i = col - 1; i >= 0; --i)
  {
    if (grid[row][i] >= height) { success = false; }
  }
  if (success) { return true; }

  success = true;
  for (std::size_t i = col + 1; i < grid[row].size (); ++i)
  {
    if (grid[row][i] >= height) { success = false; }
  }
  if (success) { return true; }

  return false;
}

unsigned
countVisible (const Grid& grid)
{
  unsigned count = 0;
  for (std::size_t i = 0; i < grid.size (); ++i)
  {
    for (std::size_t k = 0; k < grid[i].size (); ++k)
    {
      if (isVisible (grid, i, k)) { ++count; }
    }
  }
  return count;
}

unsigned
computeScenicScore (const Grid& grid, int row, int col)
{
  unsigned result = 1;
  unsigned height = grid[row][col];
  
  int count = 0;
  for (int i = row - 1; i >= 0; --i)
  {
    ++count;
    if (grid[i][col] >= height) {
      break;
    }
  }
  result *= count;

  count = 0;
  for (std::size_t i = row + 1; i < grid.size (); ++i)
  {
    ++count;
    if (grid[i][col] >= height) {
      break;
    }
  }
  result *= count;

  count = 0;
  for (int i = col - 1; i >= 0; --i)
  {
    ++count;
    if (grid[row][i] >= height) {
      break;
    }
  }
  result *= count;

  count = 0;
  for (std::size_t i = col + 1; i < grid[row].size (); ++i)
  {
    ++count;
    if (grid[row][i] >= height) {
      break;
    }
  }
  result *= count;

  return result;
}

unsigned
getMaxScenicScore (const Grid& grid)
{
  unsigned best = 0;
  for (std::size_t i = 0; i < grid.size (); ++i)
  {
    for (std::size_t j = 0; j < grid[i].size (); ++j)
    {
      best = std::max (best, computeScenicScore (grid, i, j));
    }
  }
  return best;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  Grid grid = getInput ();
  std::cout << countVisible (grid) << "\n";
  std::cout << getMaxScenicScore (grid) << "\n";
  return 0;
}
