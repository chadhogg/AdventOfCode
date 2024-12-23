/// \file 2024Day23.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-23.


#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>
#include <cmath>
#include <utility>
#include <map>
#include <optional>
#include <chrono>
#include <thread>
#include <queue>

using Computer = std::string;

struct Network
{
  std::set<Computer> vertices;
  std::map<Computer, std::set<Computer>> edges;
};

Network
readInput ()
{
  Network network;
  std::string line;
  while (std::getline (std::cin, line)) {
    assert (line.length () == 5 && line[2] == '-');
    std::string first = line.substr (0, 2);
    std::string second = line.substr (3);
    network.vertices.insert (first);
    network.vertices.insert (second);
    if (!network.edges.contains (first)) { network.edges[first] = {}; }
    if (!network.edges.contains (second)) { network.edges[second] = {}; }
    network.edges[first].insert (second);
    network.edges[second].insert (first);
  }
  return network;
}

std::set<std::set<Computer>>
findCompleteSubgraphs (const Network& network, unsigned int size)
{
  std::set<std::set<Computer>> results;
  std::set<std::set<Computer>> incomplete;
  for (const Computer & comp : network.vertices) {
    for (const Computer& comp2 : network.edges.at (comp)) {
      incomplete.insert ({comp, comp2});
    }
  }
  while (!incomplete.empty ()) {
    std::set<Computer> current = *incomplete.begin ();
    incomplete.erase (current);
    if (current.size () == size) {
      results.insert (current);
    }
    else {
      for (const Computer& unconnected : network.vertices) {
        if (!current.contains (unconnected)) {
          bool allConnected = true;
          for (const Computer& already : current) {
            if (!network.edges.at (unconnected).contains (already)) {
              allConnected = false;
              break;
            }
          }
          if (allConnected) {
            std::set<Computer> extended = current;
            extended.insert (unconnected);
            incomplete.insert (extended);
          }
        }
      }
    }
  }
  return results;
}

int
part1 (const Network& network)
{
  std::set<std::set<Computer>> complete = findCompleteSubgraphs (network, 3);
  int count = 0;
  for (const std::set<Computer>& subgraph : complete) {
    assert (subgraph.size () == 3);
    for (const Computer& one : subgraph) {
      for (const Computer& two : subgraph) {
        assert (one == two || network.edges.at (one).contains (two));
      }
    }
    bool containsT = false;
    for (const Computer& comp : subgraph) {
      if (comp.find ('t') == 0) {
        containsT = true;
      }
    }
    if (containsT) { ++count; }
  }
  return count;
}

std::set<std::set<Computer>>
findAllCompleteSubgraphs (const Network& network)
{
  std::set<std::set<Computer>> results;
  std::set<std::set<Computer>> incomplete;
  for (const Computer & comp : network.vertices) {
    for (const Computer& comp2 : network.edges.at (comp)) {
      incomplete.insert ({comp, comp2});
    }
  }
  while (!incomplete.empty ()) {
    std::set<Computer> current = *incomplete.begin ();
    incomplete.erase (current);
    bool grew = false;
    for (const Computer& unconnected : network.vertices) {
      if (!current.contains (unconnected)) {
        bool allConnected = true;
        for (const Computer& already : current) {
          if (!network.edges.at (unconnected).contains (already)) {
            allConnected = false;
            break;
          }
        }
        if (allConnected) {
          std::set<Computer> extended = current;
          extended.insert (unconnected);
          incomplete.insert (extended);
          grew = true;
        }
      }  
    }
    if (!grew) {
      results.insert (current);
    }
  }
  return results;
}

bool
allConnected (const Network& network, const std::set<Computer>& first, const std::set<Computer>& second)
{
  for (const Computer& a: first) {
    for (const Computer& b: second) {
      if (!network.edges.at (a).contains (b)) { return false; }
    }
  }
  return true;
}

std::ostream&
operator<< (std::ostream& out, const std::set<Computer>& stuff)
{
  bool first = true;
  for (const Computer& thing: stuff) {
    if (!first) { out << ","; }
    out << thing;
    first = false;
  }
  return out;
}

std::set<std::set<Computer>>
bronKerbosch (const Network& network, std::set<Computer> partialClique, std::set<Computer> possible, std::set<Computer> excluded)
{
  if (possible.empty () && excluded.empty ()) {
    return {partialClique};
  }
  std::set<std::set<Computer>> found;
  std::set<Computer> possibleButNotErased = possible;
  for (const Computer& comp : possible) {
    if (possibleButNotErased.contains (comp)) {
      std::set<Computer> newPartialClique = partialClique;
      newPartialClique.insert (comp);
      std::set<Computer> newPossible;
      std::set<Computer> newExcluded;
      for (const Computer& other : network.edges.at (comp)) {
        if (possibleButNotErased.contains (other)) { newPossible.insert (other); }
        else if (excluded.contains (other)) { newExcluded.insert (other); }
      }
      std::set<std::set<Computer>> recursive = bronKerbosch (network, newPartialClique, newPossible, newExcluded);
      found.insert (recursive.begin (), recursive.end ());
      possibleButNotErased.erase (comp);
      excluded.insert (comp);
    }
  }
  return found;
}



std::string
part2 (const Network& network)
{
  std::set<std::set<Computer>> complete = bronKerbosch (network, {}, network.vertices, {});
  std::set<Computer> largest = *complete.begin ();
  for (const std::set<Computer>& comp : complete) {
    if (comp.size () > largest.size ()) { largest = comp; }
  }
  std::string result;
  bool first = true;
  for (const Computer& comp : largest) {
    if (!first) { result += ","; }
    result += comp;
    first = false;
  }
  return result;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Network prob = readInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";
  return 0;
}

// ab,bu,cn,da,ht,io,kq,nb,pf,us,ww,xq is not the right answer