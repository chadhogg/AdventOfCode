/// \file 2022Day25.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2022-12-25.


#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using Problem = std::vector<std::string>;

Problem
getInput ()
{
  Problem prob;
  std::string line;
  std::getline (std::cin, line);
  while (std::cin)
  {
    prob.push_back (line);
    std::getline (std::cin, line);
  }
  return prob;
}

long
snafuToDecimal (std::string snafu)
{
  long result = 0;
  long power = 1;
  while (!snafu.empty ())
  {
    char digit = snafu.back ();
    snafu.pop_back ();
    assert (digit == '2' || digit == '1' || digit == '0' || digit == '-' || digit == '=');
    if (digit == '2') { result += 2 * power; }
    else if(digit == '1') { result += power; }
    else if (digit == '-') { result -= power; }
    else if (digit == '=') { result -= 2 * power; }
    power *= 5;
  }
  return result;
}

std::string
decimalToSnafu (long number)
{
  std::string result;
  while (number > 0)
  {
    if (number % 5 == 0) { result = "0" + result; number = number / 5; }
    else if (number % 5 == 1) { result = "1" + result; number = number / 5; }
    else if (number % 5 == 2) { result = "2" + result; number = number / 5; }
    else if (number % 5 == 3) { result = "=" + result; number = number / 5 + 1; }
    else if (number % 5 == 4) { result = "-" + result; number = number / 5 + 1; }
  }
  return result;
}

std::string
solve (const Problem& prob)
{
  long total = 0;
  for (const std::string& snafu : prob)
  {
    total += snafuToDecimal (snafu);
  }
  return decimalToSnafu (total);
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  //std::cout << "1=-0-2" << " -> " << snafuToDecimal ("1=-0-2") << "\n";
  //std::cout << 10 << " -> " << decimalToSnafu (10) << "\n";
  //std::cout << 314159265 << " -> " << decimalToSnafu (314159265) << "\n";
  std::cout << solve (prob) << "\n";
  return 0;
}

// My answers:
// Part 1: 
// Part 2: 
