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
constexpr unsigned int PART2_ITERATIONS = 200;
constexpr unsigned int ALL_DEPTHS = PART2_ITERATIONS * 2 + 3;
constexpr unsigned int MIDDLE_DEPTH = ALL_DEPTHS / 2;
constexpr std::size_t MIDDLE_TILE = SIZE / 2;

using State = std::array<std::array<bool, SIZE>, SIZE>;
using RecursiveState = std::array<std::array<std::array<bool, SIZE>, SIZE>, PART2_ITERATIONS * 2 + 3>;

std::ostream & operator<< (std::ostream & out, State const& state) {
    for (std::size_t row {0U}; row < SIZE; ++row) {
        for (std::size_t col {0U}; col < SIZE; ++col) {
            if (row == MIDDLE_TILE && col == MIDDLE_TILE) { std::cout << '?'; }
            else if (state[row][col] == BUG_BOOL) { std::cout << BUG_CHAR; }
            else { std::cout << EMPTY_CHAR; }
        }
        std::cout << "\n";
    }
    return out;
}

unsigned int countBugs (State const& state) {
    unsigned int count {0U};
    for (std::size_t row {0U}; row < SIZE; ++row) {
        for (std::size_t col {0U}; col < SIZE; ++col) {
            if (state[row][col] == BUG_BOOL) { ++count; }
        }
    }
    return count;
}

unsigned int countBugs (RecursiveState const& state) {
    unsigned int count {0U};
    for (std::size_t depth {0U}; depth < ALL_DEPTHS; ++depth) {
        count += countBugs (state[depth]);
    }
    return count;
}

std::ostream & operator<< (std::ostream & out, RecursiveState const& state) {
    for (std::size_t depth {0U}; depth < ALL_DEPTHS; ++depth) {
        if (countBugs (state[depth]) > 0) {
            out << "Depth " << (int)depth - (int)MIDDLE_DEPTH << ":\n";
            out << state[depth] << "\n";
        }
    }
    return out;
}

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

unsigned int getRecursiveNorthNeighbors (RecursiveState const& current, std::size_t depth, std::size_t row, std::size_t col) {
    unsigned int count {0U};
    if (row == 0) {
        // If in top row, we need to look just above middle tile in lower depth.
        if (current[depth - 1][MIDDLE_TILE - 1][MIDDLE_TILE] == BUG_BOOL) { ++count; }
    }
    else if (row == MIDDLE_TILE + 1 && col == MIDDLE_TILE) {
        // If just below middle tile, we need to look at all parts of bottom row in higher depth.
        for (std::size_t otherCol {0U}; otherCol < SIZE; ++otherCol) {
            if (current[depth + 1][SIZE - 1][otherCol] == BUG_BOOL) { ++count; }
        }
    }
    else { 
        // Just look at the cell above it at the same depth.
        if (current[depth][row - 1][col] == BUG_BOOL) { ++count; }
    }
    return count;
}

unsigned int getRecursiveSouthNeighbors (RecursiveState const& current, std::size_t depth, std::size_t row, std::size_t col) {
    unsigned int count {0U};
    if (row == SIZE - 1) {
        // If in bottom row, we need to look just below middle tile in lower depth.
        if (current[depth - 1][MIDDLE_TILE + 1][MIDDLE_TILE] == BUG_BOOL) { ++count; }
    }
    else if (row == MIDDLE_TILE - 1 && col == MIDDLE_TILE) {
        // If just above middle tile, we ened to look at all parts of top row in higher depth.
        for (std::size_t otherCol {0U}; otherCol < SIZE; ++otherCol) {
            if (current[depth + 1][0][otherCol] == BUG_BOOL) { ++count; }
        }
    }
    else { 
        // Just look at the cell below it at the same depth.
        if (current[depth][row + 1][col] == BUG_BOOL) { ++count; }
    }
    return count;
}

