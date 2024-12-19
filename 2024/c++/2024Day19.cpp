/// \file 2024Day19.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-19.


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
#include <map>
#include <optional>
#include <chrono>
#include <thread>
#include <queue>

struct Problem
{
  std::vector<std::string> types;
  std::vector<std::string> patterns;

};

std::ostream&
operator<< (std::ostream& out, const Problem& prob)
{
  out << prob.types.front ();
  for (unsigned int i = 0; i < prob.types.size (); ++i) {
    if (i != 0) { out << ", "; }
    out << prob.types.at (i);
  }
  out << "\n\n";
  for (unsigned int i = 0; i < prob.patterns.size (); ++i) {
    out << prob.patterns.at (i);
    if (i != prob.patterns.size () - 1) { out << "\n"; }
  }
  return out;
}

Problem
readInput ()
{
  Problem prob;
  std::string word;
  bool finishedTypes = false;
  while (std::cin >> word) {
    if (word[word.size () - 1] == ',') {
      prob.types.push_back (word.substr (0, word.size () - 1));
    }
    else if (!finishedTypes) {
      prob.types.push_back (word);
      finishedTypes = true;
    }
    else {
      prob.patterns.push_back (word);
    }
  }
  return prob;
}

bool
canFinish (const std::string& pattern, const std::vector<std::string>& types, unsigned int charsDone)
{
  static std::map<std::pair<std::string, unsigned int>, bool> cache;
  if (charsDone == pattern.size ()) { return true; }
  if (cache.count ({pattern, charsDone}) == 1) { return cache[{pattern, charsDone}]; }
  for (const std::string& type: types) {
    if (type.size () <= pattern.size () - charsDone) {
      if (pattern.substr (charsDone, type.size ()) == type) {
        if (canFinish (pattern, types, charsDone + type.size ())) { return true; }
      }
    }
  }
  cache[{pattern, charsDone}] = false;
  return false;
}

int
part1 (const Problem& prob)
{
  int count = 0;
  for (const std::string& pattern: prob.patterns) {
    if (canFinish (pattern, prob.types, 0)) { ++count; }
  }
  return count;
}

long
countWaysToFinish (const std::string& pattern, const std::vector<std::string>& types, unsigned int charsDone)
{
  static std::map<std::pair<std::string, unsigned int>, long> cache;
  if (charsDone == pattern.size ()) { return 1; }
  if (cache.count ({pattern, charsDone}) == 1) { return cache[{pattern, charsDone}]; }
  long ways = 0;
  for (const std::string& type: types) {
    if (type.size () <= pattern.size () - charsDone) {
      if (pattern.substr (charsDone, type.size ()) == type) {
        ways += countWaysToFinish (pattern, types, charsDone + type.size ());
      }
    }
  }
  cache[{pattern, charsDone}] = ways;
  return ways;
}

long
part2 (const Problem& prob)
{
  long count = 0;
  for (const std::string& pattern: prob.patterns) {
    count += countWaysToFinish (pattern, prob.types, 0);
  }
  return count;
}
/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  //std::cout << prob << "\n";
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";
  return 0;
}
