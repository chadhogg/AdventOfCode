/// \file 2024Day06.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-06.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>

const char OPEN = '.';
const char WALL = '#';
const char NORTH = '^';
const char EAST = '>';
const char SOUTH = 'v';
const char WEST = '<';

const char UNVISITED = 'Z';
const char VISITED = 'X';

using Map = std::vector<std::vector<char>>;
using Location = std::pair<int, int>;
using Guard = std::pair<Location, char>;
using Problem = std::pair<Map, Guard>;

Problem
readInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    prob.first.push_back (std::vector<char> (line.begin (), line.end ()));
  }
  for (int i = 0; i < (int)prob.first.size (); ++i) {
    for (int j = 0; j < (int)prob.first.at (i).size (); ++j) {
      if (prob.first.at (i).at (j) == NORTH || prob.first.at (i).at (j) == EAST
      || prob.first.at (i).at (j) == SOUTH || prob.first.at (i).at (j) == WEST) {
        prob.second.first = {i, j};
        prob.second.second = prob.first.at (i).at (j);
        prob.first[i][j] = OPEN;
      }
    }
  }
  return prob;
}

bool
isInBounds (const Map& map, const Location& loc)
{
  return loc.first >= 0 && loc.second >= 0 && loc.first < (int)map.size () && loc.second < (int)map.at (0).size ();
}

bool
isOpenOrOutOfBounds (const Map& map, const Location& loc) {
  return !isInBounds (map, loc) || map.at (loc.first).at (loc.second) == OPEN;
}

int
part1 (const Problem& prob)
{
  std::vector<char> blank (prob.first.at (0).size (), UNVISITED);
  Map visited = {prob.first.size (), blank};
  Guard current = prob.second;
  while (isInBounds (prob.first, current.first)) {
    visited[current.first.first][current.first.second] = VISITED;
    Guard next = current;
    switch (current.second) {
      case NORTH: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first - 1, next.first.second})) {
          next.first.first = next.first.first - 1;
        }
        else {
          next.second = EAST;
        }
        break;
      }
      case EAST: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first, next.first.second + 1})) {
          next.first.second = next.first.second + 1;
        }
        else {
          next.second = SOUTH;
        }
        break;
      }
      case SOUTH: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first + 1, next.first.second})) {
          next.first.first = next.first.first + 1;
        }
        else {
          next.second = WEST;
        }
        break;
      }
      case WEST: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first, next.first.second - 1})) {
          next.first.second = next.first.second - 1;
        }
        else {
          next.second = NORTH;
        }
        break;
      }
      default:
      assert (false);
    }
    current = next;
  }
  int count = 0;
  for (int i = 0; i < (int)visited.size (); ++i) {
    for (int j = 0; j < (int)visited.at (i).size (); ++j) {
      if (visited.at (i).at (j) == VISITED) { ++count; }
    }
  }
  return count;
}

bool
entersCycle (const Problem& prob)
{
  std::vector<char> blank (prob.first.at (0).size (), UNVISITED);
  Map visited = {prob.first.size (), blank};
  std::vector<std::set<char>> blank2 (prob.first.at (0).size (), std::set<char> ());
  std::vector<std::vector<std::set<char>>> directions = {prob.first.size (), blank2};
  Guard current = prob.second;
  while (isInBounds (prob.first, current.first)) {
    if (directions.at (current.first.first).at (current.first.second).count (current.second) > 0) {
      // We are at a place we have been before, facing a direction we were facing when we were there.
      return true;
    }
    directions[current.first.first][current.first.second].insert (current.second);
    visited[current.first.first][current.first.second] = VISITED;
    Guard next = current;
    switch (current.second) {
      case NORTH: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first - 1, next.first.second})) {
          next.first.first = next.first.first - 1;
        }
        else {
          next.second = EAST;
        }
        break;
      }
      case EAST: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first, next.first.second + 1})) {
          next.first.second = next.first.second + 1;
        }
        else {
          next.second = SOUTH;
        }
        break;
      }
      case SOUTH: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first + 1, next.first.second})) {
          next.first.first = next.first.first + 1;
        }
        else {
          next.second = WEST;
        }
        break;
      }
      case WEST: {
        if (isOpenOrOutOfBounds (prob.first, {next.first.first, next.first.second - 1})) {
          next.first.second = next.first.second - 1;
        }
        else {
          next.second = NORTH;
        }
        break;
      }
      default:
      assert (false);
    }
    current = next;
  }
  return false;
}

int
part2BruteForce (const Problem& prob)
{
  int count = 0;
  Problem copy (prob);
  for (int i = 0; i < (int)copy.first.size (); ++i) {
    for (int j = 0; j < (int)copy.first.at (i).size (); ++j) {
      if (copy.first.at (i).at (j) == OPEN) {
        copy.first[i][j] = WALL;
        if (entersCycle (copy)) { ++count; }
        copy.first[i][j] = OPEN;
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
  Problem prob = readInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2BruteForce (prob) << "\n";
  return 0;
}
