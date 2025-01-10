/// \file 2015Day24.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-24.

// This sure feels knapsack-y and subset-sum-y and thus NP-complete.
// But hopefully small enough to solve anyway?

#include <iostream>
#include <string>
#include <cassert>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>
#include <array>
#include <list>

using Container = std::set<int>;
using Sleigh = std::array<Container, 3>;

std::ostream&
operator<< (std::ostream& out, const Container& c)
{
  for (int i : c) {
    out << i << " ";
  }
  return out;
}

std::ostream&
operator<< (std::ostream& out, const Sleigh& s)
{
  out << s[0] << "\n" << s[1] << "\n" << s[2] << "\n";
  return out;
}

std::set<int>
readInput ()
{
  std::set<int> packages;
  int current;
  while (std::cin >> current) {
    assert (!packages.contains (current));
    packages.insert (current);
  }
  return packages;
}

int
totalWeight (const Container& container)
{
  return std::ranges::fold_left (container.cbegin (), container.cend (), 0, std::plus{});
}

// Uh, sometimes the answer to this doesn't fit ...
unsigned long long
quantumEntanglement (const Container& container)
{
  //return std::ranges::fold_left (container.cbegin (), container.cend (), 1LL, std::multiplies{});
  unsigned long long result = 1ULL;
  for (int num : container) {
    result *= num;
  }
  return result;
}

Sleigh
bruteForceOptimize (const std::set<int>& packages)
{
  std::vector<int> orderedPackages (packages.cbegin (), packages.cend ());
  // Let's deal with the heavy packages first, because we will
  //   be able to detect dead-ends more quickly.
  std::reverse (orderedPackages.begin (), orderedPackages.end ());
  Sleigh bestSoFar;
  std::list<Sleigh> frontier;
  frontier.push_back ({});
  while (!frontier.empty ()) {
    Sleigh current = frontier.front ();
    //std::cout << "Considering\n" << current << "\n";
    frontier.pop_front ();
    std::size_t numDone = current[0].size () + current[1].size () + current[2].size ();
    if (numDone == orderedPackages.size ()) {
      if (totalWeight (current[0]) == totalWeight (current[1]) && totalWeight (current[1]) == totalWeight (current[2])) {
        // This partition is valid.
        if (bestSoFar[0].size () == 0) {
          bestSoFar = current;
        }
        else if (current[0].size () < bestSoFar[0].size ()) {
          bestSoFar = current;
        }
        else if (current[0].size () == bestSoFar[0].size () && quantumEntanglement (current[0]) < quantumEntanglement (bestSoFar[0])) {
          bestSoFar = current;
        }
      }
    }
    else {
      int totalRemaining = std::ranges::fold_left (orderedPackages.begin () + numDone, orderedPackages.end (), 0, std::plus{});
      int weight0 = totalWeight (current[0]);
      int weight1 = totalWeight (current[1]);
      int weight2 = totalWeight (current[2]);
      int biggestDiff = std::max ({
        std::abs (weight0 - weight1),
        std::abs (weight0 - weight2),
        std::abs (weight1 - weight2)
      });
      if (totalRemaining >= biggestDiff) {
        // This partial partition can conceivably be made valid.
        // Probably an even tighter bound can be checked here, but I am not sure what.
        Sleigh inA = current, inB = current, inC = current;
        inA[0].insert (orderedPackages[numDone]);
        inB[1].insert (orderedPackages[numDone]);
        inC[2].insert (orderedPackages[numDone]);
        frontier.push_front (inC);
        frontier.push_front (inB);
        frontier.push_front (inA);
      }
    }
  }
  return bestSoFar;
}

