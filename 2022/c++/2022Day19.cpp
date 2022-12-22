/// \file 2022Day19.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-19.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <set>
#include <queue>
#include <optional>
#include <array>

using Number = unsigned int;

enum Resource {ORE, CLAY, OBSIDIAN, GEODE};

struct Recipe
{
  Resource m_robotType;
  std::array<Number, 4> m_costs;
};

struct Blueprint
{
  Number m_number;
  std::array<Recipe, 4> m_recipes;
};

struct State
{
  Number m_minutesPassed;
  std::array<Number, 4> m_stockpiles;
  std::array<Number, 4> m_robots;
};

std::ostream&
operator<< (std::ostream& out, const State& state)
{
  out << "After " << state.m_minutesPassed << " minutes you have:\n";
  out << "\t" << state.m_stockpiles.at (ORE) << " ore, " << state.m_stockpiles.at (CLAY) << " clay, " << state.m_stockpiles.at (OBSIDIAN) << " obsidian, and " << state.m_stockpiles.at (GEODE) << " geodes.\n";
  out << "\t" << state.m_robots.at (ORE) << " ore bots, " << state.m_robots.at (CLAY) << " clay bots, " << state.m_robots.at (OBSIDIAN) << " obsidian bots, and " << state.m_robots.at (GEODE) << " geode bots.\n";
  return out;
}

std::vector<Blueprint>
getInput ()
{
  std::vector<Blueprint> blueprints;
  while (true)
  {
    Number blueprintNum;
    Number oreRobotOreCost = 0;
    Number clayRobotOreCost = 0;
    Number obsidianRobotOreCost = 0, obsidianRobotClayCost = 0;
    Number geodeRobotOreCost = 0, geodeRobotObsidianCost = 0;
    if (scanf (" Blueprint %d:", &blueprintNum) != 1) { break; }
    if (scanf (" Each ore robot costs %d ore.", &oreRobotOreCost) != 1) { assert (false); }
    if (scanf (" Each clay robot costs %d ore.", &clayRobotOreCost) != 1) { assert (false); }
    if (scanf (" Each obsidian robot costs %d ore and %d clay.", &obsidianRobotOreCost, &obsidianRobotClayCost) != 2) { assert (false); }
    if (scanf (" Each geode robot costs %d ore and %d obsidian.", &geodeRobotOreCost, &geodeRobotObsidianCost) != 2) { assert (false); }
    assert (blueprintNum == blueprints.size () + 1);
    Blueprint current;
    current.m_number = blueprintNum;
    current.m_recipes[ORE] = {ORE, {oreRobotOreCost, 0, 0, 0}};
    current.m_recipes[CLAY] = {CLAY, {clayRobotOreCost, 0, 0, 0}};
    current.m_recipes[OBSIDIAN] = {OBSIDIAN, {obsidianRobotOreCost, obsidianRobotClayCost, 0, 0}};
    current.m_recipes[GEODE] = {GEODE, {geodeRobotOreCost, 0, geodeRobotObsidianCost, 0}};
    blueprints.push_back (current);
  }
  return blueprints;
}

bool
canMake (const Blueprint& blueprint, const State& current, Resource robot)
{
  for (Resource r : {ORE, CLAY, OBSIDIAN})
  {
    if (blueprint.m_recipes[robot].m_costs[r] > current.m_stockpiles[r]) { return false; }
  }
  return true;
}

Number
upperBound (const State& current, const Number MAX_MINUTES)
{
  Number alreadyHas = current.m_stockpiles[GEODE];
  Number increment = current.m_robots[GEODE];
  Number extra = 0;
  // Yeah, there's an n(n+1)/2 formula for this, but do I trust that I have the bounds on the sum correct?
  for (Number minute = current.m_minutesPassed; minute < MAX_MINUTES; ++minute)
  {
    extra += increment;
    ++increment;
  }
  return alreadyHas + extra;
}

State
advance (const Blueprint& blueprint, const State& current, std::optional<Resource> toBuild)
{
  State next = current;
  for (Resource thing : {ORE, CLAY, OBSIDIAN, GEODE})
  {
    next.m_stockpiles[thing] += current.m_robots[thing];
    if (toBuild)
    {
      next.m_stockpiles[thing] -= blueprint.m_recipes[*toBuild].m_costs[thing];
    }
  }
  if (toBuild) { ++next.m_robots[*toBuild]; }
  ++next.m_minutesPassed;
  return next;
}

