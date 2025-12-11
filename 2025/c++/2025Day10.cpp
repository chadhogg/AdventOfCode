/// \file 2025Day10.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-10.

#include <cstddef>
#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <sstream>
#include <cstdint>
#include <z3++.h>

using Lights = std::vector<bool>;
using Button = std::set<int>;
using Joltages = std::vector<int>;

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
  out << "[";
  for (T val : vec) {
    out << val << " ";
  }
  out << "]";
  return out;
}

struct Machine
{
  Lights desiredIndicators;
  std::vector<Button> buttons;
  Joltages joltageRequirements;

  std::string toString () const {
    std::stringstream str;
    str << "[";
    for (bool b : desiredIndicators) {
      if (b) { str << '#'; }
      else { str << '.'; }
     }
     str << "]";
    for (const Button& button : buttons) {
      str << " (";
      std::size_t count = 0;
      for (int x : button) {
        str << x;
        ++count;
        if (count < button.size ()) { str << ","; }
      }
      str << ")";
    }
    str << " {";
    std::size_t count = 0;
    for (int x : joltageRequirements) {
      str << x;
      ++count;
      if (count < joltageRequirements.size ()) { str << ","; }
    }
    str << "}";
    return str.str ();
  }
};



using Problem = std::vector<Machine>;

Problem
getInput ()
{
  Problem prob;
  std::string line;
  std::regex indicatorPattern ("\\[([.#]*)\\]");
  std::regex buttonPattern ("\\(([\\d,]*)\\)");
  std::regex joltagePattern ("\\{(.*)\\}");
  std::regex numberPattern ("(\\d+)");
  while (std::getline (std::cin, line)) {
    std::smatch results;
    bool success;
    Machine machine;
    success = std::regex_search (line, results, indicatorPattern);
    assert (success);
    for (char c : std::string (results[1])) {
      if (c == '#') { machine.desiredIndicators.push_back (true); }
      else { machine.desiredIndicators.push_back (false); }
    }
    for (auto it = std::sregex_iterator (line.begin (), line.end (), buttonPattern); it != std::sregex_iterator (); ++it) {
      std::string buttonString = it->str ();
      Button button;
      for (auto it2 = std::sregex_iterator (buttonString.begin (), buttonString.end (), numberPattern); it2 != std::sregex_iterator (); ++it2) {
        std::string numString = it2->str ();
        int num = atoi (numString.c_str ());
        assert (button.count (num) == 0);
        button.insert (num);
      }
      machine.buttons.push_back (button);
    }
    success = std::regex_search (line, results, joltagePattern);
    assert (success);
    std::string joltageStr = results[1];
    for (auto it2 = std::sregex_iterator (joltageStr.begin (), joltageStr.end (), numberPattern); it2 != std::sregex_iterator (); ++it2) {
      std::string numString = it2->str ();
      int num = atoi (numString.c_str ());
      machine.joltageRequirements.push_back (num);
    }
    assert (line == machine.toString ());
    prob.push_back (machine);
  }
  return prob;
}

Lights
applyPresses (const Machine& machine, const std::set<std::size_t>& presses)
{
  Lights lights (machine.desiredIndicators.size (), false);
  for (std::size_t buttonNo : presses) {
    const Button& button = machine.buttons.at (buttonNo);
    for (std::size_t lightNo : button) {
      lights[lightNo].flip();
    }
  }
  return lights;
}

bool
myLess (const std::set<size_t>& s1, const std::set<size_t>& s2)
{
  if (s1.size () < s2.size ()) { return true; }
  else if (s1.size () > s2.size ()) { return false; }
  else { return s1 < s2; }
}

std::set<std::size_t>
configureIndicatorLights (const Machine& machine)
{
  std::set<std::set<std::size_t>, decltype(&myLess)> frontier (myLess);
  std::set<std::set<std::size_t>> explored;
  frontier.insert (std::set<std::size_t> ({}));
  while (!frontier.empty ()) {
    // I am making an assumption here that smaller sets are always "less than" larger sets.
    // Turned out not to be true, so I had to write my own.
    std::set<std::size_t> current = *frontier.begin ();
    frontier.erase (frontier.begin ());
    explored.insert (current);
    if (applyPresses (machine, current) == machine.desiredIndicators) {
      return current;
    }
    for (std::size_t buttonNo = 0; buttonNo < machine.buttons.size (); ++buttonNo) {
      if (current.count (buttonNo) == 0) {
        std::set<std::size_t> next = current;
        next.insert (buttonNo);
        if (explored.count (next) == 0) {
          frontier.insert (next);
        }
      }
    }
  }
  assert (false);
  return {};
}

