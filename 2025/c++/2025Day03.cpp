/// \file 2025Day03.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2025-12-03.

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using Joltage = unsigned long;
using Bank = std::vector<Joltage>;
using Problem = std::vector<Bank>;

Problem
getInput ()
{
  Problem prob;
  std::string line;
  while (std::cin >> line) {
    Bank bank;
    for (char c : line) {
      bank.push_back (c);
    }
    prob.push_back (bank);
  }
  return prob;
}

Joltage
maximumJoltage (const Bank& bank, unsigned int count)
{
  Joltage best = 0;
  std::size_t startIndex = 0;
  for (std::size_t digit = 0; digit < count; ++digit) {
    Joltage largestDigit = 0;
    std::size_t largestIndex;
    for (std::size_t index = startIndex; index + (count - digit - 1) < bank.size () && largestDigit < 9; ++index) {
      if (bank[index] - '0' > largestDigit) {
        largestDigit = bank[index] - '0';
        largestIndex = index;
      }
    }
    best += std::pow (10, (count - digit - 1)) * largestDigit;
    startIndex = largestIndex + 1;
}
  return best;
}

Joltage
sumMaximums (const Problem& prob, int count) {
  Joltage sum = 0;
  for (const Bank& bank : prob) {
    sum += maximumJoltage (bank, count);
  }
  return sum;
}

int
main (int argc, char* argv[])
{
  Problem prob = getInput ();
  std::cout << sumMaximums (prob, 2) << "\n";
  std::cout << sumMaximums (prob, 12) << "\n";
  return EXIT_SUCCESS;
}

