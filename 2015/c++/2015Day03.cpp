/// \file 2015Day03.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-03.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <map>

const char NORTH = '^';
const char EAST = '>';
const char SOUTH = 'v';
const char WEST = '<';

std::string
readInput ()
{
  std::string line;
  std::cin >> line;
  for (char c : line) { assert (c == NORTH || c == EAST || c == SOUTH || c == WEST ); }
  return line;
}

struct Position
{
  long row;
  long col;

  auto operator<=>(const Position&) const = default;
};

std::map<Position, long>
visit (const std::string& input)
{
  std::map<Position, long> visited;
  Position current = {0, 0};
  visited[current] = 1;
  for (char c : input) {
    if (c == NORTH) { --current.row; }
    else if (c == EAST) { ++current.col; }
    else if (c == SOUTH) { ++current.row; }
    else if (c == WEST) { --current.col; }
    if (visited.contains (current)) { visited[current] = visited[current] + 1; }
    else { visited[current] = 1; }
  }
  return visited;
}

long
part1 (const std::map<Position, long>& visited)
{
  return visited.size ();
}

long part2 (const std::string& input)
{
  std::string even, odd;
  for (unsigned int i = 0; i < input.size (); ++i) {
    if (i % 2 == 0) { even.push_back (input[i]); }
    else { odd.push_back (input[i]); }
  }
  std::map<Position, long> santa = visit (even);
  std::map<Position, long> robot = visit (odd);
  santa.merge (robot);
  return santa.size ();
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string input = readInput ();
  std::map<Position, long> visits = visit (input);
  std::cout << part1 (visits) << "\n";
  std::cout << part2 (input) << "\n";
  return 0;
}