std::size_t
part1 (const Problem& prob)
{
  std::size_t total = 0;
  for (const Machine& machine : prob) {
    total += configureIndicatorLights (machine).size ();
  }
  return total;
}

using NumPresses = std::vector<uint16_t>;

Joltages
applyPresses (const Machine& machine, const NumPresses& presses)
{
  Joltages joltages (machine.joltageRequirements.size (), 0);
  for (std::size_t buttonNo = 0; buttonNo < presses.size (); ++buttonNo) {
    const Button& button = machine.buttons.at (buttonNo);
    for (std::size_t counterNo : button) {
      joltages[counterNo] += presses[buttonNo];
    }
  }
  return joltages;
}

bool
myLess2 (const NumPresses& s1, const NumPresses& s2)
{
  std::size_t total1 = 0, total2 = 0;
  for (std::size_t x : s1) { total1 += x; }
  for (std::size_t x : s2) { total2 += x; }
  if (total1 < total2) { return true; }
  else if (total1 > total2) { return false; }
  else { return s1 < s2; }
}

NumPresses
configureJoltage (const Machine& machine)
{
  std::set<NumPresses, decltype(&myLess2)> frontier (myLess2);
  std::set<NumPresses> explored;
  frontier.insert (NumPresses (machine.buttons.size (), 0U));
  while (!frontier.empty ()) {
    NumPresses current = *frontier.begin ();
    frontier.erase (frontier.begin ());
    explored.insert (current);
    Joltages result = applyPresses (machine, current);
    if (result == machine.joltageRequirements) { return current; }
    for (std::size_t index = 0; index < result.size (); ++index) {
      if (result[index] > machine.joltageRequirements.at (index)) { continue; }
    }
    for (std::size_t buttonNo = 0; buttonNo < machine.buttons.size (); ++buttonNo) {
      NumPresses next = current;
      ++(next[buttonNo]);
      if (explored.count (next) == 0) {
        frontier.insert (next);
      }
    }
  }
  assert (false);
  return {};
}

// Tooooo slow, and memory-intensive.  Need to add a good heuristic and do A*.
std::size_t
part2 (const Problem& prob)
{
  std::size_t total = 0;
  std::size_t finished = 0;
  for (const Machine& machine : prob) {
    NumPresses result = configureJoltage (machine);
    std::cout << "Solved machine " << finished << " with: ";
    for (std::size_t x : result) { std::cout << x << " "; }
    std::cout << "\n";
    for (std::size_t x : result) { total += x; }
    ++finished;
  }
  return total;
}

std::size_t
lamePart2 (const Problem& prob)
{
  std::size_t total = 0;
  for (const Machine& machine: prob) {
    z3::context context;
    z3::optimize optimizer (context);
    std::vector<z3::expr> buttonVars;
    std::string prefix = "b";
    // Create a variable per button.
    for (std::size_t index = 0; index < machine.buttons.size (); ++index) {
      std::string name = prefix + std::to_string (index);
      buttonVars.push_back (context.int_const (name.c_str ()));
    }
    // Create an extra constant just because I think it will make building expressions easier.
    z3::expr zero = context.int_const ("zero");
    optimizer.add (zero == 0);
    // Create a constraint per variable saying it may not be negative.
    for (const auto& var : buttonVars) {
      optimizer.add (var >= 0);
    }
    // Create a constraint for each counter, that it ends up at exactly the rigth nnumber
    for (std::size_t counterIndex = 0; counterIndex < machine.joltageRequirements.size (); ++counterIndex) {
      z3::expr partialSum = zero;
      for (std::size_t buttonIndex = 0; buttonIndex < machine.buttons.size (); ++buttonIndex) {
        if (machine.buttons.at (buttonIndex).count (counterIndex) != 0) {
          partialSum = partialSum + buttonVars[buttonIndex];
        }
      }
      optimizer.add (partialSum == machine.joltageRequirements.at (counterIndex));
    }
    // The expression that I want to minimize:
    z3::expr totalButtons = zero;
    for (const auto& var : buttonVars) {
      totalButtons = totalButtons + var;
    }
    // Optimize it
    optimizer.minimize (totalButtons);
    if (optimizer.check () == z3::sat) {
      z3::model results = optimizer.get_model();
      for (const auto& var : buttonVars) {
        total += results.eval (var).get_numeral_int();
      }
    }
    else {
      assert (false);
    }
  }
  return total;
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << lamePart2 (prob) << "\n";
  return EXIT_SUCCESS;
}

