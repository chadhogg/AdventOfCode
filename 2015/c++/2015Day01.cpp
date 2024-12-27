/// \file 2015Day01.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-01.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::string
readInput ()
{
  std::string line;
  std::cin >> line;
  return line;
}

int
part1 (const std::string& input)
{
  int floor = 0;
  for (char c : input) {
    if (c == '(') { ++floor; }
    else if (c == ')') { --floor; }
    else { throw "Impossible"; }
  }
  return floor;
}

unsigned int
part2 (const std::string& input)
{
  int floor = 0;
  for (unsigned int index = 0; index < input.size (); ++index) {
    char c = input.at (index);
    if (c == '(') { ++floor; }
    else if (c == ')') { --floor; }
    else { throw "Impossible"; }
    if (floor < 0) { return index + 1; }
  }
  throw "Impossible";
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string input = readInput ();
  std::cout << part1 (input) << "\n";
  std::cout << part2 (input) << "\n";
  return 0;
}
