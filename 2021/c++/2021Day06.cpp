/// \file 2021Day06.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/6.

#include <iostream>
#include <cstdio>
#include <array>
#include <numeric>

#include "utilities.hpp"

/// The timer that a fish returns to after reproducing.
constexpr unsigned int COUNTER_RESET = 6U;
/// The timer that a brand new fish starts at.
constexpr unsigned int COUNTER_START = 8U;
/// The length of an array needed to store fish counts.
constexpr unsigned int ARRAY_LENGTH = std::max (COUNTER_RESET, COUNTER_START) + 1;

/// An array that stores the counts for each timer of how many fish have that timer value.
using Counts = std::array<unsigned long long, ARRAY_LENGTH>;

/// \brief Reads the input.
/// \return An array of the counts of how many fish are at each timer value.
Counts getInput () {
    Counts counts {0ULL};
    unsigned int x;
    scanf ("%u", &x);
    ++counts[x];
    while (scanf (",%u", &x) > 0) {
        ++counts[x];
    }
    return counts;
}

/// \brief Iterates the fish growth simulation some number of steps.
/// \param[in] original The current state of the simulation.
/// \param[in] times How many steps to move forward in time.
/// \return The state of the simulation after that many additional steps.
Counts iterate (Counts const& original, unsigned int times) {
    Counts previous {original};
    for (unsigned int time {0U}; time < times; ++time) {
        Counts current {0ULL};
        for (unsigned int index {0U}; index < ARRAY_LENGTH - 1; ++index) {
            current[index] = previous[index + 1];
        }
        current[COUNTER_RESET] += previous[0];
        current[COUNTER_START] += previous[0];
        previous = std::move (current);
    }
    return previous;
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    constexpr unsigned int PART1_ITERATIONS = 80U;
    constexpr unsigned int PART2_ITERATIONS = 256U;
    Counts original = getInput ();
    Counts answer = iterate (original, PART1_ITERATIONS);
    std::cout << std::accumulate (answer.begin (), answer.end (), 0ULL) << "\n";
    Counts answer2 = iterate (answer, PART2_ITERATIONS - PART1_ITERATIONS);
    std::cout << std::accumulate (answer2.begin (), answer2.end (), 0ULL) << "\n";
    return 0;
}