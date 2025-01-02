/// \file 2015Day09.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-09.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <list>
#include <set>

using Location = std::string;
using Cost = int;
using AdjList = std::map<Location, std::map<Location, Cost>>;

AdjList
readInput ()
{
  AdjList adjlist;
  std::string line, first, second, dist;
  while (std::getline (std::cin, line)) {
    std::size_t break1 = line.find (" to ");
    std::size_t break2 = line.find (" = ");
    first = line.substr (0, break1);
    second = line.substr (break1 + 4, break2 - break1 - 4);
    dist = line.substr (break2 + 3);
    if (!adjlist.contains (first)) { adjlist[first] = {}; }
    if (!adjlist.contains (second)) { adjlist[second] = {}; }
    adjlist[first][second] = atoi (dist.c_str ());
    adjlist[second][first] = atoi (dist.c_str ());
  }
  return adjlist;
}

Cost
bruteForceTSP (const AdjList& adjlist, bool best)
{
  // Maybe we could decide which link to skip (start/end) more intelligently?
  using Partial = std::pair<std::vector<Location>, Cost>;
  std::list<Partial> frontier;
  frontier.push_back ({{}, 0});
  Partial bestPath {{}, INT32_MAX};
  Partial worstPath {{}, 0};
  while (!frontier.empty ()) {
    Partial current = frontier.front ();
    frontier.pop_front ();
    std::set<Location> visited (current.first.cbegin (), current.first.cend ());
    if (visited.size () == adjlist.size ()) {
      if (current.second < bestPath.second) { bestPath = current; }
      if (current.second > worstPath.second) { worstPath = current; }
    }
    else {
      for (auto it = adjlist.cbegin (); it != adjlist.cend (); ++it) {
        if (!visited.contains (it->first)) {
          if (current.first.empty ()) {
            Partial next (current);
            next.first.push_back (it->first);
            frontier.push_back (next);
          }
          else {
            Partial next (current);
            next.second += adjlist.at (next.first.back ()).at (it->first);
            next.first.push_back (it->first);
            frontier.push_back (next);
          }
        }
      }
    }
  }
  return best ? bestPath.second : worstPath.second;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  AdjList adjlist = readInput ();
  std::cout << bruteForceTSP (adjlist, true) << "\n";
  std::cout << bruteForceTSP (adjlist, false) << "\n";
  return 0;
}
