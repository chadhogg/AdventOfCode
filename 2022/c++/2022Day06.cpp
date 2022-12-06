/// \file 2022Day06.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-06.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <stack>

std::string
getInput ()
{
  std::string line;
  std::getline (std::cin, line);
  return line;
}

std::size_t
whereDuplicate (const std::string& str, std::size_t start, std::size_t len)
{
  for (std::size_t i = 0; i < len; ++i)
  {
    for (std::size_t j = i + 1; j < len; ++j)
    {
      if (str[start + i] == str[start + j]) { return i; }
    }
  }
  return std::string::npos;
}

std::size_t
findEndOfFirstDistinctSequence (const std::string& str, const std::size_t LENGTH)
{
  std::size_t start = 0;
  while (start < str.size () - LENGTH)
  {
    std::size_t jump = whereDuplicate (str, start, LENGTH);
    if (jump == std::string::npos) { return start + LENGTH; }
    else { start += jump + 1; }
  }
  return std::string::npos;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
  std::string input = getInput ();
  std::cout << findEndOfFirstDistinctSequence (input, 4) << "\n";
  std::cout << findEndOfFirstDistinctSequence (input, 14) << "\n";
  return 0;
}
