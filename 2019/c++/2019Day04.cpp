/// \file 2019Day04.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/4.

#include <iostream>
#include <vector>
#include <utility>
#include <climits>
#include <array>

#include "utilities.hpp"

constexpr unsigned int RANGE_MIN = 172930U;
constexpr unsigned int RANGE_MAX = 683082U;

constexpr unsigned int NUM_DIGITS = 6;
constexpr unsigned int ULTIMATE_INDEX = 5;
constexpr unsigned int PENULTIMATE_INDEX = ULTIMATE_INDEX - 1U;
constexpr unsigned int FIRST_INDEX = 0;

using Digits = std::array<char, NUM_DIGITS>;

Digits digitize (unsigned int num) {
    Digits digits;
    digits[0] = num / 100000;
    digits[1] = num % 100000 / 10000;
    digits[2] = num % 10000 / 1000;
    digits[3] = num % 1000 / 100;
    digits[4] = num % 100 / 10;
    digits[5] = num % 10;
    return digits;
}

bool isValid (unsigned int potential, bool triplesAllowed) {
    Digits digits = digitize (potential);
    bool nonDecreasing = true;
    bool doubled = false;
    for (unsigned int index {FIRST_INDEX}; index < ULTIMATE_INDEX; ++index) {
        if (digits[index + 1] < digits[index]) { nonDecreasing = false; }
        if (digits[index] == digits[index + 1]) {
            if (triplesAllowed) {
                doubled = true;
            }
            else if ((index == PENULTIMATE_INDEX || digits[index] != digits[index + 2]) && (index == FIRST_INDEX || digits[index] != digits[index - 1])) {
                doubled = true;
            }
        }
    }
    return nonDecreasing && doubled;
}

unsigned int countPossiblePasswordsBruteForce (bool triplesAllowed) {
    unsigned int count {0U};
    for (unsigned int potential {RANGE_MIN}; potential <= RANGE_MAX; ++potential) {
        if (isValid (potential, triplesAllowed)) { ++count; }
    }
    return count;
}

int main () {
    std::cout << countPossiblePasswordsBruteForce (true) << "\n";
    std::cout << countPossiblePasswordsBruteForce (false) << "\n";
    return 0;
}