void
replaceIfBetter (std::optional<State>& bestSoFar, const State& current)
{
  if (!bestSoFar || (current.m_stockpiles[GEODE] > bestSoFar->m_stockpiles[GEODE]))
  {
    bestSoFar = current;
  }
}

void
findBestEndingState (const Blueprint& blueprint, const State& current, std::optional<State>& bestSoFar, const Number MAX_MINUTES)
{
  // We've reached the end of recursion.
  if (current.m_minutesPassed == MAX_MINUTES)
  {
    replaceIfBetter (bestSoFar, current);
    return;
  }
  // Bail on this if we've found a solution that is provably better.
  // Because the best in several instances is 0 or 1, this is almost never true.
  else if (bestSoFar && bestSoFar->m_stockpiles[GEODE] > upperBound (current, MAX_MINUTES))
  {
    return;
  }
  // Questionable heuristic: Whenever we can make a geode robot, we should.
  // Seems to not break optimality.
  else if (canMake (blueprint, current, GEODE))
  {
    findBestEndingState (blueprint, advance (blueprint, current, GEODE), bestSoFar, MAX_MINUTES);
  }
  // Explore various branches.
  else
  {
    int branches = 0;
    // Only try to make obsidian robots if we don't already have enough to cover the cost of a new geode each minute.
    if (canMake (blueprint, current, OBSIDIAN) && current.m_robots[OBSIDIAN] < blueprint.m_recipes[GEODE].m_costs[OBSIDIAN])
    {
      findBestEndingState (blueprint, advance (blueprint, current, OBSIDIAN), bestSoFar, MAX_MINUTES);
      ++branches;
    }
    // Only try to make clay robots if we don't already have enough to cover the cost of a new obsidian each minute.
    if (canMake (blueprint, current, CLAY) && current.m_robots[CLAY] < blueprint.m_recipes[OBSIDIAN].m_costs[CLAY])
    {
      findBestEndingState (blueprint, advance (blueprint, current, CLAY), bestSoFar, MAX_MINUTES);
      ++branches;
    }
    // Only try to make ore robots if we don't already have enough to cover the cost of a new clay / obsidian / geode each minute.
    if (canMake (blueprint, current, ORE) && (current.m_robots[ORE] < blueprint.m_recipes[GEODE].m_costs[ORE] || current.m_robots[ORE] < blueprint.m_recipes[OBSIDIAN].m_costs[ORE] || current.m_robots[ORE] < blueprint.m_recipes[CLAY].m_costs[ORE]))
    {
      findBestEndingState (blueprint, advance (blueprint, current, ORE), bestSoFar, MAX_MINUTES);
      ++branches;
    }
    // Defensible heuristic: If it was possible and desirable to make every robot, don't not make one.
    if (branches < 3)
    {
      findBestEndingState (blueprint, advance (blueprint, current, std::nullopt), bestSoFar, MAX_MINUTES);
    }
  }
}

std::map<Number, Number>
computeMaxGeodesPerBlueprint (const std::vector<Blueprint>& blueprints, const int MAX_MINUTES)
{
  std::map<Number, Number> results;
  for (const Blueprint& bp : blueprints)
  {
    State current = {0, {0, 0, 0, 0}, {1, 0, 0, 0}};
    std::optional<State> best = std::nullopt;
    findBestEndingState (bp, current, best, MAX_MINUTES);
    results[bp.m_number] = best->m_stockpiles[GEODE];
    std::cout << "Finished blueprint " << bp.m_number << " with result " << best->m_stockpiles[GEODE] << "\n";
  }
  return results;
}

Number
computeSumOfQualityLevels (const std::map<Number, Number>& maxGeodes)
{
  Number total = 0;
  for (const auto& pair : maxGeodes)
  {
    total += pair.first * pair.second;
  }
  return total;
}

Number
multiplyGeodeNumbers (const std::map<Number, Number>& maxGeodes)
{
  Number product = 1;
  for (const auto& pair : maxGeodes)
  {
    product *= pair.second;
  }
  return product;
}

int main ()
{
  std::vector<Blueprint> blueprints = getInput ();
  //std::cout << computeSumOfQualityLevels (computeMaxGeodesPerBlueprint (blueprints, 24)) << "\n";
  blueprints.resize (3);
  std::cout << multiplyGeodeNumbers (computeMaxGeodesPerBlueprint (blueprints, 32)) << "\n";
  return 0;
}

// My answers:
// Part 1: 1266
// Part 2: 5800
