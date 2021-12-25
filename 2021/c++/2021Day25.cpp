/// \file 2021Day25.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/25.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_set>

#include "utilities.hpp"

constexpr char EAST = '>';
constexpr char SOUTH = 'v';
constexpr char BLANK = '.';

using Map = std::vector<std::string>;

std::string mapToString (Map const& map) {
    std::string result {""};
    for (std::string const& line : map) {
        result += line + "\n";
    }
    return result;
}

Map getInput () {
    Map map;
    std::string line;
    while (std::cin >> line) {
        map.push_back (line);
        for (char c : line) {
            assert (c == EAST || c == SOUTH || c == BLANK);
        }
    }

    return map;
}

unsigned int iterateUntilStop (Map const& initial) {
    Map current {initial};
    //std::cout << "Initial state:\n" << mapToString (current) << "\n";
    unsigned int iteration {0U};
    while (true) {
        unsigned int moves {0U};
        std::vector<std::vector<bool>> eastCanMove;
        for (int row {0}; row < (int)current.size (); ++row) {
            std::vector<bool> result;
            for (int col {0}; col < (int)current.at (row).size (); ++col) {
                if (current.at (row).at (col) == EAST && current.at (row).at ((col + 1) % current.at (row).size ()) == BLANK) {
                    result.push_back (true);
                }
                else {
                    result.push_back (false);
                }
            }
            eastCanMove.push_back (result);
        }
        for (int row {0}; row < (int)current.size (); ++row) {
            for (int col {0}; col < (int)current.at (row).size (); ++col) {
                if (eastCanMove.at (row).at (col)) {
                    current.at (row).at (col) = BLANK;
                    current.at (row)[(col + 1) % current.at (row).size ()] = EAST;
                    ++moves;
                }
            }
        }
        std::vector<std::vector<bool>> southCanMove;
        for (int row {0}; row < (int)current.size (); ++row) {
            std::vector<bool> result;
            for (int col {0}; col < (int)current.at (row).size (); ++col) {
                if (current.at (row).at (col) == SOUTH && current.at ((row + 1) % current.size ()).at (col) == BLANK) {
                    result.push_back (true);
                }
                else {
                    result.push_back (false);
                }
            }
            southCanMove.push_back (result);
        }
        for (int row {0}; row < (int)current.size (); ++row) {
            for (int col {0}; col < (int)current.at (row).size (); ++col) {
                if (southCanMove.at (row).at (col)) {
                    current.at (row).at (col) = BLANK;
                    current.at ((row + 1) % current.size ()).at (col) = SOUTH;
                    ++moves;
                }
            }
        }
        ++iteration;
        //std::cout << "After " << iteration << (iteration == 1 ? " step:\n" : " steps:\n") << mapToString (current) << "\n";
        if (moves == 0U) { return iteration; }
    }
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    Map initial = getInput ();
    std::cout << iterateUntilStop (initial) << "\n";
    return 0;
}
