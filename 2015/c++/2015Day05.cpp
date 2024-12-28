/// \file 2015Day05.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-05.

#include <iostream>
#include <vector>
#include <string>
#include <set>

std::vector<std::string>
readInput ()
{
  std::vector<std::string> strings;
  std::string word;
  while (std::cin >> word) {
    strings.push_back (word);
  }
  return strings;
}

const std::set<std::string> badWords = {"ab", "cd", "pq", "xy"};

bool
containsIllegalSequence (const std::string& str, const std::set<std::string>& badSubstrs)
{
  for (const std::string& bad : badSubstrs) {
    if (str.find (bad) != std::string::npos) {
      return true;
    }
  }
  return false;
}

bool
hasAtLeastXVowels (const std::string& str, int needed)
{
  int count = 0;
  for (char c : str) {
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
      ++count;
    }
    if (count >= needed) {return true; }
  }
  return false;
}

bool
hasDoubleLetter (const std::string& str)
{
  for (unsigned int index = 0; index < str.size () - 1; ++index) {
    if (str[index] == str[index + 1]) { return true; }
  }
  return false;
}

int
countNiceStrings (const std::vector<std::string>& strings)
{
  int count = 0;
  for (const std::string& str : strings) {
    if (!containsIllegalSequence (str, badWords)
    && hasAtLeastXVowels (str, 3)
    && hasDoubleLetter (str)) {
      ++count;
    }
  }
  return count;
}

bool
containsRepeatedDyad (const std::string& str)
{
  for (unsigned int index1 = 0; index1 < str.size () - 1; ++index1) {
    for (unsigned int index2 = index1 + 2; index2 < str.size () - 1; ++index2) {
      if (str[index1] == str[index2] && str[index1 + 1] == str[index2 + 1]) {
        return true;
      }
    }
  }
  return false;
}

bool
containsRepeatWithSeparator (const std::string& str)
{
  for (unsigned int index = 0; index < str.size () - 2; ++index) {
    if (str[index] == str[index + 2]) { return true; }
  }
  return false;
}

int
countNiceStrings2 (const std::vector<std::string>& strings)
{
  int count = 0;
  for (const std::string& str : strings) {
    if (containsRepeatedDyad (str) && containsRepeatWithSeparator (str)) {
      ++count;
    }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<std::string> strings = readInput ();
  std::cout << countNiceStrings (strings) << "\n";
  std::cout << countNiceStrings2 (strings) << "\n";
  return 0;
}
