/// \file 2024Day03.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-03.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <regex>

/// @brief Gets the entire input, without processing it in any way.
/// @return A string containing the entire input.
std::string
readInput ()
{
  std::stringstream buffer;
  buffer << std::cin.rdbuf ();
  return buffer.str ();
}

/// @brief Sums the results of the valid mul(x,y) instructions in the input.
/// @param input A string of corrupted memory.
/// @return The sum of the results of the valid mul(x,y) instructions in memory.
long
part1 (const std::string& input)
{
  long total = 0;
  std::regex pattern ("mul\\((\\d+),(\\d+)\\)");
  auto words_begin = std::sregex_iterator (input.begin (), input.end (), pattern);
  auto words_end = std::sregex_iterator();
  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    total += atoi (match[1].str ().c_str ()) * atoi (match[2].str ().c_str ());
  }
  return total;
}

/// @brief Like part1, but ignoring mul(x,y) instructions between "don't()" and "do()" / EOF.
/// @param input A string containing corrupted memory.
/// @return The sum of the results of the multiplications that were to be done.
long
part2 (const std::string& input)
{
  long total = 0;
  std::regex pattern ("(do\\(\\)|don't\\(\\))");
  bool doing = true;
  std::string remainder = input;
  std::smatch result;
  while (std::regex_search (remainder, result, pattern)) {
    if (doing) {
      total += part1 (result.prefix ());
    }
    doing = (result.str () == "do()");
    remainder = result.suffix ();
  }
  if (doing) {
    total += part1 (remainder);
  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  std::string input = readInput ();
  std::cout << part1 (input) << "\n";
  std::cout << part2 (input) << "\n";
}
