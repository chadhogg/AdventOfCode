/// \file 2024Day18.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-18.


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
#include <optional>
#include <chrono>
#include <thread>
#include <queue>

const char TILE_WALL = '#';
const char TILE_FLOOR = '.';
const char TILE_START = 'S';
const char TILE_END = 'E';

const char DIR_NORTH = '^';
const char DIR_EAST = '>';
const char DIR_SOUTH = 'v';
const char DIR_WEST = '<';

using Grid = std::vector<std::vector<char>>;

struct Location
{
  int y;
  int x;

  auto operator<=>(const Location& other) const = default;
};

Location operator+ (const Location& a, const Location& b)
{
  return {a.y + b.y, a.x + b.x};
}

Location operator- (const Location& a, const Location& b)
{
  return {a.y - b.y, a.x - b.x};
}

std::ostream&
operator<< (std::ostream& out, const Grid& grid)
{
  for (unsigned int y = 0; y < grid.size (); ++y) {
    for (unsigned int x = 0; x < grid.at (y).size (); ++x) {
      out << grid.at (y).at (x);
    }
    out << "\n";
  }
  return out;
}

struct Problem
{
  std::vector<Location> bytes;
  Location start;
  Location end;
};

const bool SAMPLE = false;

Problem
readInput ()
{
  Problem prob;
  int x, y;
  while (scanf ("%d,%d", &x, &y) == 2) {
    prob.bytes.push_back ({y, x});
    scanf ("\n");
  }
  prob.start = {0, 0};
  prob.end = (SAMPLE ? Location {6, 6} : Location {70, 70});
  return prob;
}

struct LocationWithCost
{
  Location loc;
  int cost;
  int estimate;
  bool operator< (const LocationWithCost& other) const
  {
    return cost + estimate < other.cost + other.estimate || (cost == other.cost && loc < other.loc);
  }
  bool operator== (const LocationWithCost& other) const
  {
    return cost + estimate == other.cost + other.estimate && loc == other.loc;
  }
  bool operator> (const LocationWithCost& other) const
  {
    return !(*this < other || *this == other);
  }
};

int
manhattan (const Location& a, const Location& b)
{
  return std::abs (a.y - b.y) + std::abs (a.x - b.x);
}

void
draw (const Problem& prob, const std::set<Location>& corrupted)
{
  for (int y = 0; y <= prob.end.y; ++y) {
    for (int x = 0; x <= prob.end.x; ++x) {
      if (corrupted.count ({y, x}) == 1) {
        std::cout << "#";
      }
      else {
        std::cout << ".";
      }
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

const int INVALID = -1;

int
searchAStar (const Problem& prob, int bytesToDrop)
{
  std::set<Location> corrupted (prob.bytes.begin (), prob.bytes.begin () + bytesToDrop);
//  std::cout << "Searching after " << bytesToDrop << " fell.\n";
//  draw (prob, corrupted);
  std::priority_queue<LocationWithCost, std::vector<LocationWithCost>, std::greater<LocationWithCost>> frontier;
  frontier.push ({prob.start, 0, manhattan (prob.start, prob.end)});
  std::set<Location> visited;
  while (!frontier.empty ()) {
    LocationWithCost current = frontier.top ();
    frontier.pop ();
    if (current.loc == prob.end) {
      return current.cost;
    }
    if (visited.count (current.loc) > 0) { continue; }
    visited.insert (current.loc);
    std::vector<Location> successors = {{current.loc.y + 1, current.loc.x}, {current.loc.y - 1, current.loc.x}, {current.loc.y, current.loc.x + 1}, {current.loc.y, current.loc.x - 1}};
    for (Location& next : successors) {
      if (next.y >= 0 && next.y <= prob.end.y && next.x >= 0 && next.x <= prob.end.x) {
        if (corrupted.count (next) == 0) {
          if (visited.count (next) == 0) {
            frontier.push ({next, current.cost + 1, manhattan (next, prob.end)});
          }
        }
      }
    }
  }
  return INVALID;
}

int
tryToBlock (const Problem& prob, int start)
{
  // Sure, a linear search is slow.  Do I look like I care?
  for (int i = start; i < (int)prob.bytes.size (); ++i) {
    int result = searchAStar (prob, i);
    // NOTE: No idea why I should subtract 1 here ... but it works.
    if (result == INVALID) { return i - 1; }
  }
  return INVALID;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  std::cout << searchAStar (prob, (SAMPLE ? 12 : 1024)) << "\n";
  int index = tryToBlock (prob, (SAMPLE ? 12 : 1024));
  std::cout << prob.bytes.at (index).x << "," << prob.bytes.at (index).y << "\n";
  return 0;
}
