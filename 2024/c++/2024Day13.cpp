/// \file 2024Day13.cpp
/// \author Chad Hogg
/// \brief My solution to Advent Of Code for 2024-12-13.


// Algebraic notes:
// ax + by = c and dx + ey = f
//   (a, b, c, d, e, and f) are A's X-delta, B's X-delta, Prize's X, A's Y-delta, B's Y-delta, Prize's Y
//   (x, y) are number of presses for A and B, respectively
// ax = c - by
// x = (c - by) / a
// ((c - by) / a) * d + ey = f
// ((c - by) * d) + aey = af
// (ae - db) * y = af - cd
// y = (af - cd) / (ae - db)
// x = (c - by) / a
// Manually verified on first machine in first example.

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

using Number = long long;
using Button = std::pair<Number, Number>;
using Prize = std::pair<Number, Number>;
using Machine = std::pair<std::pair<Button, Button>, Prize>;
using Problem = std::vector<Machine>;

Problem
readInput ()
{
  Problem prob;
  Machine next;
  while (scanf ("Button A: X+%lld, Y+%lld\n"
                "Button B: X+%lld, Y+%lld\n"
                "Prize: X=%lld, Y=%lld", &next.first.first.first, &next.first.first.second, 
                &next.first.second.first, &next.first.second.second, 
                &next.second.first, &next.second.second) == 6) {
    prob.push_back (next);
    scanf ("\n");
  }
  return prob;
}

std::optional<std::pair<Number, Number>>
integerSolutionToLinearEquations (Number a, Number b, Number c, Number d, Number e, Number f)
{
  if ((a * f - c * d) % (a * e - d * b) == 0) {
    Number y = (a * f - c * d) / (a * e - d * b);
    if ((c - b * y) % a == 0) {
      Number x = (c - b * y) / a;
      if (x >= 0 && y >= 0) { return std::pair<Number, Number> (x, y); }
    }
  }
  return {};
}

const Number ACOST = 3;
const Number BCOST = 1;

Number
minTokensForWinnablePrizes (const Problem& prob, Number addedToPrize) {
  Number total = 0;
  for (const Machine& machine: prob) {
    std::optional<std::pair<Number, Number>> solution = integerSolutionToLinearEquations (machine.first.first.first, machine.first.second.first, machine.second.first + addedToPrize, machine.first.first.second, machine.first.second.second, machine.second.second + addedToPrize);
    if (solution) {
      total += solution->first * ACOST + solution->second * BCOST;
    }
  }
  return total;
}

/// \brief Runs the program.
/// \return Always 0.
int
main ()
{
  Problem prob = readInput ();
  std::cout << minTokensForWinnablePrizes (prob, 0) << "\n";
  std::cout << minTokensForWinnablePrizes (prob, 10000000000000) << "\n";
  return 0;
}
