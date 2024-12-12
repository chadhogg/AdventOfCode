/// \file 2024Day12.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-12.

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>
#include <cmath>
#include <utility>
#include <map>

using PlantType = char;
using Grid = std::vector<std::vector<char>>;
using Location = std::pair<unsigned int, unsigned int>;
using Region = std::pair<PlantType, std::set<Location>>;

Grid
readInput ()
{
  Grid grid;
  std::string line;
  while (std::getline (std::cin, line)) {
    grid.push_back ({line.begin (), line.end ()});
  }
  return grid;
}

Region
floodFill (const Grid& grid, const Location& start)
{
  Region region;
  region.first = grid.at (start.first).at (start.second);
  std::set<Location> frontier;
  frontier.insert (start);
  while (!frontier.empty ()) {
    Location current = *frontier.begin ();
    frontier.erase (current);
    std::vector<Location> neighbors;
    if (current.first != 0) { neighbors.push_back ({current.first - 1, current.second}); }
    if (current.first != grid.size () - 1) { neighbors.push_back ({current.first + 1, current.second}); }
    if (current.second != 0) { neighbors.push_back ({current.first, current.second - 1}); }
    if (current.second != grid.at (0).size () - 1) { neighbors.push_back ({current.first, current.second + 1}); }
    for (const Location& loc: neighbors) {
      if (grid.at (loc.first).at (loc.second) == region.first && region.second.count (loc) == 0) {
        frontier.insert (loc);
      }
    }
    region.second.insert (current);
  }
  return region;
}

std::vector<Region>
findRegions (const Grid& grid)
{
  std::vector<Region> regions;
  std::set<Location> alreadyDone;

  for (unsigned int row = 0; row < grid.size (); ++row) {
    for (unsigned int col = 0; col < grid.at (row).size (); ++col) {
      if (alreadyDone.count ({row, col}) == 0) {
        Region region = floodFill (grid, {row, col});
        for (const Location& loc : region.second) {
          assert (alreadyDone.count (loc) == 0);
          alreadyDone.insert (loc);
        }
        regions.push_back (region);
      }
    }
  }
  return regions;
}

int
findArea (const Region& region)
{
  return region.second.size ();
}

bool
isNorthOf (const Location& a, const Location& b)
{
  return (a.first == b.first + 1 && a.second == b.second);
}

bool
isSouthOf (const Location& a, const Location& b)
{
  return (a.first == b.first - 1 && a.second == b.second);
}

bool
isWestOf (const Location& a, const Location& b)
{
  return (a.first == b.first && a.second == b.second - 1);
}

bool
isEastOf (const Location& a, const Location& b)
{
  return (a.first == b.first && a.second == b.second + 1);
}

bool
touches (const Location& a, const Location& b)
{
  return (isNorthOf (a, b) || isSouthOf (a, b) || isWestOf (a, b) || isEastOf (a, b));
}

int
findPerimeter (const Region& region)
{
  int perimeter = 0;
  for (const Location& a : region.second) {
    int neighbors = 0;
    for (const Location& b : region.second) {
      if (touches (a, b)) { neighbors++; } 
    }
    perimeter += 4 - neighbors;
  }
  return perimeter;
}

int
part1 (const Grid& grid)
{
  std::vector<Region> regions = findRegions(grid);
  int total = 0;
  for (const Region& region : regions) {
    total += findArea (region) * findPerimeter (region);
  }
  return total;
}

int
countDisconnectedParts (const std::set<unsigned int> cols)
{
  // this should work because I am using an (ordered) set
  int count = 0;
  unsigned int prev = __UINT16_MAX__;
  for (const unsigned int& next : cols) {
    if (next != prev + 1) { ++count; }
    prev = next;
  }
  return count;
}

int
countDisconnectedVertical (const std::set<Location> outliers)
{
  int count = 0;
  std::map<unsigned int, std::set<unsigned int>> rows;
  for (const Location& loc : outliers) {
    if (rows.count (loc.first) == 0) {
      rows.insert ({});
    }
    rows[loc.first].insert (loc.second);
  }

  for (auto it = rows.begin (); it != rows.end (); ++it) {
    count += countDisconnectedParts (it->second);
  }
  return count;
}

std::set<Location>
flip (const std::set<Location> locs) {
  std::set<Location> result;
  for (const Location& loc : locs) {
    result.insert ({loc.second, loc.first});
  }
  return result;
}

int
findNumSides (const Region& region)
{
  // All the plots with no northern neighbor can be divided by row, then see how many disconnected segments there are.
  // The same applies to not having a neighbor on each of the other sides. 
  std::set<Location> noNorthernNeighbor;
  std::set<Location> noSouthernNeighbor;
  std::set<Location> noWesternNeighbor;
  std::set<Location> noEasternNeighbor;
  for (const Location& a : region.second) {
    bool hasNorthernNeighbor = false;
    bool hasSouthernNeighbor = false;
    bool hasWesternNeighbor = false;
    bool hasEasternNeighbor = false;
    for (const Location& b: region.second) {
      if (isNorthOf (b, a)) { hasNorthernNeighbor = true; }
      else if (isSouthOf (b, a)) { hasSouthernNeighbor = true; }
      else if (isWestOf (b, a)) { hasWesternNeighbor = true; }
      else if (isEastOf (b, a)) { hasEasternNeighbor = true; }
    }
    if (!hasNorthernNeighbor) { noNorthernNeighbor.insert (a); }
    if (!hasSouthernNeighbor) { noSouthernNeighbor.insert (a); }
    if (!hasWesternNeighbor) { noWesternNeighbor.insert (a); }
    if (!hasEasternNeighbor) { noEasternNeighbor.insert (a); }
  }

  return countDisconnectedVertical (noNorthernNeighbor) + countDisconnectedVertical (noSouthernNeighbor)
  + countDisconnectedVertical (flip (noWesternNeighbor)) + countDisconnectedVertical (flip (noEasternNeighbor));
}

int
part2 (const Grid& grid)
{
  std::vector<Region> regions = findRegions(grid);
  int total = 0;
  for (const Region& region : regions) {
    total += findArea (region) * findNumSides (region);
  }
  return total;
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
