/// \file 2021Day08.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/8.

#include <iostream>
#include <cstdio>
#include <array>
#include <numeric>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>
#include <set>

#include "utilities.hpp"

/// The number of segments in a digit.
constexpr unsigned int NUM_SEGMENTS = 7;
/// The number of unique digits that can be displayed.
constexpr unsigned int NUM_DIGITS = 10;
/// The number of digits in a reading.
constexpr unsigned int DISPLAY_LENGTH = 4;

/// A listing of which segments (a-g) are lit.
/// Should always be sorted in alpabetical order.
/// Much of this code would be simpler if this were instead std::set<char>, but std::string gives me a lot for free.
using SignalPattern = std::string;

/// One of the 4 x 7-segment displays we are working with.
struct Display {
    /// The 10 unique signal patterns we have seen.
    std::array<SignalPattern, NUM_DIGITS> patterns;
    /// The sequence of 4 signal patterns currently displayed.
    std::array<SignalPattern, DISPLAY_LENGTH> shown;
};

/// A problem to work on, which is just a collection of displays.
using Problem = std::vector<Display>;


/// \brief Reads a display from standard input.
/// \param[inout] is An input stream to work with.
/// \param[out] display The display that should be reinitialized.
/// \return The same input stream.
std::istream& operator>> (std::istream& is, Display& display) {
    for (unsigned int index {0U}; index < NUM_DIGITS; ++index) {
        std::string str = read<std::string> (is);
        for (unsigned int sindex {0U}; sindex < str.size (); ++sindex) {
            std::sort (str.begin (), str.end ());
            display.patterns[index] = str;
        }
    }
    std::string pipe = read<std::string> ();
    assert (!std::cin || pipe == "|");
    for (unsigned int index {0U}; index < DISPLAY_LENGTH; ++index) {
        std::string str = read<std::string> (is);
        for (unsigned int sindex {0U}; sindex < str.size (); ++sindex) {
            std::sort (str.begin (), str.end ());
            display.shown[index] = str;
        }
    }
    return is;
}

/// \brief Reads a problem from standard input.
/// \return The problem to solve.
Problem getInput () {
    Problem prob;
    Display display;
    while (std::cin >> display) {
        prob.push_back (display);
    }
    return prob;
}

/// \brief Solves part 1 of the problem (counting the number of easily identifiable digits in all of the displays combined).
/// \param[in] prob The problem.
/// \post The answer has been printed.
void part1 (Problem const& prob) {
    unsigned int count {0U};
    for (Display const& disp : prob) {
        for (SignalPattern pattern : disp.shown) {
            if (pattern.size () == 2 || pattern.size () == 3 || pattern.size () == 4 || pattern.size () == 7) {
                ++count;
            }
        }
    }
    std::cout << count << "\n";
}

/// \brief Finds the signal patterns that represent the easily identifiable digits.
/// \param[in] disp A display.
/// \param[in] num The digit whose pattern we are searching for, which must be 1, 4, 7, or 8.
/// \return The pattern that represents that digit.
SignalPattern const& findPattern (Display const& disp, unsigned int num) {
    for (SignalPattern const& sig : disp.patterns) {
        if (num == 1 && sig.size () == 2) {
            return sig;
        }
        else if (num == 4 && sig.size () == 4) {
            return sig;
        }
        else if(num == 7 && sig.size () == 3) {
            return sig;
        }
        else if (num == 8 && sig.size () == 7) {
            return sig;
        }
    }
    throw std::runtime_error ("Cannot determine " + std::to_string (num) + " without further analysis.");
}


/// \brief Finds the single letter that is in one signal pattern but not another.
/// \param[in] hasIt A signal pattern that contains a number of letters.
/// \param[in] missingIt Another signal pattern that is missing exactly one letter from hasIt.
/// \return The missing letter.
char missingLetter (SignalPattern const& hasIt, SignalPattern const& missingIt) {
    bool found = false;
    char ret = ' ';
    for (char c : hasIt) {
        if (missingIt.find (c) == std::string::npos) {
            if (found) {
                throw std::runtime_error (missingIt + " was missing more than one letter from " + hasIt);
            }
            found = true;
            ret = c;
        }
    }
    if (!found) {
        throw std::runtime_error (missingIt + " was not missing any letters in " + hasIt);
    }
    return ret;
}

