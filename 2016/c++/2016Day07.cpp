/// \file 2016Day07.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-07.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <set>

std::vector<std::string>
readInput ()
{
  std::vector<std::string> result;
  std::string line;
  while (std::cin >> line) {
    result.push_back (line);
  }
  return result;
}

bool
hasABBA (const std::string& str, bool inBrackets)
{
  bool currentlyInBrackets = false;
  for (std::size_t index = 0; index < str.size () - 3; ++index) {
    if (str[index] == '[') {
      assert (!currentlyInBrackets);
      currentlyInBrackets = true;
    }
    else if (str[index] == ']') {
      assert (currentlyInBrackets);
      currentlyInBrackets = false;
    }
    else if (isalpha (str[index]) && isalpha (str[index + 1]) && str[index] != str[index + 1] && str[index + 1] == str[index + 2] && str[index] == str[index + 3]) {
      if (currentlyInBrackets == inBrackets) {
        return true;
      }
    }
  }
  return false;
}

int
countTLS (const std::vector<std::string>& strs)
{
  int count = 0;
  for (const std::string& str : strs) {
    if (!hasABBA (str, true) && hasABBA (str, false)) {
      ++count;
    }
  }
  return count;
}

bool
supportsSSL (const std::string& str)
{
  std::set<std::string> abasInSupernets;
  std::set<std::string> babsInHypernets;
  bool insideBrackets = false;
  for (std::size_t index = 0; index < str.size () - 2; ++index) {
    if (str[index] == '[') {
      assert (!insideBrackets);
      insideBrackets = true;
    }
    else if (str[index] == ']') {
      assert (insideBrackets);
      insideBrackets = false;
    }
    else if (isalpha (str[index + 1]) && str[index] != str[index + 1] && str[index] == str[index + 2]) {
      if (insideBrackets) { babsInHypernets.insert (str.substr (index, 3)); }
      else { abasInSupernets.insert (str.substr (index, 3)); }
    }
  }
  for (const std::string& aba : abasInSupernets) {
    std::string bab = "   ";
    bab[0] = bab[2] = aba[1];
    bab[1] = aba[0];
    if (babsInHypernets.contains (bab)) { return true; } 
  }
  return false;
}

int
countSSL (const std::vector<std::string>& strs)
{
  int count = 0;
  for (const std::string& str : strs) {
    if (supportsSSL (str)) { ++count; }
  }
  return count;
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<std::string> input = readInput ();
  std::cout << countTLS (input) << "\n";
  std::cout << countSSL (input) << "\n";
  return 0;
}

