/// \file 2015Day13.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-13.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <regex>
#include <list>
#include <set>

using Person = std::string;
using Happiness = int;
using Preferences = std::map<Person, std::map<Person, Happiness>>;

Preferences
readInput ()
{
  std::basic_regex reg ("^(\\w+) would (gain|lose) (-?\\d+) happiness units by sitting next to (\\w+).$");
  Preferences prefs;
  std::string line;
  while (std::getline (std::cin, line)) {
    std::smatch match;
    if (std::regex_match (line, match, reg)) {
      assert (match.size () == 5);
      Person person1 = match[1].str ();
      std::string direction = match[2].str ();
      Happiness happiness = atoi (match[3].str ().c_str ());
      if (direction == "lose") { happiness *= -1; }
      Person person2 = match[4].str ();
      if (!prefs.contains (person1)) { prefs[person1] = {}; }
      assert (!prefs[person1].contains (person2));
      prefs[person1][person2] = happiness;
    }
    else {
      std::cout << line << " did not match!\n";
    }
  }
  return prefs;
}

struct PartialSolution
{
  std::vector<Person> order;
  Happiness total;
};

PartialSolution
bruteForceOptimizeSeating (const Preferences& prefs)
{

  std::set<Person> people;
  for (auto it = prefs.cbegin (); it != prefs.cend (); ++it) {
    people.insert (it->first);
  }
  std::list<PartialSolution> frontier;
  for (const Person& person : people) {
    frontier.push_back ({{person}, 0});
  }
  PartialSolution best {{}, INT32_MIN};
  while (!frontier.empty ()) {
    PartialSolution current = frontier.front ();
    frontier.pop_front ();
    if (current.order.size () == prefs.size ()) {
      current.total += prefs.at (current.order.front ()).at (current.order.back ());
      current.total += prefs.at (current.order.back ()).at (current.order.front ());
      if (current.total > best.total) { best = current; }
    }
    else {
      std::set<Person> alreadySeated (current.order.begin (), current.order.end ());
      for (const Person& person : people) {
        if (!alreadySeated.contains (person)) {
          PartialSolution next = current;
          next.total += prefs.at (next.order.back ()).at (person);
          next.total += prefs.at (person).at (next.order.back ());
          next.order.push_back (person);
          frontier.push_back (next);
        }
      }
    }
  }
  return best;
}

void
upgradePreferences (Preferences& prefs)
{
  std::set<Person> people;
  for (auto it = prefs.begin (); it != prefs.end (); ++it) {
    people.insert (it->first);
    it->second["Me"] = 0;
  }
  prefs["Me"] = {};
  for (const Person& person : people) {
    prefs["Me"][person] = 0;
  }
}
/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  Preferences prefs = readInput ();
  std::cout << bruteForceOptimizeSeating (prefs).total << "\n";
  upgradePreferences (prefs);
  std::cout << bruteForceOptimizeSeating (prefs).total << "\n";  
  return 0;
}

