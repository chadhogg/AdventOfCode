/// \file 2015Day11.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-11.

#include <iostream>
#include <string>

std::string
readInput ()
{
  std::string line;
  std::cin >> line;
  return line;
}

std::string
increment (const std::string& original, int place = 0)
{
  std::size_t index = original.size () - place - 1;
  if (original.at (index) == 'z') {
    return increment (original.substr (0, index) + 'a' + original.substr (index + 1), place + 1);
  }
  else {
    return original.substr (0, index) + char ((original.at (index) + 1)) + original.substr (index + 1);
  }
}

bool
containsIncreasingStraight (const std::string& str)
{
  for (std::size_t index = 0; index < str.size () - 2; ++index) {
    if (str.at (index) == str.at (index + 1) - 1 && str.at (index + 1) == str.at (index + 2) - 1) {
      return true;
    }
  }
  return false;
}

bool
noBadLetters (const std::string& str)
{
  return str.find ('i') == std::string::npos && str.find ('o') == std::string::npos && str.find ('l') == std::string::npos;
}

bool
hasNonOverlappingPairs (const std::string& str)
{
  for (std::size_t index1 = 0; index1 < str.size () - 3; ++index1) {
    if (str.at (index1) == str.at (index1 + 1)) {
      for (std::size_t index2 = index1 + 2; index2 < str.size () - 1; ++index2) {
        if (str.at (index2) == str.at (index2 + 1)) {
          return true;
        }
      }
    }
  }
  return false;
}

std::string
bruteForceNextPassword (const std::string& old)
{
  std::string current = increment (old);
  while (!containsIncreasingStraight (current) || !noBadLetters (current) || !hasNonOverlappingPairs (current)) {
    current = increment (current);
  }
  return current;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::string input = readInput ();

  // std::cout << increment ("abcde") << "\n";
  // std::cout << increment ("abcdz") << "\n";
  // std::cout << increment ("abczz") << "\n";

  // std::cout << bruteForceNextPassword ("abcdefgh") << "\n";
  // std::cout << bruteForceNextPassword ("ghijklmn") << "\n";

  std::string second = bruteForceNextPassword (input);
  std::cout << second << "\n";
  std::string third = bruteForceNextPassword (second);
  std::cout << third << "\n";

  return 0;
}
