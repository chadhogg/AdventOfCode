/// \file 2015Day20.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-20.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <map>
#include <regex>
#include <list>

long long
getPresentsNumber (long long houseNo)
{
  long long total = 0;
  for (long long elfNo = 1; elfNo <= houseNo; ++elfNo) {
    if (houseNo % elfNo == 0) {
      total += 10 * elfNo;
    }
  }
  return total;
}

// It takes a little time, but brute force is good enough.
// Otherwise, we have to think about how many unique factors each number has or something.
long long
firstHouseNumber (long long minPresents)
{
  for (long long houseNo = 1; ; ++houseNo) {
    if (getPresentsNumber (houseNo) >= minPresents) {
      return houseNo;
    }
  }
}

long long
getPresentsNumberModified (long long houseNo)
{
  long long total = 0;
  for (long long elfNo = std::max (1LL, houseNo / 50); elfNo <= houseNo; ++elfNo) {
    if (houseNo % elfNo == 0 && houseNo / elfNo < 50) {
      total += 11 * elfNo;
    }
  }
  return total;
}

long long
firstHouseNumberModified (long long minPresents)
{
  for (long long houseNo = 1; ; ++houseNo) {
    if (getPresentsNumberModified (houseNo) >= minPresents) {
      return houseNo;
    }
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  assert (130 == getPresentsNumber (9));
  assert (8 == firstHouseNumber (140));
  // Let's not wait for this again.
  //std::cout << firstHouseNumber (36000000) << "\n";
  std::cout << firstHouseNumberModified (36000000) << "\n";
  return 0;
}

