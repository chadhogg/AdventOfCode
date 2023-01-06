/// \file 2022Day23.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-23.


#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>

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

int
main (int argc, char* argv[])
{
  std::vector<Elf> initial = getInput ();

  return 0;
}

// My answers:
// Part 1: 149250
// Part 2: 12462
