/// \file 2021Day11.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/11.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <algorithm>

#include "utilities.hpp"

/// The row/column at which we start storing real data.  (There's a dummy row/column before it.)
constexpr unsigned int REAL_GRID_START = 1U;
/// The last row/column in which we store real data.  (There's a dummy row/column after it.)
constexpr unsigned int REAL_GRID_LAST = 10U;
/// The total size of the grid (10 rows/columns of real data with a dummy on either size).
constexpr unsigned int SIZE = 12U;
/// The lowest possible energy level, which each octopus returns to after flashing.
constexpr unsigned int MIN_ENERGY_LEVEL = 0U;
/// The lowest energy level at which an octopus flashes.
constexpr unsigned int FLASH_ENERGY_LEVEL = 10U;
/// The amount by which an octopus's energy level increases.
constexpr unsigned int ENERGY_INCREMENT = 1U;

/// A square 2-d array with a dummy row and column around the borders.
template<typename T>
using Grid = std::array<std::array<T, SIZE>, SIZE>;

/// \brief Inserts an image of a grid into an output stream.
/// \param[inout] out The stream.
/// \param[in] levels The grid.
/// \return The stream.
std::ostream& operator<< (std::ostream& out, Grid<unsigned int> levels) {
    for (unsigned int row {REAL_GRID_START}; row <= REAL_GRID_LAST; ++row) {
        for (unsigned int col {REAL_GRID_START}; col <= REAL_GRID_LAST; ++col) {
            out << levels[row][col];
        }
        out << "\n";
    }
    return out;
}

/// \brief Gets the input.
/// \return A grid containing all of the octopus's initial energy levels.
Grid<unsigned int> getInput () {
    Grid<unsigned int> levels;
    for (unsigned int row {REAL_GRID_START}; row <= REAL_GRID_LAST; ++row) {
        std::string line = read<std::string> ();
        for (unsigned int col {REAL_GRID_START}; col <= REAL_GRID_LAST; ++col) {
            levels[row][col] = line.at (col - REAL_GRID_START) - '0';
        }
    }
    return levels;
}

/// \brief Increments the energy level of each octopus in a range.
/// \param[inout] levels The current energy level of each octopus.
/// \param[in] firstRow The first row in which octopi should be incremented.
/// \param[in] lastRow The last row in which octopi should be incremented.
/// \param[in] firstCol The first column in which octopi should be incremented.
/// \param[in] lastCol The last column in which octopi should be incremented.
/// \post The energy level of every octopus in range has been incremented.
void increment (Grid<unsigned int> & levels, unsigned int firstRow, unsigned int lastRow, unsigned int firstCol, unsigned int lastCol) {
    for (unsigned int row {firstRow}; row <= lastRow; ++row) {
        for (unsigned int col {firstCol}; col <= lastCol; ++col) {
            levels[row][col] += ENERGY_INCREMENT;
        }
    }
}

/// \brief Does one time-step of octopus energy level changes.
/// \param[inout] levels The octopus energy levels.
/// \return The number of octopi that flashed during this time-step.
/// \post Every octopus's energy level was incremented, some of them may have flashed, and any that flashed used up all of their energy.
unsigned int doStep (Grid<unsigned int> & levels) {
    increment (levels, REAL_GRID_START, REAL_GRID_LAST, REAL_GRID_START, REAL_GRID_LAST);
    Grid<bool> flashed {false};
    unsigned int totalFlashes {0U};
    bool atLeastOneFlash {true};
    while (atLeastOneFlash) {
        atLeastOneFlash = false;
        for (unsigned int row {REAL_GRID_START}; row <= REAL_GRID_LAST; ++row) {
            for (unsigned int col {REAL_GRID_START}; col <= REAL_GRID_LAST; ++col) {
                if (levels[row][col] >= FLASH_ENERGY_LEVEL && !flashed[row][col]) {
                    flashed[row][col] = true;
                    increment (levels, row - 1, row + 1, col - 1, col + 1);
                    ++totalFlashes;
                    atLeastOneFlash = true;
                }
            }
        }
    }
    for (unsigned int row {REAL_GRID_START}; row <= REAL_GRID_LAST; ++row) {
        for (unsigned int col {REAL_GRID_START}; col <= REAL_GRID_LAST; ++col) {
            if (flashed[row][col]) {
                levels[row][col] = MIN_ENERGY_LEVEL;
            }
        }
    }
    return totalFlashes;
}

/// \brief Runs the simulation through many time-steps.
/// \param[inout] levels The octopus energy levels.
/// \param[in] steps The number of time-steps to run.
/// \return The total number of flashes that occurred among all steps.
unsigned int doManySteps (Grid<unsigned int> & levels, unsigned int steps) {
    unsigned int total {0U};
    for (unsigned int count {0U}; count < steps; ++count) {
        total += doStep (levels);
    }
    return total;
}

/// \brief Checks whether or not every real octopus level is zero.
/// \param[in] levels The octopus energy levels.
/// \return True if every energy level is zero.
bool allZeroes (Grid<unsigned int> const& levels) {
    for (unsigned int row {REAL_GRID_START}; row <= REAL_GRID_LAST; ++row) {
        for (unsigned int col {REAL_GRID_START}; col <= REAL_GRID_LAST; ++col) {
            if (levels[row][col] != MIN_ENERGY_LEVEL) {
                return false;
            }
        }
    }
    return true;
}

/// \brief Runs the simulation until all octopi flash at the same time.
/// \param[inout] levels The octopus energy levels.
/// \return The number of time-steps until they all flash simultaneously.
unsigned int stepUntilAllFlash (Grid<unsigned int> & levels) {
    unsigned int count {0U};
    while (true) {
        doStep (levels);
        ++count;
        if (allZeroes (levels)) {
            return count;
        }
    }
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Grid<unsigned int> levels = getInput ();
    Grid<unsigned int> copy (levels);
    std::cout << doManySteps (copy, 100U) << "\n";
    std::cout << stepUntilAllFlash (levels) << "\n";
    return 0;
}