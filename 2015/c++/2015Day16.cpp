/// \file 2015Day16.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-16.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <regex>
#include <list>
#include <set>
#include <algorithm>

struct Sue
{
  int number;
  std::map<std::string, int> stuff;
};

std::vector<Sue>
readInput ()
{
  std::vector<Sue> aunts;
  std::basic_regex regex ("^Sue (\\d+): (\\w+): (\\d+), (\\w+): (\\d+), (\\w+): (\\d+)$");
  std::string line;
  while (std::getline (std::cin, line)) {
    std::smatch match;
    if (std::regex_match (line, match, regex)) {
      assert (match.size () == 8);
      Sue aunt;
      aunt.number = atoi (match[1].str ().c_str ());
      aunt.stuff[match[2].str ()] = atoi (match[3].str ().c_str ());
      aunt.stuff[match[4].str ()] = atoi (match[5].str ().c_str ());
      aunt.stuff[match[6].str ()] = atoi (match[7].str ().c_str ());
      aunts.push_back (aunt);
    }
    else {
      std::cerr << "\"" << line << "\" didn't match\n";
    }
  }
  return aunts;
}

bool
isCompatible (const Sue& a, const Sue& b)
{
  for (auto it = a.stuff.cbegin (); it != a.stuff.cend (); ++it) {
    if (b.stuff.contains (it->first) && b.stuff.at (it->first) != it->second) {
      return false;
    }
  }
  for (auto it = b.stuff.cbegin (); it != b.stuff.cend (); ++it) {
    if (a.stuff.contains (it->first) && a.stuff.at (it->first) != it->second) {
      return false;
    }
  }
  return true;
}

void
printCompatibleSues (const std::vector<Sue>& aunts, const Sue& letter)
{
  for (const Sue& aunt : aunts) {
    if (isCompatible (aunt, letter)) {
      std::cout << aunt.number << "\n";
    }
  }
}

bool
isCompatible2 (const Sue& part, const Sue& full)
{
  for (auto it = full.stuff.cbegin (); it != full.stuff.cend (); ++it) {
    if (part.stuff.contains (it->first)) {
      if (it->first == "cats" || it->first == "trees") {
        if (part.stuff.at (it->first) <= it->second) { return false; }
      }
      else if (it->first == "pomeranians" || it->first == "goldfish") {
        if (part.stuff.at (it->first) >= it->second) { return false; }
      }
      else {
        if (part.stuff.at (it->first) != it->second) { return false; }
      }
    }
  }
  return true;
}

void
printCompatibleSues2 (const std::vector<Sue> &aunts, const Sue& letter)
{
  for (const Sue& aunt : aunts) {
    if (isCompatible2 (aunt, letter)) {
      std::cout << aunt.number << "\n";
    }
  }
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<Sue> aunts = readInput ();
  Sue letter;
  letter.stuff["children"] = 3;
  letter.stuff["cats"] = 7;
  letter.stuff["samoyeds"] = 2;
  letter.stuff["pomeranians"] = 3;
  letter.stuff["akitas"] = 0;
  letter.stuff["vizslas"] = 0;
  letter.stuff["goldfish"] = 5;
  letter.stuff["trees"] = 3;
  letter.stuff["cars"] = 2;
  letter.stuff["perfumes"] = 1;

  printCompatibleSues (aunts, letter);
  printCompatibleSues2 (aunts, letter);
  return 0;
}