/// \brief Determines whether or not one signal pattern is a superset of another.
/// \param[in] super The potential superset.
/// \param[in] sub The potential subset.
/// \return True if every character in sub is also in super.
bool containsAll (SignalPattern const& super, SignalPattern const& sub) {
    for (char c : sub) {
        if (super.find (c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

/// \brief Finds the only signal pattern of a certain size that is a superset of some other signal pattern, and isn't excluded.
/// \param[in] display The display.
/// \param[in] sub The signal pattern that it should be a superset of.
/// \param[in] size The length the signal pattern should have.
/// \param[in] exclusions A set of signal patterns that should not be considered potential solutions.
/// \return The only signal pattern meeting those critera.
SignalPattern findOnlySuperOfSizeExcluding (Display const& display, SignalPattern const& sub, size_t size, std::set<SignalPattern> const& exclusions) {
    bool found = false;
    SignalPattern ret;
    for (SignalPattern const& patt : display.patterns) {
        if (patt.size () == size && containsAll (patt, sub) && exclusions.count (patt) == 0) {
            if (found) {
                throw std::runtime_error ("Found multiple size " + std::to_string (size) + " patterns that were supersets of " + sub + "and not excluded: " + ret + " and " + patt);
            }
            found = true;
            ret = patt;
        }
    }
    if (!found) {
        throw std::runtime_error ("Could not find a size " + std::to_string (size) + " pattern that is a superset of " + sub + " but is not excluded");
    }
    return ret;
}

/// \brief Finds the only signal pattern of a certain size that is a subset of some other signal pattern, and isn't excluded.
/// \param[in] display The display.
/// \param[in] super The signal pattern that it should be a subset of.
/// \param[in] size The length the signal pattern should have.
/// \param[in] exclusions A set of signal patterns that should not be considered potential solutions.
/// \return The only signal pattern meeting those critera.
SignalPattern findOnlySubOfSizeExcluding (Display const& display, SignalPattern const& super, size_t size, std::set<SignalPattern> const& exclusions) {
    bool found = false;
    SignalPattern ret;
    for (SignalPattern const& patt : display.patterns) {
        if (patt.size () == size && containsAll (super, patt) && exclusions.count (patt) == 0) {
            if (found) {
                throw std::runtime_error ("Found multiple size " + std::to_string (size) + " patterns that were subsets of " + super + "and not excluded: " + ret + " and " + patt);
            }
            found = true;
            ret = patt;
        }
    }
    if (!found) {
        throw std::runtime_error ("Could not find a size " + std::to_string (size) + " pattern that is a subset of " + super + " but is not excluded");
    }
    return ret;
}


/// \brief Determines which signal pattern is used for each digit.
/// \param[in] display The display.
/// \return A map of digit to signal pattern.
std::map<unsigned int, SignalPattern> decode (Display const& display) {
    std::map<unsigned int, SignalPattern> intToPattern;

    // i: The pattern of length 2 is #1, of length 3 is #7, of length 4 is #4, of length 7 is #8
    intToPattern[1] = findPattern (display, 1);
    intToPattern[4] = findPattern (display, 4);
    intToPattern[7] = findPattern (display, 7);
    intToPattern[8] = findPattern (display, 8);

    // ii: The only length-6 pattern that contains all of #4 would be #9.
    intToPattern[9] = findOnlySuperOfSizeExcluding (display, intToPattern[4], 6, {});

    // Note: At this point we can figure out which signal is G, but I don't think that helps us.

    // iii: The only length-6 pattern other than #9 that contains all of #1 would be #0.
    intToPattern[0] = findOnlySuperOfSizeExcluding (display, intToPattern[1], 6, {intToPattern[9]});

    // iv: The only length-6 pattern that isn't #0 or #9 is #6.
    intToPattern[6] = findOnlySuperOfSizeExcluding (display, "", 6, {intToPattern[0], intToPattern[9]});

    // v: The only length-5 pattern that is a superset of #1 is #3.
    intToPattern[3] = findOnlySuperOfSizeExcluding (display, intToPattern[1], 5, {});

    // vi: The only length-5 pattern that is a subset of #6 is #5.
    intToPattern[5] = findOnlySubOfSizeExcluding (display, intToPattern[6], 5, {});

    // vii: The only remaining length-5 pattern is #2.
    intToPattern[2] = findOnlySuperOfSizeExcluding (display, "", 5, {intToPattern[3], intToPattern[5]}); 

    return intToPattern;
}

/// \brief Determines the value of a display.
/// \param[in] display A broken display.
/// \param[in] mapping A mapping of signal patterns in the display to digits.
/// \return The numeric value of the display.
unsigned int interpret (Display const& display, std::map<unsigned int, SignalPattern> const& mapping) {
    unsigned int total {0U};
    unsigned int factor {1000U};
    for (unsigned int index {0U}; index < display.shown.size (); ++index) {
        SignalPattern const& pattern = display.shown.at (index);
        for (auto it = mapping.begin (); it != mapping.end (); ++it) {
            if ((*it).second == pattern) {
                total += factor * (*it).first;
                factor /= 10U;
            }
        }
    }
    return total;
}

/// \brief Solves part of the problem (summing the values shown on all displays).
/// \param[in] prob The problem to solve.
/// \post The solution has been printed.
void part2 (Problem const& prob) {
    unsigned long total {0UL};
    for (Display const& disp : prob) {
        std::map<unsigned int, SignalPattern> solution = decode (disp);
        unsigned int result = interpret (disp, solution);
        //std::cout << result << "\n";
        total += result;
    }
    std::cout << total << "\n";
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Problem prob = getInput ();
    part1 (prob);
    part2 (prob);
    return 0;
}