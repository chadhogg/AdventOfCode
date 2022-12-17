/// \file 2022Day16.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-16.

// Observation: Movements that don't end in opening a value accomplish nothing and are not worth doing.
// Observation: I can pre-compute the shortest paths between any two valves.
// These two together reduce the problem to finding the optimal order of values to open ignoring movement.
// Of course, there are still 15! possible orders to consider.
// A greedy heuristic always moves to the biggest closed valve (maybe opening others you pass by?)
// Can we try that first and prune branches that are definitively worse than it?
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <stack>

const unsigned MAX_MINUTES = 30;

struct Valve
{
  std::string m_name;
  unsigned m_flowRate;
  bool m_opened;
  std::set<std::string> m_tunnels;
};

struct Problem
{
  std::map<std::string, struct Valve> m_valves;
};

struct State
{
  std::set<std::string> m_openValves;
  int m_minutesLeft;
  unsigned long m_pressureReleased;
  std::string m_location;
};

Problem
getInput ()
{
  Problem prob;
  std::string line;
  std::string part1 = "Valve ";
  std::string part2 = " has flow rate=";
  std::string part3a = "; tunnels lead to valves ";
  std::string part3b = "; tunnel leads to valve ";
  std::string part4 = ", ";
  while (std::getline (std::cin, line))
  {
    if (line.empty ()) { break; }
    Valve current;
    current.m_opened = false;
    std::size_t part1Loc = line.find (part1);
    assert (part1Loc == 0);
    std::size_t part2Loc = line.find (part2);
    assert (part2Loc == part1.size () + 2);
    current.m_name = line.substr (part1.size (), 2);
    current.m_flowRate = atoi (line.substr (part1.size () + 2 + part2.size ()).c_str ());
    std::size_t part3aLoc = line.find (part3a);
    std::size_t part3bLoc = line.find (part3b);
    assert (part3aLoc != std::string::npos || part3bLoc != std::string::npos);
    if (part3bLoc != std::string::npos)
    {
      assert (part3bLoc + part3b.size () + 2 == line.size ());
      current.m_tunnels.insert (line.substr (part3bLoc + part3b.size ()));
    }
    else
    {
      std::size_t now = part3aLoc + part3a.size ();
      while (now < line.size ())
      {
        current.m_tunnels.insert (line.substr (now, 2));
        now += 2;
        if (now < line.size ())
        {
          assert (line.substr (now, 2) == part4);
          now += 2;
        }
      }
    }
    prob.m_valves.insert ({current.m_name, current});
  }
  return prob;
}

using PathDistances = std::map<std::string, std::map<std::string, unsigned>>;

PathDistances floydWarshall (const Problem& prob)
{
  PathDistances result;
  for (const auto& v1 : prob.m_valves)
  {
    result[v1.first] = {};
    for (const auto& v2 : prob.m_valves)
    {
      if (v1.first == v2.first) {result[v1.first][v2.first] = 0; }
      else if (v1.second.m_tunnels.count (v2.first) == 1) { result[v1.first][v2.first] = 1; }
      else { result[v1.first][v2.first] = MAX_MINUTES + 1; }
    }
  }
  for (const auto& v1 : prob.m_valves)
  {
    for (const auto& v2 : prob.m_valves)
    {
      for (const auto& v3 : prob.m_valves)
      {
        result[v2.first][v3.first] = std::min (result[v2.first][v3.first],
          result[v2.first][v1.first] + result[v1.first][v3.first]);
      }
    }
  }
  return result;
}

void
advanceState (const Problem& prob, State& state, int minutes)
{
  int pressurePerMinute = 0;
  for (const std::string& valve : state.m_openValves)
  {
    pressurePerMinute += prob.m_valves.at (valve).m_flowRate;
  }
  state.m_pressureReleased += minutes * pressurePerMinute;
  state.m_minutesLeft -= minutes;
}

