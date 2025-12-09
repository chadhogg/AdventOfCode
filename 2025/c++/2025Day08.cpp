/// \file 2025Day08.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-08.

#include <cstddef>
#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include <algorithm>

using Distance = long long;

struct JunctionBox
{
  int x;
  int y;
  int z;
  auto operator<=> (const JunctionBox& other) const = default;
};

using Circuit = std::set<JunctionBox>;

Distance
squaredDistance (const JunctionBox& p1, const JunctionBox& p2)
{
  Distance x = (p1.x - p2.x);
  Distance y = (p1.y - p2.y);
  Distance z = (p1.z - p2.z);
  return x*x + y*y + z*z;
}

// In retrospect, I should have just had a collection of junction boxes.
// Not bothering to change it now.
using Problem = std::vector<Circuit>;

Problem
getInput ()
{
  Problem prob;
  int x, y, z;
  while (scanf ("%d,%d,%d", &x, &y, &z) == 3) {
    prob.push_back ({{x, y, z}});
  }
  return prob;
}

using Edge = std::pair<JunctionBox, JunctionBox>;

std::vector<Edge>
makeMinimumConnections (const Problem& prob, std::size_t count)
{
  std::vector<Edge> edges;
  std::set<std::pair<Distance, Edge>> possibleEdges;
  for (auto it = prob.begin (); it != prob.end (); ++it) {
    for (auto it2 = it + 1; it2 != prob.end (); ++it2) {
      possibleEdges.insert ({squaredDistance (*it->begin (), *it2->begin ()), {*it->begin (), *it2->begin ()}});
    }
  }
  while (edges.size () < count) {
    edges.push_back (possibleEdges.begin ()->second);
    possibleEdges.erase (possibleEdges.begin ());
  }
  return edges;
}

std::vector<Circuit>
getConnectedComponents (const Problem& prob, std::vector<Edge> edges)
{
  std::vector<Circuit> circuits;
  std::set<JunctionBox> todo;
  for (const Circuit& thing : prob) {
    for (const JunctionBox& thing2 : thing) {
      todo.insert (thing2);
    }
  }
  while (!todo.empty ()) {
    Circuit explored, frontier;
    frontier.insert (*todo.begin ());
    todo.erase (todo.begin ());
    while (!frontier.empty ()) {
      JunctionBox current = *frontier.begin ();
      frontier.erase (current);
      for (const Edge& edge : edges) {
        if (edge.first == current) {
          if (todo.count (edge.second) != 0) {
            frontier.insert (edge.second);
            todo.erase (edge.second);
          }
        }
        else if (edge.second == current) {
          if (todo.count (edge.first) != 0) {
            frontier.insert (edge.first);
            todo.erase (edge.first);
          }
        }
      }
      explored.insert (current);
    }
    circuits.push_back (explored);
  }
  return circuits;
}

bool
componentSizeLess (const Circuit& c1, const Circuit& c2)
{
  return c1.size () < c2.size ();
}

std::size_t
multiplySizes (std::vector<Circuit>& components)
{
  std::sort (components.begin (), components.end (), componentSizeLess);
  return components.at (components.size () - 1).size () * components.at (components.size () - 2).size () * components.at (components.size () - 3).size ();
}

Distance
connectAll (const Problem& prob)
{
  // Get all possible cables, sorted from shortest to longest.
  std::set<std::pair<Distance, Edge>> possibleEdges;
  for (auto it = prob.begin (); it != prob.end (); ++it) {
    for (auto it2 = it + 1; it2 != prob.end (); ++it2) {
      possibleEdges.insert ({squaredDistance (*it->begin (), *it2->begin ()), {*it->begin (), *it2->begin ()}});
    }
  }

  // Make a vector of initially-unary circuits.
  std::vector<Circuit> circuits;
  for (auto it = prob.begin (); it != prob.end (); ++it) {
    circuits.push_back ({*it});
  }

  // Keep track of our answer for the last pair that we connected.
  Distance mostRecent = 0;
  // Repeat until finally all junction boxes are in a single circuit.
  while (circuits.size () > 1) {
    // Get the first (shortest) un-used cable.
    std::pair<Distance, Edge> current = *possibleEdges.begin ();
    possibleEdges.erase (possibleEdges.begin ());
    mostRecent = current.second.first.x * current.second.second.x;
    // Go find the circuits containing those junction boxes.
    std::size_t found1 = 0;
    std::size_t found2 = 0;
    for (std::size_t index = 0; index < circuits.size (); ++index) {
      if (circuits.at (index).count (current.second.first)) { found1 = index; }
      if (circuits.at (index).count (current.second.second)) { found2 = index; }
    }
    // If the junction boxes are in different circuits, combine them.
    if (found1 != found2) {
      circuits[found1].insert (circuits[found2].begin (), circuits[found2].end ());
      circuits.erase (circuits.begin () + found2);
    }
  }

  return mostRecent;
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::vector<Edge> edges = makeMinimumConnections (prob, 1000);
  std::vector<Circuit> components = getConnectedComponents (prob, edges);
  std::cout << multiplySizes (components) << "\n";
  std::cout << connectAll (prob) << "\n";
  return EXIT_SUCCESS;
}

