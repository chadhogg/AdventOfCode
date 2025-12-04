/// \file 2025Day04.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-04.

#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

const char PAPER = '@';
const char BLANK = '.';

using Problem = std::vector<std::string>;

Problem
getInput ()
{
  Problem prob;
  std::string line;
  while (std::cin >> line) {
    for (char c : line) {
      assert (c == PAPER || c == BLANK);
    }
    if (!prob.empty ()) { assert (line.length () == prob[0].length ()); }
    prob.push_back (line);
  }
  return prob;
}

bool
hasFewerThanXNeighbors (const Problem& prob, std::size_t row, std::size_t col, std::size_t x)
{
  std::vector<std::pair<std::size_t, std::size_t>> toCheck;
  if (row > 0) {
    if (col > 0) { toCheck.push_back ({row - 1, col - 1}); }
    toCheck.push_back ({row - 1, col});
    if (col < prob.at (0).length () - 1) { toCheck.push_back ({row - 1, col + 1}); }
  }
  if (col > 0) { toCheck.push_back ({row, col - 1}); }
  if (col < prob.at (0).length () - 1) { toCheck.push_back ({row, col + 1}); }
  if (row < prob.size () - 1) {
    if (col > 0) { toCheck.push_back ({row + 1, col - 1}); }
    toCheck.push_back ({row + 1, col});
    if (col < prob.at (0).length () - 1) { toCheck.push_back ({row + 1, col + 1}); }
  }
  std::size_t count = 0;
  for (const std::pair<std::size_t, std::size_t>& loc : toCheck) {
    if (prob.at (loc.first).at (loc.second) == PAPER) { ++count; }
  }
  return count < x;
}

std::size_t
countAccessiblePaper (const Problem& prob) {
  std::size_t count = 0;
  for (std::size_t row = 0; row < prob.size (); ++row) {
    for (std::size_t col = 0; col < prob.at (row).size (); ++col) {
      if (prob.at (row).at (col) == PAPER) {
        if (hasFewerThanXNeighbors (prob, row, col, 4)) {
          ++count;
        }
      }
    }
  }
  return count;
}

std::size_t
removePaper (Problem & prob) {
  std::size_t count = 0;
  bool removedSomething = true;
  while (removedSomething) {
    removedSomething = false;
    for (std::size_t row = 0; row < prob.size (); ++row) {
      for (std::size_t col = 0; col < prob.at (row).size (); ++col) {
        if (prob.at (row).at (col) == PAPER) {
          if (hasFewerThanXNeighbors(prob, row, col, 4)) {
            ++count;
            prob[row][col] = BLANK;
            removedSomething = true;
          }
        }
      }
    }
  }
  return count;
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::cout << countAccessiblePaper (prob) << "\n";
  std::cout << removePaper (prob) << "\n";
  return EXIT_SUCCESS;
}

