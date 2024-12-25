/// \file 2024Day25.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-25.


#include <iostream>
#include <vector>
#include <string>
#include <cassert>

const int HEIGHT = 7;
const int WIDTH = 5;

using Tumbler = std::vector<int>;

struct Problem
{
  std::vector<Tumbler> locks;
  std::vector<Tumbler> keys;
};

Problem
readInput ()
{
  Problem prob;
  std::string lines[HEIGHT];
  while (std::getline (std::cin, lines[0]) && std::getline (std::cin, lines[1])
  && std::getline (std::cin, lines[2]) && std::getline (std::cin, lines[3])
  && std::getline (std::cin, lines[4]) && std::getline (std::cin, lines[5])
  && std::getline (std::cin, lines[6])) {
    assert ((lines[0] == ".....") != (lines[HEIGHT - 1] == "....."));
    if (lines[0] == ".....") {
      std::vector<int> key ({0, 0, 0, 0, 0});
      for (int column = 0; column < WIDTH; ++column) {
        for (int row = 0; row < HEIGHT - 1; ++row) {
          if (lines[row][column] == '#') { ++key[column]; }
        }
      }
      prob.keys.push_back (key);
    }
    else {
      std::vector<int> lock ({0, 0, 0, 0, 0});
      for (int column = 0; column < WIDTH; ++column) {
        for (int row = 1; row < HEIGHT; ++row) {
          if (lines[row][column] == '#') { ++lock[column]; }
        }
      }
      prob.locks.push_back (lock);
    }
    std::getline (std::cin, lines[0]);
  }
  return prob;
}

bool
overlaps (const Tumbler& a, const Tumbler& b)
{
  for (int index = 0; index < WIDTH; ++index) {
    if (a[index] + b[index] > HEIGHT - 2) { return true; }
  }
  return false;
}

int
countCompatible (const Problem& prob)
{
  int count = 0;
  for (const Tumbler& key : prob.keys) {
    for (const Tumbler& lock : prob.locks) {
      if (!overlaps (key, lock)) { ++count; }
    }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  std::cout << countCompatible (prob) << "\n";
  return 0;
}

