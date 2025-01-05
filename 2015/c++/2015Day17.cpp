/// \file 2015Day17.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-17.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <regex>
#include <list>
#include <set>
#include <algorithm>
#include <ranges>

using Container = int;

const Container TOTAL = 150;

std::vector<Container> 
readInput () {
  std::vector<Container> containers;
  Container c;
  while (std::cin >> c) {
    containers.push_back (c);
  }
  return containers;
}

struct PartialSelection
{
  std::vector<bool> choices;
  Container totalOfChosen;
  Container notYetDecided;

  auto operator<=>(const PartialSelection& other) const = default;
};

std::set<PartialSelection>
findExactMatches (const std::vector<Container>& containers, Container total, Container sumOfAll, const PartialSelection& current)
{
  if (current.totalOfChosen == total) {
    return {current};
  }
  else if (current.totalOfChosen > total || containers.size () == current.choices.size ()) {
    return {};
  }
  else if (current.totalOfChosen + current.notYetDecided < total) {
    return {};
  }
  else {
    PartialSelection with = current;
    PartialSelection without = current;
    with.choices.push_back (true);
    without.choices.push_back (false);
    with.totalOfChosen += containers.at (with.choices.size () - 1);
    without.notYetDecided -= containers.at (with.choices.size () - 1);
    std::set<PartialSelection> withAnswers = findExactMatches (containers, total, sumOfAll, with);
    std::set<PartialSelection> withoutAnswers = findExactMatches (containers, total, sumOfAll, without);
    std::set<PartialSelection> allAnswers;
    std::move (withAnswers.begin (), withAnswers.end (), std::inserter (allAnswers, allAnswers.end ()));
    std::move (withoutAnswers.begin (), withoutAnswers.end (), std::inserter (allAnswers, allAnswers.end ()));
    return allAnswers;
  }
}

std::set<PartialSelection>
minimalOnes (const std::set<PartialSelection>& all) {
  int smallestSize = INT32_MAX;
  std::set<PartialSelection> minimal;
  for (const PartialSelection& one : all) {
    int numTrues = 0;
    for (bool b : one.choices) { 
      if (b) { ++numTrues; }
    }
    if (numTrues < smallestSize) {
      minimal.clear ();
      minimal.insert (one);
      smallestSize = numTrues;
    }
    else if (numTrues == smallestSize) {
      minimal.insert (one);
    }
  }
  return minimal;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Container> containers = readInput ();
  Container sumOfAll = std::ranges::fold_left (containers.cbegin (), containers.cend (), 0, std::plus {});
  std::set<PartialSelection> matches = findExactMatches (containers, TOTAL, sumOfAll, {{}, 0, sumOfAll});
  std::cout << matches.size () << "\n";
  std::cout << minimalOnes (matches).size () << "\n";
  return 0;
}
