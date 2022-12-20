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

using Number = unsigned long;

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

std::vector<Blueprint>
getInput ()
{
  std::vector<Blueprint> blueprints;
  while (true)
  {
    Number bluePrintNum;
    Number oreRobotOreCost = 0;
    Number clayRobotOreCost = 0;
    Number obsidianRobotOreCost = 0, obsidianRobotClayCost = 0;
    Number geodeRobotOreCost = 0, geodeRobotObsidianCost = 0;
    if (scanf ("Blueprint %ld:", &bluePrintNum) != 1) { break; }
    if (scanf (" Each ore robot costs %ld ore.", &oreRobotOreCost) != 1) { assert (false); }
    if (scanf (" Each clay robot costs %ld ore.", &clayRobotOreCost) != 1) { assert (false); }
    if (scanf (" Each obsidian robot costs %ld ore and %ld clay.", &obsidianRobotOreCost, &obsidianRobotClayCost) != 2) { assert (false); }
    if (scanf (" Each geode robot costs %ld ore and %ld obsidian.", &geodeRobotOreCost, &geodeRobotObsidianCost) != 2) { assert (false); }
    assert (bluePrintNum == blueprints.size () + 1);
    Blueprint current;
    current.m_number = bluePrintNum;
    current.m_recipes[ORE] = {ORE, {{ORE, oreRobotOreCost}}};
    current.m_recipes[CLAY] = {CLAY, {{ORE, clayRobotOreCost}}};
    current.m_recipes[OBSIDIAN] = {OBSIDIAN, {{ORE, obsidianRobotOreCost}, {CLAY, obsidianRobotClayCost}}};
    current.m_recipes[GEODE] = {GEODE, {{ORE, geodeRobotOreCost}, {OBSIDIAN, geodeRobotObsidianCost}}};
    blueprints.push_back (current);
  }
  return blueprints;
}



int main ()
{
  std::vector<Blueprint> blueprints = getInput ();

  return 0;
}