/// \file 2025Day07.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-07.

#include <cstddef>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>

const char BLANK = '.';
const char SPLITTER = '^';
const char START = 'S';
const char BEAM = '|';

using Problem = std::vector<std::string>;

Problem
getInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    prob.push_back (line);
  }
  int startCount = 0;
  assert (prob.size () > 1);
  for (const std::string& str : prob) {
    assert (str.length () == prob[0].length ());
    for (char c : str) {
      assert (c == BLANK || c == SPLITTER || c == START);
      if (c == START) { ++startCount; }
    }
  }
  assert (startCount == 1);
  assert (prob[0].find (START) != std::string::npos);
  return prob;
}

Problem
propagate (const Problem& prob)
{
  Problem result;
  for (std::size_t lineNo = 0; lineNo != prob.size (); ++lineNo) {
    result.push_back ("");
    for (std::size_t charNo = 0; charNo != prob.at(lineNo).length (); ++charNo) {
      if (prob.at (lineNo).at (charNo) == BLANK) {
        // If beam or start above us, it continues into this spot
        if (lineNo > 0 && (result.at (lineNo - 1).at (charNo) == BEAM || result.at (lineNo - 1).at (charNo) == START)) {
          result.back ().push_back (BEAM);
        }
        // If splitter left of us receiving a beam, split into this spot.
        else if (charNo > 0 && lineNo > 0 && result.at (lineNo).at (charNo - 1) == SPLITTER && result.at (lineNo - 1).at (charNo - 1) == BEAM) {
          result.back ().push_back (BEAM);
        }
        // If splitter right of us receiving a beam, split into this spot.
        else if (charNo < prob.at (lineNo).length () - 1 && lineNo > 0 && prob.at (lineNo).at (charNo + 1) == SPLITTER && result.at (lineNo - 1).at (charNo + 1) == BEAM) {
          result.back ().push_back (BEAM);
        }
        // Otherwise, stays blank.
        else {
          result.back ().push_back (BLANK);
        }
      }
      else {
        result.back ().push_back (prob.at (lineNo).at (charNo));
      }
    }
  }
  return result;
}

std::size_t
countSplits (const Problem& prob)
{
  std::size_t count = 0;
  for (std::size_t lineNo = 1; lineNo < prob.size (); ++lineNo) {
    for (std::size_t charNo = 0; charNo < prob.at (lineNo).size (); ++charNo) {
      if (prob.at (lineNo).at (charNo) == SPLITTER && prob.at (lineNo - 1).at (charNo) == BEAM) {
        ++count;
      }
    }
  }
  return count;
}

std::size_t
countTimelines (const Problem& prob, std::pair<std::size_t, std::size_t> start)
{
  assert (prob.at (start.first).at (start.second) == BEAM);
  static std::map<std::pair<std::size_t, std::size_t>, std::size_t> cache;
  if (cache.count (start) != 0) { return cache.at (start); }
  std::size_t lineNo = start.first;
  while (lineNo < prob.size () - 1 && prob.at (lineNo).at (start.second) == BEAM) {
    ++lineNo;
  }
  if (prob.at (lineNo).at (start.second) == SPLITTER) {
    std::size_t total = 0;
    if (start.second > 0) {
      total += countTimelines (prob, {lineNo, start.second - 1});
    }
    if (start.second < prob.at (0).size () - 1) {
      total += countTimelines (prob, {lineNo, start.second + 1});
    }
    cache[start] = total;
  }
  else {
    assert (lineNo == prob.size () - 1 && prob.at (lineNo).at (start.second) == BEAM);
    cache[start] = 1;
  }
  return cache.at (start);
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  Problem finished = propagate (prob);
  std::cout << countSplits (finished) << "\n";
  std::cout << countTimelines (finished, {1, finished.at (1).find (BEAM)}) << "\n";
  return EXIT_SUCCESS;
}
