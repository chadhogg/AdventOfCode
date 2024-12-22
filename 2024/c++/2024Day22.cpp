/// \file 2024Day22.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-22.


#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <set>
#include <cmath>
#include <utility>
#include <map>
#include <optional>
#include <chrono>
#include <thread>
#include <queue>

using Number = long long;
using Problem = std::vector<Number>;

Problem
readInput ()
{
  Problem prob;
  long long num;
  std::cin >> num;
  while (std::cin) {
    prob.push_back (num);
    std::cin >> num;
  }
  return prob;
}

inline Number
mix (Number a, Number b)
{
  return a ^ b;
}

inline Number
prune (Number a, Number b = 16777216)
{
  return a % b;
}

Number
evolve (Number current)
{
  current = prune (mix (current, current * 64));
  current = prune (mix (current, current / 32));
  current = prune (mix (current, current * 2048));
  return current;
}

Number
evolve (Number current, int numTimes)
{
  for (int i = 0; i < numTimes; ++i) {
    current = evolve (current);
  }
  return current;
}

Number
part1 (const Problem& prob)
{
  Number sum = 0;
  for (Number current: prob) {
    sum += evolve (current, 2000);
  }
  return sum;
}

Number
part2 (const Problem& prob)
{
  std::vector<Problem> history;
  history.push_back (prob);
  for (int count = 0; count < 2000; ++count) {
    Problem next;
    for (Number num : history.back ()) {
      next.push_back (evolve (num));
    }
    history.push_back (next);
  }

  std::vector<std::vector<int>> prices;
  for (unsigned int i = 0; i < history.size (); ++i) {
    std::vector<int> next;
    for (unsigned int j = 0; j < history.at (i).size (); ++j) {
      next.push_back (history.at (i).at (j) % 10);
    }
    prices.push_back (next);
  }
  std::vector<std::vector<int>> differences;
  for (unsigned timeIndex = 1; timeIndex < prices.size (); ++timeIndex) {
    std::vector<int> next;
    for (unsigned int sellerIndex = 0; sellerIndex < prices.at (timeIndex).size (); ++sellerIndex) {
      int diff = prices[timeIndex][sellerIndex] - prices[timeIndex - 1][sellerIndex];
      assert (diff >= -9 && diff <= 9);
      next.push_back (diff);
    }
    differences.push_back (next);
  }

  Number highest = 0;
  // Could this be, like, a lot more elegant and efficient?  Probably.
  for (int a = -9; a < 10; ++a) {
    for (int b = -9; b < 10; ++b) {
      for (int c = -9; c < 10; ++c) {
        for (int d = -9; d < 10; ++d) {
          Number sum = 0;
          for (unsigned int probIndex = 0; probIndex < prob.size (); ++probIndex) {
            for (unsigned int index = 0; index < 2000 - 4; ++index) {
              if (differences[index][probIndex] == a && differences[index + 1][probIndex] == b
              && differences[index + 2][probIndex] == c && differences[index + 3][probIndex] == d) {
                //std::cout << "Seller " << probIndex << " has pattern " << a << " " << b << " " << c << " " << d << " leading to price " << prices[index + 4][probIndex] << "\n";
                sum += prices[index + 4][probIndex];
                break;
              }
            }
          }
          highest = std::max (highest, sum);
        }
      }
    }
  }
  return highest;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  std::cout << part1 (prob) << "\n";
  std::cout << part2 (prob) << "\n";
  return 0;
}
