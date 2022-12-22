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

using Number = unsigned long;

const Number MAX_MINUTES = 24;

enum Resource {ORE, CLAY, OBSIDIAN, GEODE};

struct Recipe
{
  Resource m_robotType;
  std::map<Resource, Number> m_costs;
};

struct Blueprint
{
  Number m_number;
  std::map<Resource, Recipe> m_recipes;
};

struct State
{
  Number m_minutesPassed;
  std::map<Resource, Number> m_stockpiles;
  std::map<Resource, Number> m_robots;
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
    if (scanf (" Blueprint %ld:", &blueprintNum) != 1) { break; }
    if (scanf (" Each ore robot costs %ld ore.", &oreRobotOreCost) != 1) { assert (false); }
    if (scanf (" Each clay robot costs %ld ore.", &clayRobotOreCost) != 1) { assert (false); }
    if (scanf (" Each obsidian robot costs %ld ore and %ld clay.", &obsidianRobotOreCost, &obsidianRobotClayCost) != 2) { assert (false); }
    if (scanf (" Each geode robot costs %ld ore and %ld obsidian.", &geodeRobotOreCost, &geodeRobotObsidianCost) != 2) { assert (false); }
    assert (blueprintNum == blueprints.size () + 1);
    Blueprint current;
    current.m_number = blueprintNum;
    current.m_recipes[ORE] = {ORE, {{ORE, oreRobotOreCost}, {CLAY, 0}, {OBSIDIAN, 0}, {GEODE, 0}}};
    current.m_recipes[CLAY] = {CLAY, {{ORE, clayRobotOreCost}, {CLAY, 0}, {OBSIDIAN, 0}, {GEODE, 0}}};
    current.m_recipes[OBSIDIAN] = {OBSIDIAN, {{ORE, obsidianRobotOreCost}, {CLAY, obsidianRobotClayCost}, {OBSIDIAN, 0}, {GEODE, 0}}};
    current.m_recipes[GEODE] = {GEODE, {{ORE, geodeRobotOreCost}, {CLAY, 0}, {OBSIDIAN, geodeRobotObsidianCost}, {GEODE, 0}}};
    blueprints.push_back (current);
  }
  return blueprints;
}

bool
canMake (const Blueprint& blueprint, const State& current, Resource robot)
{
  for (const auto& pair : blueprint.m_recipes.at (robot).m_costs)
  {
    if (pair.second > current.m_stockpiles.at (pair.first)) { return false; }
  }
  return true;
}

std::map<Resource, Number>
getProductivity (const State& current)
{
  std::map<Resource, Number> products;
  for (const auto& pair : current.m_robots)
  {
    products[pair.first] = pair.second;
  }
  return products;
}

Number
upperBound (const State& current)
{
  Number alreadyHas = current.m_stockpiles.at (GEODE);
  Number increment = current.m_robots.at (GEODE);
  Number extra = 0;
  // Yeah, there's an n(n+1)/2 formula for this, but I'm hoping a bunch of additions isn't much worse than that.
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
  std::map<Resource, Number> production = getProductivity (current);
  State next = current;
  for (Resource thing : {ORE, CLAY, OBSIDIAN, GEODE})
  {
    next.m_stockpiles[thing] += production[thing];
    if (toBuild)
    {
      next.m_stockpiles[thing] -= blueprint.m_recipes.at (*toBuild).m_costs.at (thing);
    }
  }
  if (toBuild) { ++next.m_robots[*toBuild]; }
  ++next.m_minutesPassed;
  return next;
}

void
replaceIfBetter (std::optional<State>& bestSoFar, const State& current)
{
  if (!bestSoFar || (current.m_stockpiles.at (GEODE) > bestSoFar->m_stockpiles.at (GEODE)))
  {
    bestSoFar = current;
  }
}

void
findBestEndingState (const Blueprint& blueprint, const State& current, std::optional<State>& bestSoFar)
{
  if (current.m_minutesPassed == MAX_MINUTES)
  {
    replaceIfBetter (bestSoFar, current);
    return;
  }
  if (bestSoFar && bestSoFar->m_minutesPassed == MAX_MINUTES && bestSoFar->m_stockpiles.at (GEODE) > upperBound (current))
  {
    return;
  }
  //std::cout << current << "\n";
  std::map<Resource, Number> production = getProductivity (current);
  for (std::optional<Resource> robot : std::vector<std::optional<Resource>> {GEODE, OBSIDIAN, CLAY, ORE, std::nullopt})
  {
    if (!robot || canMake (blueprint, current, *robot))
    {
      findBestEndingState (blueprint, advance (blueprint, current, robot), bestSoFar);
    }
  }
}

std::map<Number, Number>
computeMaxGeodesPerBlueprint (const std::vector<Blueprint>& blueprints)
{
  std::map<Number, Number> results;
  for (const Blueprint& bp : blueprints)
  {
    State current = {0, {{ORE, 0}, {CLAY, 0}, {OBSIDIAN, 0}, {GEODE, 0}}, {{ORE, 1}, {CLAY, 0}, {OBSIDIAN, 0}, {GEODE, 0}}};
    std::optional<State> best = std::nullopt;
    findBestEndingState (bp, current, best);
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

int main ()
{
  std::vector<Blueprint> blueprints = getInput ();
  std::cout << "Number of blueprints is: " << blueprints.size () << "\n";
  std::cout << computeSumOfQualityLevels (computeMaxGeodesPerBlueprint (blueprints)) << "\n";
  return 0;
}