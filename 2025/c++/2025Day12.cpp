/// \file 2025Day12.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-12.

#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <regex>

const char FILLED = '#';
const char EMPTY = '.';
using PresentShape = std::array<std::array<char, 3>, 3>;

struct Region
{
  std::size_t width;
  std::size_t length;
  std::vector<std::size_t> counts;
};

struct Problem
{
  std::vector<PresentShape> shapes;
  std::vector<Region> regions;
};

Problem
getInput ()
{
  Problem prob;
  std::string line0;
  while (std::getline (std::cin, line0) && line0.length () == 2) {
    assert (std::isdigit (line0[0]));
    assert (prob.shapes.size () == (std::size_t)atoi (line0.substr (0, 1).c_str ()));
    assert (line0[1] == ':');
    PresentShape shape;
    for (std::size_t lineNo = 0; lineNo < 3; ++lineNo) {
      std::getline (std::cin, line0);
      assert (line0.size () == 3);
      for (char c : line0) { assert (c == FILLED || c == EMPTY); }
      for (std::size_t charNo = 0; charNo < 3; ++charNo) {
        shape[lineNo][charNo] = line0[charNo];
      }
    }
    std::getline (std::cin, line0);
    assert (line0.size () == 0);
    prob.shapes.push_back (shape);
  }
  assert (prob.shapes.size () < 10);

  std::regex regionExp ("^(\\d+)x(\\d+):(.*)$");
  std::regex intExp ("(\\d+)");
  std::smatch match, match2;
  while (std::cin) {
    if (std::regex_match (line0, match, regionExp)) {
      Region region;
      region.width = atoi (match[1].str ().c_str ());
      region.length = atoi (match[2].str ().c_str ());
      std::string rest = match[3].str ();
      for (std::sregex_iterator it (rest.begin (), rest.end (), intExp); it != std::sregex_iterator (); ++it) {
        region.counts.push_back (atoi (it->str ().c_str ()));
      }
      assert (region.counts.size () == prob.shapes.size ());
      prob.regions.push_back (region);
    }
    else {
      assert (false);
    }
    std::getline (std::cin, line0);
  }
  return prob;
}

bool
isTriviallyBigEnough (const Problem& prob, const Region& region)
{
  std::size_t totalPresents = 0;
  for (std::size_t number : region.counts) { totalPresents += number; }
  std::size_t across = region.width / 3;
  return (totalPresents <= across * (region.length / 3));
}

bool
isTriviallyTooSmall (const Problem& prob, const Region& region)
{
  std::size_t totalSquares = 0;
  for (std::size_t shapeIndex = 0; shapeIndex < prob.shapes.size (); ++shapeIndex) {
    std::size_t shapeSquares = 0;
    for (std::size_t x = 0; x < 3; ++x) {
      for (std::size_t y = 0; y < 3; ++y) {
        if (prob.shapes.at (shapeIndex).at (x).at (y) == FILLED) { ++shapeSquares; }
      }
    }
    totalSquares += shapeSquares * region.counts.at (shapeIndex);
  }
  return (region.width * region.length < totalSquares);
}

void
explorePart1 (const Problem& prob)
{
  std::size_t easyYes = 0, easyNo = 0, hard = 0;
  for (const Region& region : prob.regions) {
    if (isTriviallyBigEnough (prob, region)) {
      ++easyYes;
    }
    else if (isTriviallyTooSmall (prob, region)) {
      ++easyNo;
    }
    else {
      ++hard;
    }
  } 
  std::cout << easyYes << " definitely yes, " << easyNo << " definitely no, " << hard << " maybe\n";
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  explorePart1 (prob);
  // Maybe we'll write an algorithm for solving the "maybe" cases later ...
  return EXIT_SUCCESS;
}

