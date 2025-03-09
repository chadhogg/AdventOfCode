/// \file 2016Day11.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-11.


#include <iostream>
#include <string>
#include <cassert>
#include <regex>
#include <set>
#include <array>
#include <map>
#include <climits>
#include <bitset>
#include <queue>

using Element = std::string;
const std::size_t NUM_FLOORS = 4;
const std::size_t MAX_OBJECTS = 16;

struct Problem {
  std::size_t elevator;
  std::array<std::set<Element>, NUM_FLOORS> generators;
  std::array<std::set<Element>, NUM_FLOORS> microchips;
  auto operator<=> (const Problem& other) const = default;

  // Checks to make sure that if there are any generators on a floor, all microchips on that floor
  //   have their matching generators.
  bool
  isSafe () const {
    for (std::size_t floor = 0; floor < NUM_FLOORS; ++floor) {
      if (!generators[floor].empty()) {
        for (const Element& chip : microchips[floor]) {
          if (!generators[floor].contains (chip)) {
            return false;
          }
        }
      }
    }
    return true;
  }

  bool
  isGoal () const {
    return generators[0].empty () && generators[1].empty () && generators[2].empty ()
        && microchips[0].empty () && microchips[1].empty () && microchips[2].empty ();
  }
};

template <std::size_t N>
constexpr auto operator<=> (const std::bitset<N>& first, const std::bitset<N>& second)
{
  return first.to_ulong () <=> second.to_ulong ();
}

template <typename E, std::size_t N>
constexpr auto operator<=> (const std::array<E, N>& first, const std::array<E, N>& second)
{
  for (std::size_t index = 0; index < N; ++index) {
    if (auto c = first[index] <=> second[index]; c != 0) { return c; }
  }
  return (first[N - 1] <=> second[N - 1]);
}


struct ProblemOptimized {
  uint16_t elevator;
  std::array<std::bitset<MAX_OBJECTS>, NUM_FLOORS> generators;
  std::array<std::bitset<MAX_OBJECTS>, NUM_FLOORS> microchips;

  constexpr auto operator<=> (const ProblemOptimized& other) const
  {
    if (auto c = elevator <=> other.elevator; c != 0) { return c; }
    if (auto c = generators <=> other.generators; c != 0) { return c; }
    return (microchips <=> other.microchips);
  }

  // Checks to make sure that if there are any generators on a floor, all microchips on that floor
  //   have their matching generators.
  bool
  isSafe () const
  {
    for (std::size_t floor = 0; floor < NUM_FLOORS; ++floor) {
      if (!generators[floor].none()) {
        for (std::size_t index = 0; index < MAX_OBJECTS; ++index) {
          if (microchips[floor][index] && !generators[floor][index]) {
            return false;
          }
        }
      }
    }
    return true;
  }

  bool
  isGoal () const
  {
    return generators[0].none () && generators[1].none () && generators[2].none ()
        && microchips[0].none () && microchips[1].none () && microchips[2].none ();
  }

  std::size_t
  heuristic () const
  {
    return (generators[0].count () + microchips[0].count ()) * 3
        + (generators[1].count () + microchips[1].count ()) * 2
        + (generators[2].count () + microchips[2].count ());
  }
};

struct WrappedProblemOptimized
{
  ProblemOptimized prob;
  std::size_t steps;
  bool
  operator> (const WrappedProblemOptimized& other) const
  {
    return steps + prob.heuristic () > other.steps + other.prob.heuristic () 
      || (steps + prob.heuristic () == other.steps + other.prob.heuristic () && prob > other.prob);
  }
};


Problem
readInput ()
{
  Problem prob {0, {}, {}};
  std::basic_regex generatorRegex ("a (\\w+) generator");
  std::basic_regex microchipRegex ("a (\\w+)-compatible microchip");
  std::string line;
  int floor = 0;
  while (std::getline (std::cin, line)) {
    std::string tempLine = line;
    for (std::smatch smatch; std::regex_search (tempLine, smatch, generatorRegex);) {
      prob.generators[floor].insert (smatch[1].str ());
      tempLine = smatch.suffix ();
    }
    tempLine = line;
    for (std::smatch smatch; std::regex_search (tempLine, smatch, microchipRegex);) {
      prob.microchips[floor].insert (smatch[1].str ());
      tempLine = smatch.suffix ();
    }
    ++floor;
  }
  return prob;
}


