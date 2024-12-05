/// \file 2024Day05.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-05.
/// You say maps and topological sorts and other fancy fast things, I say brute force goes brrrr.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>

using PageNum = int;

using OrderRule = std::pair<PageNum, PageNum>;
using Update = std::vector<PageNum>;
using Problem = std::pair<std::vector<OrderRule>, std::vector<Update>>;

Problem
readInput ()
{
  Problem prob;
  bool stillDoingRules = true;
  std::string line;
  while (std::getline (std::cin, line)) {
    auto pipePlace = line.find('|');
    if (line == "") {
      assert (stillDoingRules);
      stillDoingRules = false;
    }
    else if (pipePlace != std::string::npos) {
      assert (stillDoingRules);
      prob.first.push_back ({atoi (line.substr (0, pipePlace).c_str ()), atoi (line.substr (pipePlace + 1).c_str ())});
    }
    else {
      assert (!stillDoingRules);
      Update update;
      auto commaPlace = line.find(',');
      auto previous = commaPlace - commaPlace;
      assert (commaPlace != std::string::npos);
      while (commaPlace != std::string::npos) {
        update.push_back (atoi (line.substr (previous, commaPlace).c_str ()));
        previous = commaPlace + 1;
        commaPlace = line.find(',', previous);
      }
      update.push_back (atoi (line.substr (previous).c_str ()));
      prob.second.push_back (update);
    }
  }

  return prob;
}

bool
isValid (const std::vector<OrderRule>& rules, const Update& update)
{
  for (unsigned int i = 0; i < update.size (); ++i) {
    for (unsigned int j = i + 1; j < update.size (); ++j) {
      for (unsigned int k = 0; k < rules.size (); ++k) {
        if (rules.at (k).first == update.at (j) && rules.at (k).second == update.at (i)) {
          return false;
        }
      }
    }
  }
  return true;
}

long
part1 (const Problem& prob) {
  long sum = 0;
  for (const Update& update: prob.second) {
    if (isValid (prob.first, update)) {
      sum += update.at (update.size () / 2);
    }
  }
  return sum;
}

Update
makeValid (const std::vector<OrderRule>& rules, const Update& original)
{
  std::vector<PageNum> notYetUsed = original;
  Update revised;
  while (!notYetUsed.empty ()) {
    for (unsigned int i = 0; i < notYetUsed.size (); ++i) {
      bool good = true;
      for (unsigned int j = 0; j < notYetUsed.size () && good; ++j) {
        for (unsigned int k = 0; k < rules.size () && good; ++k) {
          if (rules.at (k).first == notYetUsed.at (j) && rules.at (k).second == notYetUsed.at (i)) {
            good = false;
          }
        }
      }
      if (good) {
        revised.push_back (notYetUsed.at (i));
        notYetUsed.erase (notYetUsed.begin () + i);
        break;
      }
    }
  }
  return revised;
}

long
part2 (const Problem& prob) {
  long sum = 0;
  for (const Update& update: prob.second) {
    if (!isValid (prob.first, update)) {
      Update revised = makeValid (prob.first, update);
      sum += revised.at (revised.size () / 2);
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
