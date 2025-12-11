/// \file 2025Day11.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-11.

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <map>

using Device = std::string;

using Problem = std::map<Device, std::set<Device>>;

Problem
getInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    assert (line.length () >= 8);
    assert (line.length () % 4 == 0);
    std::string header = line.substr (0, 3);
    assert (isalpha (header[0]) && isalpha (header[1]) && isalpha (header[2]));
    assert (prob.count (header) == 0);
    prob[header] = {};
    assert (line[3] == ':');
    std::size_t index = 5;
    while (index < line.size ()) {
      std::string output = line.substr (index, 3);
      assert (isalpha (output[0]) && isalpha (output[1]) && isalpha (output[2]));
      assert (prob[header].count (output) == 0);
      assert (header != output);
      prob[header].insert (output);
      index += 4;
    }
  }
  // Not in second sample.
  //assert (prob.count ("you") == 1);
  std::size_t outCount = 0;
  for (auto it = prob.begin (); it != prob.end (); ++it) {
    for (auto it2 = it->second.begin (); it2 != it->second.end (); ++it2) {
      assert (*it2 == "out" || prob.count (*it2) == 1);
      if (*it2 == "out") { ++outCount; }
    }
  }
  assert (outCount > 0);
  return prob;
}

using Path = std::vector<Device>;

std::size_t
countPaths (const Problem& prob, const Device& start = "you", const Device& end = "out", const std::set<Device>& required = {})
{
  std::set<Path> finished;
  std::set<Path> inProgress;
  inProgress.insert ({1, start});
  while (!inProgress.empty ()) {
    Path current = *inProgress.begin ();
    inProgress.erase (current);
    if (current.back () == end) {
      finished.insert (current);
    }
    else {
      for (const Device& output : prob.at (current.back ())) {
        Path next = current;
        for (const Device& existing : next) {
          assert (existing != output);
        }
        next.push_back (output);
        inProgress.insert (next);
      }
    }
  }
  for (auto it = finished.begin (); it != finished.end (); ) {
    bool missingOne = false;
    for (const Device& needed : required) {
      bool found = false;
      for (const Device& has : *it) {
        if (has == needed) { found = true; }
      }
      if (!found) { missingOne = true; }
    }
    if (missingOne) { it = finished.erase (it); }
    else { ++it; }
  }
  return finished.size ();
}

std::size_t
part2 (const Problem& prob, const Device& start, const Device& end, std::set<Device> required)
{
  std::size_t result = 0;
  for (const Device& newStart : prob.at (start)) {
    result += countPaths (prob, newStart, end, required);
    std::cout << "Finished with " << newStart << "\n";
  }
  return result;
}

using Solveable = std::pair<std::pair<Device, Device>, std::set<Device>>;
unsigned long long
pureRecursive (const Problem& prob, const Device& start, const Device& end, std::set<Device> required)
{
  static std::map<Solveable, unsigned long long> cache;
  unsigned long long result = 0;
  if (start == end) {
    if (required.empty ()) { return 1; }
    else { return 0; }
  }
  Solveable solve = {{start, end}, required};
  if (cache.count (solve) != 0) { return cache[solve]; }
  for (const Device& newStart : prob.at (start)) {
    if (required.count (newStart) == 0) {
      result += pureRecursive (prob, newStart, end, required);
    }
    else {
      std::set<Device> fewer = required;
      fewer.erase (newStart);
      result += pureRecursive (prob, newStart, end, fewer);
    }
  }
  cache[solve] = result;
  return result;
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  //std::cout << countPaths (prob) << "\n";
  //std::cout << countPaths (prob, "svr", "out", {"dac", "fft"}) << "\n";
  std::cout << pureRecursive (prob, "svr", "out", {"dac", "fft"}) << "\n";
  return EXIT_SUCCESS;
}

