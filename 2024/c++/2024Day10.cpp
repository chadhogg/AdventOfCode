/// \file 2024Day10.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-10.

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

using Height = int;
using TMap = std::vector<std::vector<Height>>;
using Position = std::pair<int, int>;

const Height MIN_HEIGHT = 0;
const Height MAX_HEIGHT = 9;

TMap
readInput ()
{
  TMap map;
  std::string line;
  while (std::getline (std::cin, line)) {
    map.push_back ({});
    for (char c : line) {
      map.back ().push_back (c - '0');
    }
  }
  return map;
}

bool
isInBounds (const TMap& map, const Position& pos)
{
  return pos.first >= 0 && pos.first < (int)map.size () && pos.second >= 0 && pos.second < (int)map.at (0).size ();
}

int
countTrailEnds (const TMap& map, const Position& pos)
{
  assert (isInBounds (map, pos));
  assert (map.at (pos.first).at (pos.second) == MIN_HEIGHT);
  std::set<Position> ends;
  std::set<Position> frontier;
  frontier.insert (pos);
  while (!frontier.empty ()) {
    Position current = *frontier.begin ();
    frontier.erase (current);
    std::vector<Position> possible = {{current.first + 1, current.second}, {current.first - 1, current.second}, {current.first, current.second + 1}, {current.first, current.second - 1}};
    for (Position next : possible) {
      if (isInBounds (map, next) && map.at (next.first).at (next.second) == map.at (current.first).at (current.second) + 1) {
        if (map.at (next.first).at (next.second) == MAX_HEIGHT) {
          ends.insert (next);
        }
        else {
          frontier.insert (next);
        }
      }
    }
  }
  return ends.size ();
}

int
part1 (const TMap& map)
{
  int total = 0;
  for (int i = 0; i < (int)map.size (); ++i) {
    for (int j = 0; j < (int)map.at (0).size (); ++j) {
      if (map.at (i).at (j) == MIN_HEIGHT) {
        total += countTrailEnds (map, {i, j});
      }
    }
  }
  return total;
}

int
countTrails (const TMap& map, const Position& pos)
{
  assert (isInBounds (map, pos));
  assert (map.at (pos.first).at (pos.second) == MIN_HEIGHT);
  int count = 0;
  std::list<Position> frontier;
  frontier.push_back (pos);
  while (!frontier.empty ()) {
    Position current = frontier.back ();
    frontier.pop_back ();
    std::vector<Position> possible = {{current.first + 1, current.second}, {current.first - 1, current.second}, {current.first, current.second + 1}, {current.first, current.second - 1}};
    for (Position next : possible) {
      if (isInBounds (map, next) && map.at (next.first).at (next.second) == map.at (current.first).at (current.second) + 1) {
        if (map.at (next.first).at (next.second) == MAX_HEIGHT) {
          ++count;
        }
        else {
          frontier.push_back (next);
        }
      }
    }
  }
  return count;
}

int
part2 (const TMap& map)
{
  int total = 0;
  for (int i = 0; i < (int)map.size (); ++i) {
    for (int j = 0; j < (int)map.at (0).size (); ++j) {
      if (map.at (i).at (j) == MIN_HEIGHT) {
        total += countTrails (map, {i, j});
      }
    }
  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  TMap map = readInput ();
  std::cout << part1 (map) << "\n";
  std::cout << part2 (map) << "\n";
  return 0;
}