ProblemOptimized
convert (const Problem& prob) {
  ProblemOptimized probOpt;
  probOpt.elevator = prob.elevator;
  std::map<std::string, int> mapping;
  for (std::size_t floor = 0; floor < NUM_FLOORS; ++floor) {
    for (Element e : prob.generators[floor]) {
      assert (!mapping.contains (e));
      assert (mapping.size() < MAX_OBJECTS);
      mapping[e] = mapping.size ();
      probOpt.generators[floor][mapping[e]] = true;
    }
    for (Element e : prob.microchips[floor]) {
      assert (mapping.contains (e));
      probOpt.microchips[floor][mapping[e]] = true;
    }
  }
  return probOpt;
}

std::set<ProblemOptimized>
successors (const ProblemOptimized& prob)
{
  std::set<ProblemOptimized> successors;
  std::vector<std::size_t> nextFloors;
  if (prob.elevator > 0) { nextFloors.push_back (prob.elevator - 1); }
  if (prob.elevator < NUM_FLOORS - 1) { nextFloors.push_back (prob.elevator + 1); }
  for (int nextFloor : nextFloors) {
    // First, try taking a single generator to the next floor
    for (std::size_t index = 0; index < MAX_OBJECTS; ++index) {
      if (prob.generators[prob.elevator][index]) {
        ProblemOptimized copy = prob;
        copy.generators[prob.elevator][index] = false;
        copy.generators[nextFloor][index] = true;
        copy.elevator = nextFloor;
        if (copy.isSafe ()) {
          successors.insert (copy);
        }
      }
    }
    // Now, try taking a pair of generators to the next floor
    for (std::size_t index1 = 0; index1 < MAX_OBJECTS; ++index1) {
      if (prob.generators[prob.elevator][index1]) {
        for (std::size_t index2 = index1 + 1; index2 < MAX_OBJECTS; ++index2) {
          if (prob.generators[prob.elevator][index2]) {
            ProblemOptimized copy = prob;
            copy.generators[prob.elevator][index1] = false;
            copy.generators[prob.elevator][index2] = false;
            copy.generators[nextFloor][index1] = true;
            copy.generators[nextFloor][index2] = true;
            copy.elevator = nextFloor;
            if (copy.isSafe ()) {
              successors.insert (copy);
            }
          }
        }
      }
    }
    // Now, try taking a single microchip to the next floor
    for (std::size_t index = 0; index < MAX_OBJECTS; ++index) {
      if (prob.microchips[prob.elevator][index]) {
        ProblemOptimized copy = prob;
        copy.microchips[prob.elevator][index] = false;
        copy.microchips[nextFloor][index] = true;
        copy.elevator = nextFloor;
        if (copy.isSafe ()) {
          successors.insert (copy);
        }
      }
    }
    // Now, try taking a pair of microchips to the next floor
    for (std::size_t index1 = 0; index1 < MAX_OBJECTS; ++index1) {
      if (prob.microchips[prob.elevator][index1]) {
        for (std::size_t index2 = index1 + 1; index2 < MAX_OBJECTS; ++index2) {
          if (prob.microchips[prob.elevator][index2]) {
            ProblemOptimized copy = prob;
            copy.microchips[prob.elevator][index1] = false;
            copy.microchips[prob.elevator][index2] = false;
            copy.microchips[nextFloor][index1] = true;
            copy.microchips[nextFloor][index2] = true;
            copy.elevator = nextFloor;
            if (copy.isSafe ()) {
              successors.insert (copy);
            }
          }
        }
      }
    }
    // Finally, try taking a matched pair to the next floor
    for (std::size_t index = 0; index < MAX_OBJECTS; ++index) {
      if (prob.generators[prob.elevator][index] && prob.microchips[prob.elevator][index]) {
        ProblemOptimized copy = prob;
        copy.microchips[prob.elevator][index] = false;
        copy.microchips[nextFloor][index] = true;
        copy.generators[prob.elevator][index] = false;
        copy.generators[nextFloor][index] = true;
        copy.elevator = nextFloor;
        if (copy.isSafe ()) {
          successors.insert (copy);
        }
      }
    }
  }
  return successors;
}


