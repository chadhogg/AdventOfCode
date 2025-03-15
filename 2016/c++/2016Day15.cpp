/// \file 2016Day15.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2016-12-15.

#include <iostream>
#include <string>
#include <cassert>
#include <vector>

struct Disc
{
  int number;
  int numPositions;
  int initial;
};

std::vector<Disc>
readInput ()
{
  std::vector<Disc> discs;
  int number, numPositions, initial;
  while (scanf ("Disc #%d has %d positions; at time=0, it is at position %d.\n", &number, &numPositions, &initial) == 3) {
    assert ((std::size_t)number == discs.size () + 1);
    assert (numPositions >= 2);
    assert (initial >= 0 && initial < numPositions);
    discs.push_back ({number, numPositions, initial});
  }
  return discs;
}

bool
willFallThrough (const std::vector<Disc>& discs, int time)
{
  for (const Disc& disc : discs) {
    if ((time + disc.number + disc.initial) % disc.numPositions != 0) {
      return false;
    }
  }
  return true;
}

int
firstCapsule (const std::vector<Disc>& discs)
{
  for (int i = 0; true; ++i) {
    if (willFallThrough (discs, i)) { return i; }
  }
  return -1;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Disc> discs = readInput ();
  std::cout << firstCapsule (discs) << "\n";
  discs.push_back ({(int)discs.size () + 1, 11, 0});
  std::cout << firstCapsule (discs) << "\n";
  return 0;
}
