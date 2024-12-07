/// \file 2024Day07.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-07.

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>

using Number = long;
using Equation = std::pair<Number, std::list<Number>>;
using Problem = std::vector<Equation>;

Problem
readInput ()
{
  Problem prob;
  std::string line;
  while (std::getline (std::cin, line)) {
    Equation eq;
    eq.first = atol (line.substr (0, line.find (':')).c_str ());
    line = line.substr (line.find (':') + 2);
    while (line.find (' ') != std::string::npos) {
      eq.second.push_back (atol (line.substr (0, line.find (' ')).c_str ()));
      line = line.substr (line.find (' ') + 1);
    }
    eq.second.push_back (atol (line.c_str ()));
    prob.push_back (eq);
  }
  return prob;
}

bool
canBeTrue (const Equation& eq)
{
  if (eq.second.size () == 1) {
    return eq.first == eq.second.front ();
  }
  else {
    Equation eq1 (eq);
    Number first = eq1.second.front ();
    eq1.second.pop_front ();
    Number second = eq1.second.front ();
    eq1.second.pop_front ();
    Equation eq2 (eq1);
    eq1.second.push_front (first + second);
    eq2.second.push_front (first * second);
    return canBeTrue (eq1) || canBeTrue (eq2);
  }
}

Number
part1 (const Problem& prob)
{
  Number sum = 0;
  for (const Equation& eq: prob) {
    if (canBeTrue (eq)) {
      sum += eq.first;
    }
  }
  return sum;
}

bool
canBeTrue2 (const Equation& eq)
{
  if (eq.second.size () == 1) {
    return eq.first == eq.second.front ();
  }
  else {
    Equation eq1 (eq);
    Number first = eq1.second.front ();
    eq1.second.pop_front ();
    Number second = eq1.second.front ();
    eq1.second.pop_front ();
    Equation eq2 (eq1);
    Equation eq3 (eq2);
    eq1.second.push_front (first + second);
    eq2.second.push_front (first * second);
    eq3.second.push_front (atol ((std::to_string (first) + std::to_string (second)).c_str ()));
    return canBeTrue2 (eq1) || canBeTrue2 (eq2) || canBeTrue2 (eq3);
  }
}

Number
part2 (const Problem& prob)
{
  Number sum = 0;
  for (const Equation& eq: prob) {
    if (canBeTrue2 (eq)) {
      sum += eq.first;
    }
  }
  return sum;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";
  return 0;
}
