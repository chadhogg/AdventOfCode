/// \file 2016Day06.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-06.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>

std::vector<std::string>
readInput ()
{
  std::vector<std::string> result;
  std::string line;
  while (std::cin >> line) {
    assert (line.size () == 8);
    for (char c : line) { assert (isalpha (c)); }
    result.push_back (line);
  }
  return result;
}

std::string
decode (const std::vector<std::string>& input, bool max = true)
{
  std::string message;
  for (std::size_t index = 0; index < input.at (0).size (); ++index) {
    std::map<char, int> counts;
    for (std::size_t index2 = 0; index2 < input.size (); ++index2) {
      char c = input.at (index2).at (index);
      if (!counts.contains (c)) { counts[c] = 0; }
      ++counts[c];
    }
    char most = counts.begin ()->first;
    for (auto it = counts.begin (); it != counts.end (); ++it) {
      if (max) {
        if (it->second > counts[most]) { most = it->first; }
      }
      else {
        if (it->second < counts[most]) { most = it->first; }
      }
    }
    message.push_back (most);
  }
  return message;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<std::string> input = readInput ();
  std::cout << decode (input, true) << "\n";
  std::cout << decode (input, false) << "\n";
  return 0;
}

