/// \file 2025Day09.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-09.

#include <cstddef>
#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include <algorithm>
#include <map>

using Number = long;
using Tile = std::pair<Number, Number>;

using Problem = std::vector<Tile>;

Problem
getInput ()
{
  Problem prob;
  long x, y;
  while (scanf ("%ld,%ld", &x, &y) == 2) {
    prob.push_back ({x, y});
  }
  return prob;
}

Number
area (const Tile& t1, const Tile& t2)
{
  return (std::abs (t1.first - t2.first) + 1) * (std::abs (t1.second - t2.second) + 1);
}

Number
findLargestArea (const Problem& prob)
{
  Number highest = 0;
  for (std::size_t index1 = 0; index1 < prob.size (); ++index1) {
    for (std::size_t index2 = index1 + 1; index2 < prob.size (); ++index2) {
      Number current = area (prob.at (index1), prob.at (index2));
      highest = std::max (highest, current);
    }
  }
  return highest;
}

const char RED = 'R';
const char GREEN = 'G';
const char OTHER = 'O';

using SparseGrid = std::set<Tile>;

SparseGrid
createGrid (const Problem& prob)
{
  SparseGrid grid;
  std::vector<Tile> red = prob;
  red.push_back (red[0]);
  for (std::size_t index = 0; index < prob.size (); ++index) {
    Tile current = red[index];
    Tile next = red[index + 1];
    grid.insert (current);
    while (current != next) {
      if (current.first == next.first && current.second < next.second) { ++current.second; }
      else if (current.first == next.first && current.second > next.second) { --current.second; }
      else if (current.first < next.first && current.second == next.second) { ++current.first; }
      else if (current.first > next.first && current.second == next.second) { --current.first; }
      else { assert (false); }
      grid.insert (current);
    }
  }
  return grid;
}

void
floodFillGrid (const Problem& prob, SparseGrid& grid, const Tile& start)
{
  Number minX = prob.at (0).first, maxX = prob.at (0).first, minY = prob.at (0).second, maxY = prob.at (0).second;
  for (const Tile& tile : prob) {
    minX = std::min (minX, tile.first);
    maxX = std::max (maxX, tile.first);
    minY = std::min (minY, tile.second);
    maxY = std::max (maxY, tile.second);
  }
  std::set<Tile> frontier;
  frontier.insert (start);
  while (!frontier.empty ()) {
    Tile current = *frontier.begin ();
    frontier.erase (frontier.begin ());
    grid.insert (current);
    std::vector<Tile> neighbors;
    neighbors.push_back ({current.first + 1, current.second});
    neighbors.push_back ({current.first - 1, current.second});
    neighbors.push_back ({current.first, current.second + 1});
    neighbors.push_back ({current.first, current.second - 1});
    for (Tile& neighbor : neighbors) {
      if (neighbor.first < minX || neighbor.first > maxX || neighbor.second < minY || neighbor.second > maxY) {
        grid.clear ();
        return;
      }
      if (grid.count (neighbor) == 0 && frontier.count (neighbor) == 0) {
        frontier.insert (neighbor);
      }
    }    
  }
}

bool
isCompletelyFilled (const SparseGrid& grid, const Tile& first, const Tile& second) {
  for (Number x = std::min (first.first, second.first); x <= std::max (first.first, second.first); ++x) {
    for (Number y = std::min (first.second, second.second); y <= std::max (first.second, second.second); ++y) {
      if (grid.count ({x, y}) == 0) { return false;}
    }
  }
  return true;
}

