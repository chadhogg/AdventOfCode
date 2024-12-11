/// \file 2024Day11.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-11.

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

using Stone = long;
using Line = std::vector<Stone>;

Line
readInput ()
{
  Line input;
  int x;
  std::cin >> x;
  while (std::cin) {
    input.push_back (x);
    std::cin >> x;
  }
  return input;
}

Line
blink (const Line& original)
{
  Line revised;
  for (Stone stone: original) {
    std::string word = std::to_string (stone);
    if (stone == 0L) {
      revised.push_back (1L);
    }
    else if (word.size () % 2 == 0) {
      revised.push_back (atol (word.substr (0, word.length () / 2).c_str ()));
      revised.push_back (atol (word.substr (word.length () / 2).c_str ()));
    }
    else {
      revised.push_back (stone * 2024L);
    }
  }
  return revised;
}

Line
multiBlink (const Line& original, int count) {
  Line current = original;
  for (int i = 0; i < count; ++i) {
    current = blink (current);
  }
  return current;
}

using Work = std::pair<Stone, int>;
long
countStones (Work toDo)
{
  static std::map<Work, long> cache;
  if (cache.count (toDo) > 0) {
    return cache.at (toDo);
  }
  else if (toDo.second == 0) { return 1L; }
  else {
    long result;
    std::string word = std::to_string (toDo.first);
    if (toDo.first == 0L) {
      result = countStones ({1L, toDo.second - 1});
    }
    else if (word.size () % 2 == 0) {
      result = countStones ({atol (word.substr (0, word.length () / 2).c_str ()), toDo.second - 1})
      + countStones ({atol (word.substr (word.length () / 2).c_str ()), toDo.second - 1});
    }
    else {
      result = countStones ({toDo.first * 2024L, toDo.second - 1});
    }
    cache[toDo] = result;
    return result;
  }
}

long
countAllStones (const Line& original, int blinks) {
  long total = 0;
  for (const Stone& stone : original) {
    total += countStones ({stone, blinks});
  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Line start = readInput ();
  std::cout << multiBlink (start, 25).size () << "\n";
  std::cout << countAllStones (start, 25) << "\n";
  //std::cout << multiBlink (start, 75).size () << "\n";
  std::cout << countAllStones (start, 75) << "\n";
  return 0;
}