int
search (const ProblemOptimized& prob)
{
  long statesVisited = 0;
  long branchesConsidered = 0;
  std::map<ProblemOptimized, std::size_t> reached;
  std::map<ProblemOptimized, std::size_t> frontier;
  frontier[prob] = 0;
  while (!frontier.empty ()) {
    ProblemOptimized currentProb = frontier.begin ()->first;
    std::size_t currentSteps = frontier[currentProb];
    for (auto iter = frontier.cbegin (); iter != frontier.cend (); ++iter) {
      if (iter->second < currentSteps) {
        currentProb = iter->first;
        currentSteps = frontier[currentProb];
      }
    }
    frontier.erase (currentProb);
    if (currentProb.isGoal ()) { return currentSteps; }
    if (reached.contains (currentProb) && reached[currentProb] <= currentSteps) {
      assert (false);
      continue;
    }
    reached[currentProb] = currentSteps;
    for (const ProblemOptimized& successor : successors (currentProb)) {
      if ((!reached.contains (successor) || reached[successor] > currentSteps + 1)
      && (!frontier.contains (successor) || frontier[successor] > currentSteps + 1)) {
        frontier[successor] = currentSteps + 1;
        ++branchesConsidered;
      }
    }
    ++statesVisited;
    if (statesVisited % 10000 == 0) {
      std::cout << statesVisited << " " << branchesConsidered / (double)statesVisited << " " << currentSteps << "\n";
      //std::cout << "Visited " << statesVisited << " states\n\tcurrent length " << currentSteps << "\n\taverage branching factor " << branchesConsidered / (double)statesVisited << "\n\tfrontier size " << frontier.size () << "\n";
    }
  }
  return 0;
}

ProblemOptimized
chooseCheapest (const std::map<ProblemOptimized, std::size_t>& frontier)
{
  ProblemOptimized currentProb = frontier.cbegin ()->first;
  std::size_t currentSteps = frontier.at (currentProb);
  std::size_t cost = currentSteps + currentProb.heuristic ();
  for (auto iter = frontier.cbegin (); iter != frontier.cend (); ++iter) {
    if (iter->second + iter->first.heuristic () < cost) {
      currentProb = iter->first;
      currentSteps = iter->second;
      cost = currentSteps + currentProb.heuristic ();
    }
  }
  return currentProb;
}

int
searchAStar (const ProblemOptimized& prob)
{
  long statesVisited = 0;
  long branchesConsidered = 0;
  std::map<ProblemOptimized, std::size_t> reached;
  std::map<ProblemOptimized, std::size_t> frontier;
  std::priority_queue<WrappedProblemOptimized, std::vector<WrappedProblemOptimized>, std::greater<WrappedProblemOptimized>> cheapest;
  frontier[prob] = 0;
  cheapest.push ({prob, 0});
  while (!frontier.empty ()) {
    WrappedProblemOptimized cheapestProb = cheapest.top ();
    cheapest.pop ();
    if (reached.contains (cheapestProb.prob)) {
      if (frontier.contains (cheapestProb.prob)) {
        frontier.erase (cheapestProb.prob);
      }
      continue;
    }
    assert (frontier.contains (cheapestProb.prob));
    ProblemOptimized currentProb = cheapestProb.prob;
    std::size_t currentSteps = cheapestProb.steps;

    frontier.erase (currentProb);
    if (currentProb.isGoal ()) { return currentSteps; }

    reached[currentProb] = currentSteps;
    for (const ProblemOptimized& successor : successors (currentProb)) {
      if ((!reached.contains (successor) || reached[successor] > currentSteps + 1)
      && (!frontier.contains (successor) || frontier[successor] > currentSteps + 1)) {
        frontier[successor] = currentSteps + 1;
        cheapest.push ({successor, currentSteps + 1});
        ++branchesConsidered;
      }
    }
    ++statesVisited;
    if (statesVisited % 10000 == 0) {
      //std::cout << statesVisited << " " << branchesConsidered / (double)statesVisited << " " << currentSteps << "\n";
      //std::cout << "Visited " << statesVisited << " states\n\tcurrent length " << currentSteps << "\n\taverage branching factor " << branchesConsidered / (double)statesVisited << "\n\tfrontier size " << frontier.size () << "\n";
    }
  }
  return 0;
}


/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Problem prob = readInput ();
  ProblemOptimized probOpt = convert (prob);
  std::cout << searchAStar (probOpt) << "\n";

  prob.generators[0].insert ("elerium");
  prob.generators[0].insert ("dilithium");
  prob.microchips[0].insert ("elerium");
  prob.microchips[0].insert ("dilithium");

  probOpt = convert (prob);
  std::cout << searchAStar (probOpt) << "\n";
  return 0;
}

