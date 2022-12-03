/// \file 2022Day03.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-03.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using Problem = std::vector<std::string>;

Problem
getInput ()
{
  Problem prob;
  std::string word;
  while (std::cin)
  {
    std::cin >> word;
    prob.push_back (word);
  }
  prob.pop_back ();
  return prob;
}

char
itemInBoth (std::string line)
{
  std::string first = line.substr (0, line.size () / 2);
  std::string second = line.substr (line.size () / 2);
  for (char c : first)
  {
    if (second.find (c) != std::string::npos) { return c; }
  }
  return 0;
}

int
itemToPriority (char item)
{
  if (::islower (item)) { return item - 'a' + 1; }
  else { return item - 'A' + 27; }
}

int
part1 (const Problem& prob)
{
  int total = 0;
  for (const std::string& line : prob)
  {
    total += itemToPriority (itemInBoth (line));
  }
  return total;
}

char
onlyInCommon (const std::string& a, const std::string& b, const std::string& c)
{
  for (char ch : a)
  {
    if (b.find (ch) != std::string::npos && c.find (ch) != std::string::npos)
    {
      return ch;
    }
  }
  return 0;
}

int
part2 (const Problem& prob)
{
  int total = 0;
  for (unsigned index = 0; index < prob.size (); index += 3)
  {
    total += itemToPriority (onlyInCommon (prob[index], prob[index + 1], prob[index + 2]));
  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  Problem prob = getInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";
  return 0;
}
