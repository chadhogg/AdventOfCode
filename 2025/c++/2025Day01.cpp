/// \file 2025Day01.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-01.
/// I should have just written countInstancesTedious as soon as I got to part 2.
/// Instead, wasted a bunch of time trying to finesse countInstances in a way that
///   would have worked if the start were 0.
/// Will come back and figure out how to make that adjustment later.

#include <iostream>
#include <vector>
#include <string>

struct Rotation {
  char dir;
  int count;
};

using Problem = std::vector<Rotation>;

Problem
getInput () {
  std::vector<Rotation> problem;
  std::string token;
  while (std::cin >> token) {
    problem.push_back ({token[0], atoi (token.substr (1).c_str ())});
  }
  return problem;
}

int
countInstances (const Problem& problem, int start, int target) {
  int count = 0;
  int value = start;
  for (const Rotation& rotation : problem) {
    if (rotation.dir == 'L') {
      value -= rotation.count;
      while (value < 0) {
        value += 100;
      }
    }
    else {
      value += rotation.count;
      value = value % 100;
    }
    if (value == target) {
      ++count;
    }
  }
  return count;
}

int
countInstancesTedious (const Problem& problem, int start, int target) {
  int count = 0;
  int value = start;
  for (const Rotation& rotation : problem) {
    if (rotation.dir == 'L') {
      for (int i = 0; i < rotation.count; ++i) {
        --value;
        if (value < 0) {
          value += 100;
        }
        if (value == target) { ++count; }
      }
    }
    else {
      for (int i = 0; i < rotation.count; ++i) {
        ++value;
        if (value > 99 ) { 
          value -= 100;
        }
        if (value == target) { ++count; }
      }
    }
  }
  return count;
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::cout << countInstances(prob, 50, 0) << "\n";
  std::cout << countInstancesTedious(prob, 50, 0) << "\n";
  return EXIT_SUCCESS;
}

