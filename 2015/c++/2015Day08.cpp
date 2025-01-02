/// \file 2015Day08.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2015-12-08.

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <numeric>

std::vector<std::string>
readInput ()
{
  std::vector<std::string> strings;
  std::string line;
  while (std::getline (std::cin, line)) {
    strings.push_back (line);
  }
  return strings;
}

std::string
unencode (const std::string& str)
{
  std::string result;
  for (unsigned int index = 1; index < str.size () - 1; ) {
    if (str[index] == '\\') {
      if (str[index + 1] == '\\') {
        result += '\\';
        index += 2;
      }
      else if (str[index + 1] == '\"') {
        result += '\"';
        index += 2;
      }
      else if (str[index + 1] == 'x') {
        result += (char)std::stoi (str.substr (index + 2, 2), nullptr, 16);
        index += 4;
      }
    }
    else {
      result += str[index];
      ++index;
    }
  }
  return result;
}

std::vector<std::string>
unencodeAll (const std::vector<std::string>& encoded)
{
  return encoded | std::ranges::views::transform (unencode) | std::ranges::to<std::vector> ();
  /*
  std::vector<std::string> result;
  result.reserve (encoded.size ());
  for (const std::string& str : encoded) {
    result.push_back (unencode (str));
  }
  return result;
  */
}

std::size_t
totalLengths (const std::vector<std::string>& strs)
{
  return std::ranges::fold_left (strs | std::ranges::views::transform (&std::string::size), 0ZU, std::plus{});
  //return std::reduce (strs.cbegin (), strs.cend ());
  /*
  std::size_t total = 0;
  for (const std::string& str : strs) {
    total += str.size ();
  }
  return total;
  */
}

std::size_t
part1 (const std::vector<std::string>& strings)
{
  return totalLengths (strings) - totalLengths (unencodeAll (strings));
}

std::string
encode (const std::string& str)
{
  std::string result;
  result += "\"";
  for (unsigned int index = 0; index < str.size (); ++index) {
    if (str[index] == '\\') {
      result += "\\\\";
    }
    else if (str[index] == '\"') {
      result += "\\\"";
    }
    else {
      result += str[index];
    }
  }
  return result + "\"";
}

std::vector<std::string>
encodeAll (const std::vector<std::string>& strs)
{
  return strs | std::ranges::views::transform (encode) | std::ranges::to<std::vector> ();
  /*
  std::vector<std::string> result;
  result.reserve (strs.size ());
  for (const std::string& str : strs) {
    result.push_back (encode (str));
  }
  return result;
  */
}

std::size_t
part2 (const std::vector<std::string>& strings)
{
  return totalLengths (encodeAll (strings)) - totalLengths (strings);
}

/// \brief Runs the program.
/// \return Always 0.
int main ()
{
  std::vector<std::string> strings = readInput ();
  std::cout << part1 (strings) << "\n";
  std::cout << part2 (strings) << "\n";
  return 0;
}
