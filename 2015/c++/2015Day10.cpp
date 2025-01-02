/// \file 2015Day10.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-10.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <list>
#include <set>
#include <sstream>

std::string
readInput ()
{
  std::string line;
  std::cin >> line;
  return line;
}

std::string
seeSay (const std::string& start)
{
  std::stringstream finish;
  for (std::size_t index = 0; index < start.size (); ) {
    char digit = start.at (index);
    int count = 0;
    while (index < start.size () && digit == start.at (index)) {
      ++count;
      ++index;
    }
    finish << count << digit;
  }
  return finish.str ();
}

std::string
repeatSeeSay (const std::string& str, int times) {
  std::string result = str;
  for (int i = 0; i < times; ++i) {
    result = seeSay (result);
  }
  return result;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string input = readInput ();
  std::cout << repeatSeeSay (input, 40).size () << "\n";
  std::cout << repeatSeeSay (input, 50).size () << "\n";


  // std::cout << seeSay ("1") << "\n";
  // std::cout << seeSay ("11") << "\n";
  // std::cout << seeSay ("21") << "\n";
  // std::cout << seeSay ("1211") << "\n";
  // std::cout << seeSay ("111221") << "\n";

  return 0;
}

// 566334 is too high