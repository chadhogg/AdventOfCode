/// \file 2022Day23.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-23.


#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <set>
#include <list>

using Location = std::pair<long, long>;

bool
operator< (const Location& a, const Location& b)
{
  return a.first < b.first || (a.first == b.first && a.second < b.second);
}

struct Elf
{
  Location current;
  Location desired;
};

enum Direction {
  NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST
};

std::vector<Elf>
getInput ()
{
  std::vector<Elf> elves;
  std::string line;
  std::getline (std::cin, line);
  long row = 0;
  while (std::cin)
  {
    for (long col = 0; col < (long)line.size (); ++col)
    {
      assert (line[col] == '.' || line[col] == '#');
      if (line[col] == '#')
      {
        elves.push_back ({{row, col}, {row, col}});
      }
    }
    std::getline (std::cin, line);
    ++row;
  }
  return elves;
}

std::set<Direction>
findNeighbors(const std::set<Location>& locations, const Location& center)
{
  std::set<Direction> result;
  if (locations.count ({center.first - 1, center.second}) != 0) { result.insert (NORTH); }
  if (locations.count ({center.first - 1, center.second + 1}) != 0) { result.insert (NORTHEAST); }
  if (locations.count ({center.first, center.second + 1}) != 0) { result.insert (EAST); }
  if (locations.count ({center.first + 1, center.second + 1}) != 0) { result.insert (SOUTHEAST); }
  if (locations.count ({center.first + 1, center.second}) != 0) { result.insert (SOUTH); }
  if (locations.count ({center.first + 1, center.second - 1}) != 0) { result.insert (SOUTHWEST); }
  if (locations.count ({center.first, center.second - 1}) != 0) { result.insert (WEST); }
  if (locations.count ({center.first - 1, center.second - 1}) != 0) { result.insert (NORTHWEST); }
  return result;
}

void
draw (const std::vector<Elf> elves)
{
  long minRow = elves.at (0).current.first, 
    maxRow = elves.at (0).current.first, 
    minCol = elves.at (0).current.second, 
    maxCol = elves.at (0).current.second;
  std::set<Location> locations;
  for (const Elf& elf : elves)
  {
    locations.insert (elf.current);
    minRow = std::min (minRow, elf.current.first);
    maxRow = std::max (maxRow, elf.current.first);
    minCol = std::min (minCol, elf.current.second);
    maxCol = std::max (maxCol, elf.current.second);
  }
  for (long row = minRow; row <= maxRow; ++row)
  {
    for (long col = minCol; col <= maxCol; ++col)
    {
      if (locations.count ({row, col}) == 0) { std::cout << "."; }
      else { std::cout << "#"; }
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

void
doRound (std::vector<Elf>& elves, std::list<Direction>& priorities)
{
  std::set<Location> locations;
  for (const Elf& elf : elves)
  {
    locations.insert (elf.current);
  }
  for (Elf& elf : elves)
  {
    elf.desired = elf.current;
    std::set<Direction> neighbors = findNeighbors (locations, elf.current);
    if (neighbors.empty ()) { /* do nothing */ }
    else
    {
      for (Direction dir : priorities)
      {
        if (neighbors.count (dir) == 0 && neighbors.count ((Direction)((dir + 1) % 8)) == 0 && neighbors.count ((Direction)((dir + 7) % 8)) == 0)
        {
          if (dir == NORTH) { elf.desired = {elf.current.first - 1, elf.current.second}; }
          else if (dir == EAST) { elf.desired = {elf.current.first, elf.current.second + 1}; }
          else if (dir == SOUTH) { elf.desired = {elf.current.first + 1, elf.current.second}; }
          else { elf.desired = {elf.current.first, elf.current.second - 1}; }
          break;
        }
      }
    }
  }

  std::multiset<Location> goals;
  for (const Elf& elf : elves)
  {
    goals.insert (elf.desired);
  }

  for (Elf& elf : elves)
  {
    if (goals.count (elf.desired) == 1)
    {
      //std::cout << "(" << elf.current.first << ", " << elf.current.second << ") moves to (" << elf.desired.first << ", " << elf.desired.second << ")\n";
      elf.current = elf.desired;
    }
  }

  priorities.push_back (priorities.front ());
  priorities.pop_front ();
  //draw (elves);
}

unsigned long
countEmptyGroundTiles (const std::vector<Elf> elves)
{
  long minRow = elves.at (0).current.first, 
    maxRow = elves.at (0).current.first, 
    minCol = elves.at (0).current.second, 
    maxCol = elves.at (0).current.second;
  for (const Elf& elf : elves)
  {
    minRow = std::min (minRow, elf.current.first);
    maxRow = std::max (maxRow, elf.current.first);
    minCol = std::min (minCol, elf.current.second);
    maxCol = std::max (maxCol, elf.current.second);
  }
  return (maxRow - minRow + 1) * (maxCol - minCol + 1) - elves.size ();
}

int
main (int argc, char* argv[])
{
  std::vector<Elf> initial = getInput ();
  std::list<Direction> priorities = {NORTH, SOUTH, WEST, EAST};
  //draw (initial);
  for (int i = 0; i < 10; ++i) { doRound (initial, priorities); }
  std::cout << countEmptyGroundTiles (initial) << "\n";
  return 0;
}

// My answers:
// Part 1: 3966
// Part 2: 