// Sadly, the sparse grid gets very, very large.  (It's not really that sparse, is it?)
Number
part2 (const Problem& prob)
{
  SparseGrid outline = createGrid (prob);
  std::vector<Tile> startingPoints;
  startingPoints.push_back ({prob.at (0).first - 1, prob.at (0).second - 1});
  startingPoints.push_back ({prob.at (0).first - 1, prob.at (0).second + 1});
  startingPoints.push_back ({prob.at (0).first + 1, prob.at (0).second + 1});
  startingPoints.push_back ({prob.at (0).first + 1, prob.at (0).second - 1});
  SparseGrid complete;
  for (Tile start : startingPoints) {
    SparseGrid candidate = outline;
    floodFillGrid( prob, candidate, start);
    if (!candidate.empty ()) {
      complete = candidate;
      break;
    }
  }
  assert (!complete.empty ());

  Number highest = 0;
  for (std::size_t index1 = 0; index1 < prob.size (); ++index1) {
    for (std::size_t index2 = index1 + 1; index2 < prob.size (); ++index2) {
      if (isCompletelyFilled (complete, prob.at (index1), prob.at (index2))) {
        Number current = area (prob.at (index1), prob.at (index2));
        highest = std::max (highest, current);
      }
    }
  }
  return highest;
}

bool
interiorContainsBorderOfAnother (const Tile& a, const Tile& b, const SparseGrid& outline)
{
  for (Number x = std::min (a.first, b.first) + 1; x <= std::max (a.first, b.first) - 1; ++x) {
    for (Number y = std::min (a.second, b.second) + 1; y <= std::max (a.second, b.second) - 1; ++y) {
      if (outline.count ({x, y}) != 0) { return true; }
    }
  }
  return false;
}

bool
interiorContainsBorderOfAnother2 (const Tile& a, const Tile& b, const SparseGrid& outline)
{
  Number minX = std::min (a.first, b.first);
  Number maxX = std::max (a.first, b.first);
  Number minY = std::min (a.second, b.second);
  Number maxY = std::max (a.second, b.second);
  for (const Tile & tile : outline) {
    if (minX < tile.first && tile.first < maxX && minY < tile.second && tile.second < maxY) { return true; }
  }
  return false;
}

// This uses a modest amount of memory, but still takes absolutely forever.
Number
part2attempt2 (const Problem& prob)
{
  SparseGrid outline = createGrid (prob);
  Number highest = 0;
  for (std::size_t index1 = 0; index1 < prob.size (); ++index1) {
    for (std::size_t index2 = index1 + 1; index2 < prob.size (); ++index2) {
      if (!interiorContainsBorderOfAnother (prob.at (index1), prob.at (index2), outline)) {
        Number current = area (prob.at (index1), prob.at (index2));
        highest = std::max (highest, current);
      }
    }
  }
  return highest;
}

// New idea: rewrite the problem so that the only coordinates that matter are the ones with red tiles
// Use that version to determine intersections, but the original for determining areas.
Number
part2attempt3 (const Problem& prob)
{
  Problem compressedProblem;
  std::set<Number> xValuesSet;
  std::set<Number> yValuesSet;
  std::vector<Number> xValues;
  std::vector<Number> yValues;
  std::map<Number, std::size_t> xValuesMap;
  std::map<Number, std::size_t> yValuesMap;
  for (const Tile& tile : prob) {
    xValuesSet.insert (tile.first);
    yValuesSet.insert (tile.second);
  }
  for (Number x : xValuesSet) {
    xValuesMap[x] = xValues.size ();
    xValues.push_back (x);
    // Need to add something here, doesn't actually matter what it is, to leave space to be filled.
    xValues.push_back (x);
  }
  for (Number y : yValuesSet) {
    yValuesMap[y] = yValues.size ();
    yValues.push_back (y);
    yValues.push_back (y);
  }
  for (const Tile& tile : prob) {
    compressedProblem.push_back ({xValuesMap[tile.first], yValuesMap[tile.second]});
  }

  SparseGrid outline = createGrid (compressedProblem);
  Number highest = 0;
  for (std::size_t index1 = 0; index1 < prob.size (); ++index1) {
    for (std::size_t index2 = index1 + 1; index2 < prob.size (); ++index2) {
      if (!interiorContainsBorderOfAnother (compressedProblem.at (index1), compressedProblem.at (index2), outline)) {
        Number current = area (prob.at (index1), prob.at (index2));
        highest = std::max (highest, current);
      }
    }
  }
  return highest;

}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::cout << findLargestArea (prob) << "\n";
  std::cout << part2attempt3 (prob) << "\n";
  return EXIT_SUCCESS;
}

