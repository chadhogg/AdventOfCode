/// \file 2021Day07.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/7.

#include <iostream>
#include <cstdio>
#include <array>
#include <numeric>
#include <vector>
#include <algorithm>
#include <climits>

#include "utilities.hpp"

/// A collection of the current horizontal positions of various crabs.
using Problem = std::vector<int>;

/// \brief Gets the input from stdin.
/// \return A collection of the current horizontal positions of various crabs.
Problem getInput () {
    Problem problem {};
    int x;
    scanf ("%d", &x);
    problem.push_back (x);
    while (scanf (",%d", &x) > 0) {
        problem.push_back (x);
    }
    return problem;

}

/// \brief Calculates the total fuel used to move every crab to a certain horizontal position.
/// \param[in] problem A collection of the crabs' current horizontal positions.
/// \param[in] loc The proposed horizontal position.
/// \param[in] expensive Whether or not to use expensive (triangular number) fuel costs.
/// \return The total fuel used to align all crabs at loc.
int totalFuel (Problem const& problem, int loc, bool expensive) {
    int total {0U};
    for (int crab : problem) {
        int distance = std::abs (loc - crab);
        if (expensive) {
            total += (distance * (distance + 1)) / 2;
        }
        else {
            total += distance;
        }
    }
    return total;
}

/// \brief Finds the horizontal position all crabs can be aligned to with least fuel use.
/// \param[in] problem A collection of the crabs' current horizontal positions.
/// \param[in] expensvie Whether or not use expensive (triangular number) fuel costs.
/// \return A pair of the best position and the total fuel cost to get there.
std::pair<int, int> getBestOption (Problem const& problem, bool expensive) {
    int end = *std::max_element (problem.begin (), problem.end ());
    std::pair<int, int> best {-1, INT_MAX};
    for (int horiz {0}; horiz <= end; ++horiz) {
        int fuel = totalFuel (problem, horiz, expensive);
        if (fuel < best.second) {
            best.first = horiz;
            best.second = fuel;
        }
    }
    return best;
}


/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Problem problem = getInput ();
    std::pair<int, int> best = getBestOption (problem, false);
    std::cout << best.second << "\n";
    best = getBestOption (problem, true);
    std::cout << best.second << "\n";
    return 0;
}