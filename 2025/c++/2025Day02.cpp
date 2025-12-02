/// \file 2025Day02.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-02.

#include <iostream>
#include <vector>
#include <string>
#include <regex>


struct Range {
  std::string start;
  std::string end;
};

using Problem = std::vector<Range>;

Problem
getInput () {
  std::vector<Range> problem;
  std::string entireInput;
  std::cin >> entireInput;
  std::regex pattern ("(\\d+)-(\\d+)");
  for (std::sregex_iterator it (entireInput.begin (), entireInput.end (), pattern), end_it; it != end_it; ++it) {
    problem.push_back ({it->str (1), it->str (2)});
  }
  return problem;
}

bool
isRepeatingPattern (const std::string& id, std::size_t length) {
  if (id.length () % length != 0) { return false; }
  std::string first = id.substr (0, length);
  for (std::size_t start = length; start < id.length (); start += length) {
    if (first != id.substr (start, length)) 
    { return false; }
  }
  return true;
}

bool
isDoubled (const std::string& id) {
  return isRepeatingPattern (id, id.length () / 2);
}

bool
containsRepeatingPattern (const std::string& id) {
  for (std::size_t length = 1; length <= id.length () / 2; ++length) {
    if (isRepeatingPattern (id, length)) { return true; }
  }
  return false;
}

unsigned long
sumInvalidIds (const Problem& prob, std::function<bool (const std::string&)> invalidator) {
  unsigned long total = 0;
  for (const Range& range : prob) {
    unsigned long start = std::stoul (range.start);
    unsigned long end = std::stoul (range.end);
    for (unsigned long id = start; id <= end; ++id) {
      if (invalidator (std::to_string (id))) { total += id; }
    }
  }
  return total;
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::cout << sumInvalidIds (prob, isDoubled) << "\n";
  std::cout << sumInvalidIds (prob, containsRepeatingPattern) << "\n";
  return EXIT_SUCCESS;
}