unsigned int getRecursiveWestNeighbors (RecursiveState const& current, std::size_t depth, std::size_t row, std::size_t col) {
    unsigned int count {0U};
    if (col == 0) {
        // If in leftmost col, we need to look just left of middle tile in lower depth.
        if (current[depth - 1][MIDDLE_TILE][MIDDLE_TILE - 1] == BUG_BOOL) { ++count; }
    }
    else if (row == MIDDLE_TILE && col == MIDDLE_TILE + 1) {
        // If just right of middle tile, we need to look at all parts of rightmost col in higher depth.
        for (std::size_t otherRow {0U}; otherRow < SIZE; ++otherRow) {
            if (current[depth + 1][otherRow][SIZE - 1] == BUG_BOOL) { ++count; }
        }
    }
    else { 
        // Just look at the cell left of it at the same depth.
        if (current[depth][row][col - 1] == BUG_BOOL) { ++count; }
    }
    return count;
}

unsigned int getRecursiveEastNeighbors (RecursiveState const& current, std::size_t depth, std::size_t row, std::size_t col) {
    unsigned int count {0U};
    if (col == SIZE - 1) {
        // If in rightmost col, we need to look just right of middle tile in lower depth.
        if (current[depth - 1][MIDDLE_TILE][MIDDLE_TILE + 1] == BUG_BOOL) { ++count; }
    }
    else if (row == MIDDLE_TILE && col == MIDDLE_TILE - 1) {
        // If just left of middle tile, we need to look at all parts of leftmost col in higher depth.
        for (std::size_t otherRow {0U}; otherRow < SIZE; ++otherRow) {
            if (current[depth + 1][otherRow][0] == BUG_BOOL) { ++count; }
        }
    }
    else { 
        // Just look at the cell right of it at the same depth.
        if (current[depth][row][col + 1] == BUG_BOOL) { ++count; }
    }
    return count;
}

unsigned int getRecursiveNeighborhood (RecursiveState const& current, std::size_t depth, std::size_t row, std::size_t col) {
    return getRecursiveNorthNeighbors (current, depth, row, col)
        + getRecursiveSouthNeighbors (current, depth, row, col)
        + getRecursiveWestNeighbors (current, depth, row, col)
        + getRecursiveEastNeighbors (current, depth, row, col);
}

RecursiveState nextRecursiveState (RecursiveState const& current) {
    RecursiveState result;
    for (std::size_t depth {1U}; depth < ALL_DEPTHS - 1; ++depth) {
        for (std::size_t row {0U}; row < SIZE; ++row) {
            for (std::size_t col {0U}; col < SIZE; ++col) {
                unsigned int neighborhood = getRecursiveNeighborhood (current, depth, row, col);
                if (row == MIDDLE_TILE && col == MIDDLE_TILE) {
                    result[depth][row][col] = EMPTY_BOOL;
                }
                else if (current[depth][row][col] == BUG_BOOL) {
                    if (neighborhood >= STAY_ALIVE_COUNT_MIN && neighborhood <= STAY_ALIVE_COUNT_MAX) { result[depth][row][col] = BUG_BOOL; }
                    else { result[depth][row][col] = EMPTY_BOOL; }
                }
                else {
                    if (neighborhood >= GROW_COUNT_MIN && neighborhood <= GROW_COUNT_MAX) { result[depth][row][col] = BUG_BOOL; }
                    else { result[depth][row][col] = EMPTY_BOOL; }
                }
            }
        }
    }
    return result;
}

unsigned int part2 (State const& initial) { 
    RecursiveState current;
    for (std::size_t depth {0U}; depth < ALL_DEPTHS; ++depth) {
        for (std::size_t row {0U}; row < SIZE; ++row) {
            for (std::size_t col {0U}; col < SIZE; ++col) {
                current[depth][row][col] = EMPTY_BOOL;
            }
        }
    }
    current[MIDDLE_DEPTH] = initial;
    for (unsigned int iteration {0U}; iteration < PART2_ITERATIONS; ++iteration) {
        current = nextRecursiveState (current);
        //std::cout << "After " << iteration + 1 << " iterations:\n" << current << "\n\n";
    }
    return countBugs (current);
}

int main () {
    State initial = getInput ();
    std::cout << part1 (initial) << "\n";
    std::cout << part2 (initial) << "\n";
    /*
    RecursiveState start;
    start[MIDDLE_DEPTH] = initial;
    std::cout << start << "\n\n";
    RecursiveState after1 = nextRecursiveState (start);
    std::cout << after1 << "\n\n";
    RecursiveState after2 = nextRecursiveState (after1);
    std::cout << after2 << "\n\n";
    RecursiveState after3 = nextRecursiveState (after2);
    std::cout << after3 << "\n\n";
    */
    return 0;
}
