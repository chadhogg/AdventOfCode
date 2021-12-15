/// \file 2021Day14.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/14.

#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>

#include "utilities.hpp"

/// Two characters beside each other in a (very long eventually) string.
using Pair = std::pair<char, char>;

/// A problem to solve.
struct Problem {
    /// The rules, like AB -> C means replace AB with ACB.
    std::unordered_map<Pair, char> rules;
    /// The counts, like AB -> 12 means that there are 12 places in the string were a B immediately follows an A.
    std::unordered_map<Pair, unsigned long> counts;
    /// Which character appears first in the string, for getting accurate counts.
    char firstLetter;
    /// Which character appears last in the string, for getting accurate counts.
    char lastLetter;
};

/// \brief Prints a mapping from pair to occurrence count.
/// \param[inout] out An output stream.
/// \param[in] counts The map.
/// \return The output stream.
/// \post The information in the map has been inserted into the output stream.
std::ostream& operator<< (std::ostream & out, std::unordered_map<Pair, unsigned long> const& counts) {
    for (std::pair<Pair, unsigned long> const& count : counts) {
        out << count.first.first << count.first.second << ": " << count.second << "\n";
    }
    return out;
}

/// \brief Gets the input.
/// \return A problem to solve.
Problem getInput () {
    Problem prob {};
    std::string line = read<std::string> ();
    for (unsigned int index {0U}; index < line.size () - 1; ++index) {
        Pair pair {line.at (index), line.at (index + 1)};
        if (prob.counts.find (pair) == prob.counts.end ()) {
            prob.counts.insert ({pair, 1UL});
        }
        else {
            prob.counts[pair] = prob.counts.at (pair) + 1UL;
        }
    }
    prob.firstLetter = line.front ();
    prob.lastLetter = line.back ();
    std::getline (std::cin, line);
    assert (line == "");
    std::getline (std::cin, line);
    assert (line == "");
    while (std::getline (std::cin, line)) {
        assert (line.size () == 7);
        assert (line.at (2) == ' ');
        assert (line.at (3) == '-');
        assert (line.at (4) == '>');
        assert (line.at (5) == ' ');
        Pair pair {line.at (0), line.at (1)};
        char result {line.at (6)};
        assert (prob.rules.find (pair) == prob.rules.end ());
        prob.rules.insert ({pair, result});
    }
    return prob;
}

/// Iterates a problem forward by one step.
/// \param[in] orig The current state of the system.
/// \return The next state of the system.
Problem iterate (Problem const& orig) {
    Problem revised;
    revised.rules = orig.rules;
    revised.firstLetter = orig.firstLetter;
    revised.lastLetter = orig.lastLetter;
    for (std::pair<Pair, unsigned long> const& existing : orig.counts) {
        Pair const& currentPair = existing.first;
        char middle = orig.rules.at (currentPair);
        Pair first = Pair {currentPair.first, middle};
        Pair second = Pair {middle, currentPair.second};
        if (revised.counts.find (first) == revised.counts.end ()) {
            revised.counts.insert ({first, existing.second});
        }
        else {
            revised.counts[first] += existing.second;
        }
        if (revised.counts.find (second) == revised.counts.end ()) {
            revised.counts.insert ({second, existing.second});
        }
        else {
            revised.counts[second] += existing.second;
        }
    }
    return revised;
}

/// \brief Iterates a problem forward by some number of steps.
/// \param[in] orig The current state of the system.
/// \param[in] times How many steps to iterate.
/// \return The new state of the system.
Problem iterate (Problem const& orig, unsigned int times) {
    Problem current {orig};
    for (unsigned int time {0U}; time < times; ++time) {
        current = iterate (current);
        //std::cout << current.counts << "\n";
    }
    return current;
}

/// Determines the count of how many times a letter occurs.
/// \param[in] element The letter.
/// \param[in] count How many pairs containing that letter exist.
/// \param[in] prob The problem being solved.
/// \return The number of times that letter occurs in the string.
unsigned long correctElementCount (char element, unsigned long count, Problem const& prob) {
    // letter appeared at beginning, X times in middle, and at end: count should be X + 2
    // number of pairs it appears in will be 2X + 2
    if (element == prob.firstLetter && element == prob.lastLetter) {
        return count / 2 + 1;
    }
    // letter appeared at beginning or end and X times in middle: count should be X + 1
    // number of pairs it appears in will be 2X + 1
    else if(element == prob.firstLetter || element == prob.lastLetter) {
        return count / 2 + 1;
    }
    // letter appeared X times in middle: count should be X
    // number of pairs it appears in will be 2X
    else {
        return count / 2;
    }
}

/// \brief Finds the difference between the number of times the most frequent and least frequent letter appear.
/// \param[in] prob The problem being solved.
/// \return The difference.
unsigned long countDiff (Problem const& prob) {
    std::unordered_map<char, unsigned long> elements;
    for (std::pair<Pair, unsigned long> const& count : prob.counts) {
        if (elements.find (count.first.first) == elements.end ()) {
            elements.insert ({count.first.first, count.second});
        }
        else {
            elements[count.first.first] += count.second;
        }
        if (elements.find (count.first.second) == elements.end ()) {
            elements.insert ({count.first.second, count.second});
        }
        else {
            elements[count.first.second] += count.second;
        }
    }
    elements[prob.firstLetter] -= 1;
    elements[prob.lastLetter] -= 1;
    unsigned long max = 0UL;
    unsigned long min = INT64_MAX;
    char maxc, minc;
    for (std::pair<char, unsigned long> element : elements) {
        if (element.second > max) { max = element.second; maxc = element.first; }
        if (element.second < min) { min = element.second; minc = element.first; }
    }
    max = correctElementCount (maxc, max, prob);
    min = correctElementCount (minc, min, prob);
    return max - min;
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Problem original = getInput ();
    Problem after10 = iterate (original, 10U);
    std::cout << countDiff (after10) << "\n";
    Problem after40 = iterate (after10, 30U);
    std::cout << countDiff (after40) << "\n";
    return 0;
}