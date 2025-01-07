/// \file 2015Day19.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-19.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <map>
#include <regex>
#include <list>

using Productions = std::map<std::string, std::set<std::string>>;

struct Problem
{
  Productions rules;
  std::string goal;
};

Problem
readInput ()
{
  Problem prob;
  std::string line;
  std::basic_regex regex ("^(\\w+) => (\\w+)$");
  while (std::getline (std::cin, line) && line != "") {
    std::smatch match;
    if (std::regex_match (line, match, regex)) {
      assert (match.size () == 3);
      if (!prob.rules.contains (match[1].str ())) {
        prob.rules[match[1].str ()] = {};
      }
      prob.rules[match[1].str ()].insert (match[2].str ());
    }
  }
  std::cin >> prob.goal;
  return prob;
}

// Assumption that appears to be true for my input: no nonterminal is a prefix of another.
std::size_t
countSingleReplacements (const Problem& prob)
{
  std::set<std::string> results;
  for (std::size_t index = 0; index < prob.goal.size (); ++index) {
    std::string start = prob.goal.substr (index, 1);
    if (!prob.rules.contains (start)) {
      start = prob.goal.substr (index, 2);
    }
    if (prob.rules.contains (start)) {
      std::string before = prob.goal.substr (0, index);
      std::string after = prob.goal.substr (index + start.size ());
      for (const std::string& replacement : prob.rules.at (start)) {
        results.insert (before + replacement + after);
      }
    }
  }
  return results.size ();
}

struct PartialSolution
{
  std::size_t numAlreadyMatched;
  std::string stillToGo;
  int stepsTaken;

  auto operator<=> (const PartialSolution& p) const = default;

  // "less-than" here just means it should come first in an ordering.
  // The goal is for partial solutions that seem closer to a complete solution to be ordered first.
  bool operator< (const PartialSolution& p) const
  {
    // We always prefer partials that have more symbols matched over those with fewer.
    if (numAlreadyMatched > p.numAlreadyMatched) {
      return true;
    }
    else if (numAlreadyMatched == p.numAlreadyMatched) {
      // We prefer partials that have taken fewer steps over those that have taken many.
      if (stepsTaken < p.stepsTaken) {
        return true;
      }
      else if (stepsTaken == p.stepsTaken) {
        // We prefer partials with fewer symbols to those with more.
        if (stillToGo.size () < p.stillToGo.size ()) {
          return true;
        }
        else if (stillToGo.size () == p.stillToGo.size ()) {
          return stillToGo < p.stillToGo;
        }
        else {
          return false;
        }
      }
      else {
        return false;
      }
    }
    else {
      return false;
    }
  }

};

std::ostream&
operator<< (std::ostream& out, const PartialSolution& p)
{
  out << p.numAlreadyMatched << "\t" << p.stillToGo << "\t" << p.stepsTaken;
  return out;  
}

// The objective here is to determine whether or not it is possible
//   for a string starting with "start" to ever become a string starting
//   with "end".
// I wrote this through manual analysis of the input, not that it helped.
bool
canReach (const Problem& prob, const std::string& start, const std::string& end)
{
  if (start == "Al") {
    return std::set<std::string> {"Al", "Th"}.contains (end);
  }
  else if (start == "B") {
    return std::set<std::string> {"B", "Ti"}.contains (end);
  }
  else if (start == "Ca") {
    return std::set<std::string> {"Ca", "P", "Si"}.contains (end);
  }
  else if (start == "F") {
    return std::set<std::string> {"F", "Ca", "P", "Si"}.contains (end);
  }
  else if (start == "H") {
    return std::set<std::string> {"H", "C", "N", "O"}.contains (end);
  }
  else if (start == "Mg") {
    return std::set<std::string> {"Mg", "B", "Ti"}.contains (end);
  }
  else if (start == "N") {
    return std::set<std::string> {"N", "C", "H", "O"}.contains (end);
  }
  else if (start == "O") {
    return std::set<std::string> {"O", "C", "H", "N"}.contains (end);
  }
  else if (start == "P") {
    return std::set<std::string> {"P", "Ca", "Si"}.contains (end);
  }
  else if (start == "Si") {
    return std::set<std::string> {"Si", "Ca"}.contains (end);
  }
  else if (start == "Th") {
    return std::set<std::string> {"Th"}.contains (end);
  }
  else if (start == "Ti") {
    return std::set<std::string> {"Ti", "B"}.contains (end);
  }
  else if (start == "e") {
    return std::set<std::string> {"e", "H", "N", "O", "C"}.contains (end);
  }
  else {
    return start == end;
  }
}

// Notes:
// - The grammar in my input is left-recursive, so naive recursive descent parsing won't work.
// - There are ways to remove left-recursion, but they would make it very difficult to calculate the number of "steps" taken in the original grammar.
// - The problem implies that the grammar is ambiguous, which rules out shift-reduce parsers.
// - I thought I could do a best-first search of possible recursive descents with a smart enough heuristic, but this did not work.