Container
fasterIdea (const std::set<int>& packages)
{
  int allWeight = totalWeight (packages);
  assert (allWeight % 3 == 0);
  int targetWeight = allWeight / 3;
  Container bestSoFar;
  std::vector<int> orderedPackages (packages.cbegin (), packages.cend ());
  std::reverse (orderedPackages.begin (), orderedPackages.end ());
  struct Choices
  {
    Container chosen;
    Container notChosen;
  };
  std::list<Choices> frontier;
  frontier.push_front ({{}, {}});
  while (!frontier.empty ()) {
    Choices current = frontier.front ();
    frontier.pop_front ();
    std::size_t numConsidered = current.chosen.size () + current.notChosen.size ();
    if (numConsidered == packages.size ()) {
      if (totalWeight (current.chosen) == targetWeight) {
        // TODO: check that unchosen can be partitioned
        if (bestSoFar.empty () || current.chosen.size () < bestSoFar.size () || (current.chosen.size () == bestSoFar.size () && quantumEntanglement (current.chosen) < quantumEntanglement (bestSoFar))) {
          bestSoFar = current.chosen;
        }
      }
    }
    else {
      if (bestSoFar.size () == 0 || current.chosen.size () <= bestSoFar.size ()) {
        Choices with = current, without = current;
        with.chosen.insert (orderedPackages[numConsidered]);
        without.notChosen.insert (orderedPackages[numConsidered]);
        if (totalWeight (with.chosen) <= targetWeight) {
          frontier.push_front (with);
        }
        if (totalWeight (without.chosen) <= allWeight - targetWeight) {
          frontier.push_front (without);
        }
      }
    }
  }
  return bestSoFar;
}

std::set<Container>
partOfPowerSet (const std::set<int>& things, std::size_t howMany, int desiredWeight)
{
  std::vector<int> orderedThings (things.cbegin (), things.cend ());
  reverse (orderedThings.begin (), orderedThings.end ());
  std::set<Container> results;
  struct Choices
  {
    Container chosen;
    Container notChosen;
  };
  std::list<Choices> frontier;
  frontier.push_back ({{}, {}});
  while (!frontier.empty ()) {
    Choices current = frontier.front ();
    frontier.pop_front ();
    assert (current.chosen.size () <= howMany);
    assert (totalWeight (current.chosen) <= desiredWeight);
    assert (current.chosen.size () + current.notChosen.size () <= things.size ());
    if (current.chosen.size () == howMany) {
      if (totalWeight (current.chosen) == desiredWeight) { 
        results.insert (current.chosen);
      }
    }
    else {
      Choices with = current, without = current;
      std::size_t amountDecided = current.chosen.size () + current.notChosen.size ();
      if (amountDecided < things.size ()) {
        with.chosen.insert (orderedThings[amountDecided]);
        without.notChosen.insert (orderedThings[amountDecided]);
        if (with.chosen.size () <= howMany && totalWeight (with.chosen) <= desiredWeight) {
          frontier.push_front (with);
        }
        std::size_t stillAvailable = things.size () - without.notChosen.size ();
        if (stillAvailable + without.chosen.size () >= howMany) {
          frontier.push_back (without);
        }
      }
    }
  }
  return results;
}

Container
hopefullyEvenFasterIdea (const std::set<int>& packages, int numContainers)
{
  int allWeight = totalWeight (packages);
  assert (allWeight % numContainers == 0);
  int targetWeight = allWeight / numContainers;
  for (std::size_t containerSize = 1; containerSize < packages.size (); ++containerSize) {
    std::set<Container> possible = partOfPowerSet (packages, containerSize, targetWeight);
    if (!possible.empty ()) {
      Container bestSoFar = *possible.begin ();
      for (const Container& cont : possible) {
        if (quantumEntanglement (cont) < quantumEntanglement (bestSoFar)) {
          bestSoFar = cont;
        }
      }
      return bestSoFar;
    }
    else {
      //std::cout << "Couldn't find a selection of " << containerSize << " elements.\n";
    }
  }
  throw "Impossible";
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::set<int> packages = readInput ();
  //std::cout << quantumEntanglement (bruteForceOptimize (packages)[0]) << "\n";
  //std::cout << quantumEntanglement (fasterIdea (packages)) << "\n";
  std::cout << quantumEntanglement (hopefullyEvenFasterIdea (packages, 3)) << "\n";
  std::cout << quantumEntanglement (hopefullyEvenFasterIdea (packages, 4)) << "\n";
  return 0;
}

// Goal weight: 520