/// \file 2019Day24.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/24.

#include <iostream>
#include <string>
#include <cassert>
#include <array>
#include <unordered_set>

#include "utilities.hpp"

constexpr std::size_t SIZE = 5;
constexpr char BUG_CHAR = '#';
constexpr char EMPTY_CHAR = '.';
constexpr bool BUG_BOOL = true;
constexpr bool EMPTY_BOOL = false;
constexpr unsigned int STAY_ALIVE_COUNT_MIN = 1;
constexpr unsigned int STAY_ALIVE_COUNT_MAX = 1;
constexpr unsigned int GROW_COUNT_MIN = 1;
constexpr unsigned int GROW_COUNT_MAX = 2;

using State = std::array<std::array<bool, SIZE>, SIZE>;

State getInput () {
    State initial;
    std::string line;
    unsigned int row {0U};
    while (std::cin >> line) {
        assert (row < SIZE);
        for (unsigned int col {0U}; col < line.size (); ++col) {
            assert (col < SIZE);
            if (line[col] == BUG_CHAR) { initial[row][col] = BUG_BOOL; }
            else if (line[col] == EMPTY_CHAR) { initial[row][col] = EMPTY_BOOL; }
            else { throw std::runtime_error ("Unexpected character in \"" + line + "\""); }
        }
        ++row;
    }
    assert (row == SIZE);
    return initial;
}

unsigned int getNeighborhood (State const& current, std::size_t row, std::size_t col) {
    unsigned int count {0U};
    if (row > 0 && current[row - 1][col] == BUG_BOOL) { ++count; }
    if (row < SIZE - 1 && current[row + 1][col] == BUG_BOOL) { ++count; }
    if (col > 0 && current[row][col - 1] == BUG_BOOL) { ++count; }
    if (col < SIZE - 1 && current[row][col + 1] == BUG_BOOL) { ++count; }
    return count;
}

State nextState (State const& current) {
    State result;
    for (std::size_t row {0U}; row < SIZE; ++row) {
        for (std::size_t col {0U}; col < SIZE; ++col) {
            unsigned int neighborhood = getNeighborhood (current, row, col);
            if (current[row][col] == BUG_BOOL) {
                if (neighborhood >= STAY_ALIVE_COUNT_MIN && neighborhood <= STAY_ALIVE_COUNT_MAX) { result[row][col] = BUG_BOOL; }
                else { result[row][col] = EMPTY_BOOL; }
            }
            else {
                if (neighborhood >= GROW_COUNT_MIN && neighborhood <= GROW_COUNT_MAX) { result[row][col] = BUG_BOOL; }
                else { result[row][col] = EMPTY_BOOL; }
            }
        }
    }
    return result;
}

unsigned int biodiversity (State const& current) {
    unsigned int total {0U};
    unsigned int nextVal {1U};
    for (std::size_t row {0U}; row < SIZE; ++row) {
        for (std::size_t col {0U}; col < SIZE; ++col) {
            if (current[row][col] == BUG_BOOL) { total |= nextVal; }
            nextVal <<= 1;
        }
    }
    return total;
}

unsigned int part1 (State const& initial) {
    std::unordered_set<unsigned int> seen;
    State current (initial);
    seen.insert (biodiversity (current));
    while (true) {
        current = nextState (current);
        unsigned int bio = biodiversity (current);
        if (seen.count (bio) == 1) { return bio; }
        seen.insert (bio);
    }
}

int main () {
    State initial = getInput ();
    std::cout << part1 (initial) << "\n";
    return 0;
}
