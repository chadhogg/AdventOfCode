/// \file 2021Day10.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/10.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <algorithm>

#include "utilities.hpp"

/// Thankfully, the input is a just a series of strings.
using Problem = std::vector<std::string>;

/// A map from characters to the points you get for finding them at an illegal place in a corrupt string.
const std::unordered_map<char, unsigned int> CORRUPT_POINTS { {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };
/// A map from opening characters to their matching closing characters.
const std::unordered_map<char, char> MATCHES { {'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'} };
/// A map from characters to the points you get for adding them to an incomplete line.
const std::unordered_map<char, char> INCOMPLETE_POINTS { {')', 1}, {']', 2}, {'}', 3}, {'>', 4} };
/// The number we multiply by when moving to the next character when completing an incomplete line.
const unsigned int FACTOR = 5;


/// Gets the input.
/// \return A vector of strings.
Problem getInput () {
    Problem lines;
    std::string line;
    while (std::cin >> line) {
        lines.push_back (line);
    }
    return lines;
}

/// Adds the points of the first illegal character in every corrupt line.
/// \param[in] problem A vector of strings.
/// \return The total points of the first illegal characters on each corrupt line.
unsigned int addFirstIllegalCharacterPoints (Problem const& problem) {
    unsigned int total {0U};
    for (std::string const& line : problem) {
        std::stack<char> stack;
        for (char const& c : line) {
            if (CORRUPT_POINTS.count (c) == 0U) {
                stack.push (c);
            }
            else {
                char top = stack.top ();
                stack.pop ();
                if (c != MATCHES.at (top)) {
                    total += CORRUPT_POINTS.at (c);
                    break;
                }
            }
        }
    }
    return total;
}

/// Finds the middle of the scores for all incomplete lines.
/// \param[in] problem A vector of strings.
/// \return Of the scores for incomplete lines, the lowest.
unsigned long findMiddleIncompleteScore (Problem const& problem ) {
    std::vector<unsigned long> scores;
    for (std::string const& line : problem ) {
        std::stack<char> stack;
        bool corrupt = false;
        for (char const& c : line) {
            if (CORRUPT_POINTS.count (c) == 0U) {
                stack.push (c);
            }
            else {
                char top = stack.top ();
                stack.pop ();
                if (c != MATCHES.at (top)) {
                    corrupt = true;
                    break;
                }
            }
        }
        if (!corrupt) {
            unsigned long score = 0;
            while (!stack.empty() ) {
                score *= 5;
                char top = stack.top ();
                stack.pop ();
                score += INCOMPLETE_POINTS.at (MATCHES.at (top));
            }
            scores.push_back (score);
        }
    }
    std::sort (scores.begin (), scores.end ());
    return scores.at (scores.size () / 2);
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Problem lines = getInput ();
    std::cout << addFirstIllegalCharacterPoints (lines) << "\n";
    std::cout << findMiddleIncompleteScore (lines) << "\n";
    return 0;
}