State
depthFirstSearch (const Problem& prob, const PathDistances& distances)
{
  State initial;
  initial.m_location = "AA";
  initial.m_minutesLeft = MAX_MINUTES;
  initial.m_pressureReleased = 0;
  State best;
  std::stack<State> frontier;
  frontier.push (initial);
  while (!frontier.empty ())
  {
    State current = frontier.top ();
    frontier.pop ();
    for (const auto& valve : prob.m_valves)
    {
      if (valve.second.m_flowRate != 0 && current.m_openValves.count (valve.first) == 0)
      {
        State copy = current;
        advanceState (prob, copy, distances.at (copy.m_location).at (valve.first) + 1);
        copy.m_openValves.insert (valve.first);
        copy.m_location = valve.first;
        if (copy.m_minutesLeft >= 0)
        {
          frontier.push (copy);
        }
      }
    }
    advanceState (prob, current, current.m_minutesLeft);
    if (current.m_pressureReleased > best.m_pressureReleased) { best = current; }
  }
  return best;
}

struct DoubleState
{
  std::set<std::string> m_meOpenedValves;
  std::set<std::string> m_elephantOpenedValves;
  int m_meMinutesLeft;
  int m_elephantMinutesLeft;
  unsigned long m_pressureReleased;
  std::string m_meLocation;
  std::string m_elephantLocation;
};

void
advanceDoubleState (const Problem& prob, DoubleState& state, int meMinutes, int elephantMinutes)
{
  int mePressurePerMinute = 0;
  for (const std::string& valve : state.m_meOpenedValves)
  {
    mePressurePerMinute += prob.m_valves.at (valve).m_flowRate;
  }
  state.m_pressureReleased += meMinutes * mePressurePerMinute;
  state.m_meMinutesLeft -= meMinutes;

  int elephantPressurePerMinute = 0;
  for (const std::string& valve : state.m_elephantOpenedValves)
  {
    elephantPressurePerMinute += prob.m_valves.at (valve).m_flowRate;
  }
  state.m_pressureReleased += elephantMinutes * elephantPressurePerMinute;
  state.m_elephantMinutesLeft -= elephantMinutes;
}

DoubleState
depthFirstSearch2 (const Problem& prob, const PathDistances& distances)
{
  DoubleState initial;
  initial.m_meLocation = "AA";
  initial.m_elephantLocation = "AA";
  initial.m_meMinutesLeft = 26;
  initial.m_elephantMinutesLeft = 26;
  initial.m_pressureReleased = 0;

  DoubleState best;
  std::stack<DoubleState> frontier;
  frontier.push (initial);
  while (!frontier.empty ())
  {
    DoubleState current = frontier.top ();
    frontier.pop ();
    if (current.m_meMinutesLeft > 0)
    {
      for (const auto& valve : prob.m_valves)
      {
        if (valve.second.m_flowRate != 0 && current.m_meOpenedValves.count (valve.first) == 0)
        {
          DoubleState copy = current;
          advanceDoubleState (prob, copy, distances.at (copy.m_meLocation).at (valve.first) + 1, 0);
          copy.m_meOpenedValves.insert (valve.first);
          copy.m_meLocation = valve.first;
          if (copy.m_meMinutesLeft >= 0)
          {
            frontier.push (copy);
          }
        }
      }
      advanceDoubleState (prob, current, current.m_meMinutesLeft, 0);
      frontier.push (current);
    }
    else
    {
      for (const auto& valve : prob.m_valves)
      {
        if (valve.second.m_flowRate != 0 && current.m_meOpenedValves.count (valve.first) == 0 && current.m_elephantOpenedValves.count (valve.first) == 0)
        {
          DoubleState copy = current;
          advanceDoubleState (prob, copy, 0, distances.at (copy.m_elephantLocation).at (valve.first) + 1);
          copy.m_elephantOpenedValves.insert (valve.first);
          copy.m_elephantLocation = valve.first;
          if (copy.m_elephantMinutesLeft >= 0)
          {
            frontier.push (copy);
          }
        }
      }
      advanceDoubleState (prob, current, 0, current.m_elephantMinutesLeft);
      if (current.m_pressureReleased > best.m_pressureReleased) { best = current; }
    }
  }
  return best;
}

int main () {
  Problem prob = getInput ();
  PathDistances distances = floydWarshall (prob);
  std::cout << depthFirstSearch (prob, distances).m_pressureReleased << "\n";
  std::cout << depthFirstSearch2 (prob, distances).m_pressureReleased << "\n";
  return 0;
}
