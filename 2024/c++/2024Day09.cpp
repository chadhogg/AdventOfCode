/// \file 2024Day09.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-09.
/// Thanks to William Killian for helping me debug part 2 (was double-decremeting my iterator).

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


using RegionLen = int;
using RegionType = int;
using Region = std::pair<RegionType, RegionLen>;
using DiskMap = std::vector<RegionLen>;
using DiskMap2 = std::list<Region>;

const RegionType FREE = -1;

std::ostream&
operator<< (std::ostream& out, const Region& region)
{
  if (region.first == FREE) {
    out << "[free: " << region.second << "]";
  }
  else {
    out << "[" << region.first << ": " << region.second << "]";
  }
  return out;
}

DiskMap
readInput ()
{
  DiskMap dm;
  std::string line;
  std::getline (std::cin, line);
  for (char c : line) {
    dm.push_back (c - '0');
  }
  return dm;
}

DiskMap2
convertInput (const DiskMap& dm) {
  DiskMap2 dm2;
  RegionType nextFileNum = 0;
  for (unsigned int index = 0; index < dm.size (); ++index) {
    if (index % 2 == 0) {
      dm2.push_back ({nextFileNum, dm.at (index)});
      ++nextFileNum;
    }
    else {
      dm2.push_back ({FREE, dm.at (index)});
    }
  }
  return dm2;
}

DiskMap2
compact (const DiskMap2& original)
{
  DiskMap2 copy (original);
  // For simplicity, just remember length of free region at end; don't store it.
  int numFreeAtEnd = 0;
  if (copy.back ().first == FREE) {
    numFreeAtEnd += copy.back ().second;
    copy.pop_back ();
  }
  auto it = copy.begin ();
  assert (it->first != FREE);
  // Move from first file region to first free region
  ++it;
  assert (it->first == FREE);
  while (it != copy.end ()) {
    assert (copy.back ().first != FREE);
    if (copy.back ().second == 0) {
      // There's nothing left of what had been the last file, so move to the next-to-last file.
      copy.pop_back ();
      assert (copy.back ().first == FREE);
      numFreeAtEnd += copy.back ().second;
      if (std::next (it) == copy.end ()) {
        copy.pop_back ();
        break;
      }
      else {
        copy.pop_back ();
      }
    }
    else if (it->second == 0) {
      // No more space in this free region, so time to move on.
      ++it;
      assert (it->first != FREE);
      ++it;
    }
    else {
      // Time to move one block from end to here.
      auto prev = it;
      --prev;
      assert (prev->first != FREE);
      if (prev->first == copy.back ().first) {
        // Can just extend region behind me.
        ++(prev->second);
        --(copy.back ().second);
        --(it->second);
        ++numFreeAtEnd;
      }
      else {
        // Need to split this region.
        copy.insert (it, {FREE, 0});
        copy.insert (it, {copy.back ().first, 0});
      }
    }
  }
  assert (copy.back ().first != FREE);
  if (copy.size () >= 3) {
    auto last = copy.end ();
    --last;
    auto prev = last;
    --prev;
    --prev;
    if (prev->first == last->first) {
      prev->second += last->second;
      ++prev;
      numFreeAtEnd += prev->second;
      prev = copy.erase (prev);
      copy.erase (last);
    }
  }
  copy.push_back ({FREE, numFreeAtEnd});
  return copy;
}

DiskMap2
compact2 (const DiskMap2& original)
{
  DiskMap2 copy (original);
  if (copy.back ().first != FREE) { copy.push_back ({FREE, 0}); }
  // Starts at last file region, moves forward to first file region.
  
  for (auto toMove = std::prev (copy.end (), 2); toMove != copy.begin (); toMove = std::prev (toMove, 2)) {
    assert (toMove->first != FREE);
    // Search for the earliest preceeding free region we could move it into.
    for (auto toFill = std::next (copy.begin ()); std::prev (toFill) != toMove; toFill = std::next(toFill, 2)) {
      assert (toFill->first == FREE);
      if (toMove->second <= toFill->second) {
        // Insert an empty free region to separate the previous file from the one being moved.
        copy.insert (toFill, {FREE, 0});
        // Insert the file.
        copy.insert (toFill, {toMove->first, toMove->second});
        // Decrease my size by the file we just moved here.
        toFill->second -= toMove->second;
        // Coalesce the file we just moved into free region on either side.
        std::prev (toMove)->second += toMove->second + std::next (toMove)->second;
        toMove = copy.erase (toMove);
        toMove = copy.erase (toMove);
        // Already moved this one, so stop trying to find a place for it.
        break;
      }
    }
  }
  return copy;
}

long
checksum (const DiskMap2& dm)
{
  long sum = 0;
  long pos = 0;
  for (const Region& reg : dm) {
    if (reg.first != FREE) {
      for (int x = 0; x < reg.second; ++x) {
        sum += reg.first * (long)(pos + x);
      }
    }
    pos += reg.second;
  }
  return sum;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  DiskMap dm = readInput ();
  DiskMap2 dm2 = convertInput (dm);
  //for (const Region& r : dm2) { std::cout << r << " "; } std::cout << "\n";
  DiskMap2 cm = compact (dm2);
  //for (const Region& r : cm) { std::cout << r << " "; } std::cout << "\n";
  std::cout << checksum (cm) << "\n";
  DiskMap2 cm2 = compact2 (dm2);
  //for (const Region& r : cm) { std::cout << r << "\n"; } std::cout << "\n";
  std::cout << checksum (cm2) << "\n";
  return 0;
}
