/// \file 2019Day16.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/16.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>

#include "utilities.hpp"

/* notes
position:       abcdefgh
original input: 12345678
formulas:
a' = a - c + e - g
b' = b + c - f - g
c' = c + d + e
d' = d + e + f + g
e' = e + f + g + h
f' = f + g + h
g' = g + h
h' = h

a'' = a' - c' + e' - g'
b'' = b' + c' - f' - g'
c'' = c' + d' + e'
d'' = d' + e' + f' + g'
e'' = e' + f' + g' + h'
f'' = f' + g' + h'
g'' = g' + h'
h'' = h'

a'' = (a - c + e - g) - (c + d + e) + (e + f + g + h) - (g + h)
b'' = (b + c - f - g) + (c + d + e) - (f + g + h) - (g + h)
c'' = (c + d + e) + (d + e + f + g) + (e + f + g + h)
d'' = (d + e + f + g) + (e + f + g + h) + (f + g + h) + (g + h)
e'' = (e + f + g + h) + (f + g + h) + (g + h) + (h)
f'' = (f + g + h) + (g + h) + (h)
g'' = (g + h) + (h)
h'' = h

a'' = a - 2c - d + e + f - g
b'' = b + 2c + d + e - 2f -3g -2h
c'' = c + 2d + 3e + 2f + 2g + h
d'' = d + 2e + 3f + 4g + 3h
e'' = e + 2f + 3g + 4h
f'' = f + g2 + 3h
g'' = g + 2h
h'' = h

but this is ignoring the modular arithmetic ...
*/

using Sequence = std::vector<int>;

const Sequence BASE_PATTERN = {0, 1, 0, -1};

Sequence getPatternForIndex (unsigned int index) {
    static std::vector<Sequence> patterns;
    if (index < patterns.size ()) {
        return patterns.at (index);
    }
    assert (patterns.size () == index);
    Sequence pattern;
    for (int num : BASE_PATTERN) {
        for (unsigned int count {0U}; count <= index; ++count) {
            pattern.push_back (num);
        }
    }
    patterns.push_back (pattern);
    return pattern;
}

Sequence doPhase (Sequence input) {
    Sequence output;
    output.reserve (input.size ());
    for (unsigned int index {0U}; index < input.size (); ++index) {
        Sequence pattern = getPatternForIndex (index);
        unsigned int patternIndex = 1U;
        int sum {0};
        for (int num : input) {
            sum += num * pattern[patternIndex];
            patternIndex = (patternIndex + 1) % pattern.size ();
        }
        output.push_back (std::abs (sum) % 10);
    }
    return output;
}

Sequence doMultiplePhases (Sequence const& input, unsigned int toDo) {
    Sequence output = input;
    std::cout << "Original :" << "\n";
    for (unsigned int count {0U}; count < toDo; ++count) {
        output = doPhase (output);
        std::cout << "After " << std::setw (4) << count << " phases: " << "\n";
    }
    return output;
}

Sequence getInput () {
    std::string line = read<std::string> ();
    Sequence result;
    for (char c : line) {
        result.push_back (c - '0');
    }
    return result;
}

unsigned long sublist (Sequence const& sequence, unsigned int offset, unsigned int length) {
    unsigned long result = 0;
    for (unsigned int count {0U}; count < length; ++count) {
        result *= 10;
        result += sequence.at (offset + count);
    }
    return result;
}

Sequence expand (Sequence const& original, unsigned int duplications) {
    Sequence result;
    result.reserve (duplications * original.size ());
    for (unsigned int count {0U}; count < duplications; ++count) {
        for (int i : original) {
            result.push_back (i);
        }
    }
    return result;
}

int main () {
    Sequence input = getInput ();
    Sequence after100 = doMultiplePhases (input, 100U);
    std::cout << sublist (after100, 0U, 8U) << "\n";
    Sequence expanded = expand (input, 10000U);
    Sequence expandedAfter100 = doMultiplePhases (expanded, 100U);
    std::cout << sublist (expandedAfter100, sublist (input, 0U, 7U), 8U) << "\n";
    return 0;
}