std::string
getFirstToken (const std::string& str)
{
  // No idea why we have this one weird lower case token.
  if (str.at (0) == 'e') { return "e"; }
  assert (isupper (str.at (0)));
  if (str.size () == 1 || isupper (str.at (1))) { return str.substr (0, 1); }
  else { return str.substr (0, 2); }
}


int
minSteps (const Problem& prob)
{
  std::set<PartialSolution> frontier;
  std::set<PartialSolution> seen;
  frontier.insert ({0, "e", 0});
  int minSteps = INT32_MAX;
  PartialSolution longestMatch {0, {}, 0};
  std::size_t pruned = 0;
  while (!frontier.empty ()) {
    PartialSolution current = *frontier.begin ();
    frontier.erase (current);
    if (seen.contains (current)) { continue; }
    seen.insert (current);
    std::cout << "Considering " << current << "\n";
    if (current.numAlreadyMatched > longestMatch.numAlreadyMatched) {
      longestMatch = current;
    }

    // Any process that is already longer than the fastest solution can't be the fastest solution.
    if (current.stepsTaken > minSteps) { continue; }
    // Any process that has achieved the goal can stop.
    if (current.numAlreadyMatched == prob.goal.size ()) {
      assert (current.stillToGo == "");
      minSteps = std::min (minSteps, current.stepsTaken);
      continue;
    }
    // Additional assumption that appears to be true for my input:
    //   No substitution ever shortens the string.
    // Any process that has made the total string longer than the goal cannot be a solution.
    if (current.numAlreadyMatched + current.stillToGo.size () > prob.goal.size ()) { continue; }

    std::string tokenToParse = getFirstToken (current.stillToGo);
    // Treat the front symbol as a terminal if possible.
    if (tokenToParse == prob.goal.substr (current.numAlreadyMatched, tokenToParse.size ())) {
      std::cout << "Creating " << PartialSolution {current.numAlreadyMatched + tokenToParse.size (), current.stillToGo.substr (tokenToParse.size ()), current.stepsTaken} << "\n";
      frontier.insert ({current.numAlreadyMatched + tokenToParse.size (), current.stillToGo.substr (tokenToParse.size ()), current.stepsTaken});
    }
    // Treat the front symbol as a non-terminal if possible.
    if (prob.rules.contains (tokenToParse)) {
      std::string rest = current.stillToGo.substr (tokenToParse.size ());
      std::string goalToken = getFirstToken (prob.goal.substr (current.numAlreadyMatched));
      for (const std::string& subst : prob.rules.at (tokenToParse)) {
        if (canReach (prob, getFirstToken (subst), goalToken)) {
          std::cout << "Creating " << PartialSolution {current.numAlreadyMatched, subst + rest, current.stepsTaken + 1} << "\n";
          frontier.insert ({current.numAlreadyMatched, subst + rest, current.stepsTaken + 1});
        }
        else {
          std::cout << "Not trying to use " << tokenToParse << " => " << subst << " for goal " << prob.goal.substr (current.numAlreadyMatched) << "\n";
          ++pruned;
        }
      }
    }
  }
  std::cout << "Pruned " << pruned << " nodes\n";
  return minSteps;
}

// Let's just always run the longest matching backward rule and see how that goes.
// Can always do a backtracking search if necessary, though not sure it will be any better than forwards.
int
greedyBackwardsAttempt (const Problem& prob, const std::string& start)
{
  std::map<std::string, std::set<std::string>> backwardsRules;
  for (auto it = prob.rules.cbegin (); it != prob.rules.cend (); ++it) {
    std::string alpha = it->first;
    for (auto it2 = it->second.cbegin (); it2 != it->second.cend (); ++it2) {
      std::string beta = *it2;
      if (!backwardsRules.contains (beta)) { backwardsRules[beta] = {}; }
      backwardsRules[beta].insert (alpha);
    }
  }
  int steps = 0;
  std::string result = prob.goal;
  while (result != start) {
    std::string longestAlpha = "", longestBeta = "";
    for (auto it = backwardsRules.cbegin (); it != backwardsRules.cend (); ++it) {
      if (result.find (it->first) != std::string::npos) {
        if (it->first.size () > longestAlpha.size ()) {
          longestAlpha = it->first;
          longestBeta = *it->second.cbegin ();
        }
      }
    }
    assert (longestAlpha != "");
    result = result.substr (0, result.find (longestAlpha)) + longestBeta + result.substr (result.find (longestAlpha) + longestAlpha.size ());
    ++steps;
  }
  return steps;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Problem prob = readInput ();
  std::cout << countSingleReplacements (prob) << "\n";
//  std::cout << minSteps (prob) << "\n";
  std::cout << greedyBackwardsAttempt (prob, "e") << "\n";
  return 0;
}

// Analysis:
// First terminal is C.  Could come from:
// - O => CRnMgAr
// - 0 => CRnFYFAr
// - N => CRnFAr
// - H => CRnMgYFAr
// - H => CRnFYMgAr
// - H => CRnFYFYFAr
// - H => CRnAlAr
// So to get to C, we must get to O, N, or H
// This is useless analysis, because all of the initial productions can accomplish this.
