/// \file 2022Day04.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-04.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using Range = std::pair<unsigned, unsigned>;

using Problem = std::vector<std::pair<Range, Range>>;

Problem
getInput ()
{
  Problem prob;
  unsigned a, b, c, d;
  while (scanf ("%u-%u,%u-%u", &a, &b, &c, &d) == 4)
  {
    prob.push_back ({ {a, b}, {c, d}});
  }
  return prob;
}

bool
contains (const Range& r1, const Range& r2)
{
  if (r1.first <= r2.first && r1.second >= r2.second) { return true; }
  return false;
}

unsigned
part1 (const Problem& prob)
{
  unsigned count = 0;
  for (const std::pair<Range, Range>& pair : prob)
  {
    if (contains (pair.first, pair.second) || contains (pair.second, pair.first)) { ++count; }
  }
  return count;
}

bool
overlaps (const Range& r1, const Range& r2)
{
  if (contains (r1, r2)
      || (r1.first >= r2.first && r1.first <= r2.second)
      || (r1.second >= r2.first && r1.second <= r2.second))
  {
    return true;
  }
  return false;
}

unsigned
part2 (const Problem& prob)
{
  unsigned count = 0;
  for (const std::pair<Range, Range>& pair : prob)
  {
    if (overlaps (pair.first, pair.second)) { ++count; }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  Problem prob = getInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";
  return 0;
}
