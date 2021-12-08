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

constexpr unsigned int NUM_SEGMENTS = 7;
constexpr unsigned int NUM_DIGITS = 10;
constexpr unsigned int DISPLAY_LENGTH = 4;

using SignalPattern = std::string;

struct Display {
    std::array<SignalPattern, NUM_DIGITS> patterns;
    std::array<SignalPattern, DISPLAY_LENGTH> shown;
};

using Problem = std::vector<Display>;

using SignalIdentity = std::map<SignalPattern, unsigned int>;

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


/*
using SignalPattern = bool[NUM_SEGMENTS];

struct Display {
    SignalPattern patterns[NUM_DIGITS];
    SignalPattern shown[DISPLAY_LENGTH];
};

using Problem = std::vector<Display>;

std::istream& operator>> (std::istream& is, Display& display) {
    for (unsigned int index {0U}; index < NUM_DIGITS; ++index) {
        std::string str = read<std::string> (is);
        for (unsigned int sindex {0U}; sindex < str.size (); ++sindex) {
            assert (display.patterns[index][str.at (sindex) - 'a'] == false);
            display.patterns[index][str.at (sindex) - 'a'] = true;
        }
    }
    std::string pipe = read<std::string> ();
    assert (pipe == "|");
    for (unsigned int index {0U}; index < DISPLAY_LENGTH; ++index) {
        std::string str = read<std::string> (is);
        for (unsigned int sindex {0U}; sindex < str.size (); ++sindex) {
            assert (display.shown[index][str.at (sindex) - 'a'] == false);
            display.shown[index][str.at (sindex) - 'a'] = true;
        }
    }
    return is;
}
*/

Problem getInput () {
    Problem prob;
    Display display;
    while (std::cin >> display) {
        prob.push_back (display);
    }
    return prob;
}

SignalIdentity identifySignals (Display const& display) {
    SignalIdentity identity;
    for (std::string const& str : display.patterns) {
        if (str.size () == 2) {
            identity[str] = 1;
        }
        else if (str.size () == 3) {
            identity[str] = 7;
        }
        else if (str.size () == 4) {
            identity[str] = 4;
        }
        else if (str.size () == 7) {
            identity[str] = 8;
        }
    }
    return identity;
}

void part1 (Problem const& prob) {
    unsigned int count {0U};
    for (Display const& disp : prob) {
        SignalIdentity ident = identifySignals (disp);
        for (SignalPattern out : disp.shown) {
            if (ident.count (out) == 1) {
                ++count;
            }
        }
    }
    std::cout << count << "\n";
}

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




/*

SignalPattern make9 (SignalPattern const& four, char a) {
    SignalPattern nine = four;
    nine += a;
    std::sort (nine.begin (), nine.end ());
    return nine;
}

SignalPattern determine0 (SignalPattern const& one, Display const& display) {
    bool found = false;
    SignalPattern ret;
    for (SignalPattern const& sig : display.patterns) {
        if (sig.size () == 6 && sig.find (one.at (0)) != std::string::npos && sig.find (one.at (1)) != std::string::npos) {
            if (found) {
                std::cout << ret << "\n";
                std::cout << sig << "\n";
                std::cout << one << "\n";
                throw std::runtime_error ("Find more than one size-6 pattern that contained all elements of 1.");
            }
            found = true;
            ret = sig;
        }
    }
    if (!found) {
        throw std::runtime_error("Couldn't find 0.");
    }
    return ret;
}

SignalPattern determine6 (SignalPattern const& zero, Display const& display) {
    bool found = false;
    SignalPattern ret;
    for (SignalPattern const& sig : display.patterns) {
        if (sig.size () == 6 && sig != zero) {
            if (found) {
                throw std::runtime_error ("Found more than one size-6 pattern that wasn't 0.");
            }
            found = true;
            ret = sig;
        }
    }
    if (!found) {
        throw std::runtime_error("Couldn't find 6.");
    }
    return ret;
}

SignalPattern determine5 (SignalPattern const& nine, SignalPattern const& one, Display const& display) {
    bool found = false;
    SignalPattern ret;
    for (SignalPattern const& sig : display.patterns) {
        if (sig.size () == 5 && sig != nine && sig.find (one.at (0)) != std::string::npos && sig.find (one.at (1)) != std::string::npos) {
            if (found) {
                throw std::runtime_error ("Found multiple size-5 signals that were'nt nine but contained all parts of 1.");
            }
            found = true;
            ret = sig;
        }
    }
    if (!found) {
        throw std::runtime_error ("Couldn't find 5.");
    }
    return ret;
}

*/









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


bool containsAll (SignalPattern const& super, SignalPattern const& sub) {
    for (char c : sub) {
        if (super.find (c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

SignalPattern findOnlySupersetOfSize (Display const& display, SignalPattern const& sub, size_t size) {
    bool found = false;
    SignalPattern ret;
    for (SignalPattern const& patt : display.patterns) {
        if (patt.size () == size && containsAll (patt, sub)) {
            if (found) {
                throw std::runtime_error ("Found multiple size " + std::to_string (size) + " patterns that were supersets of " + sub + ": " + ret + " and " + patt);
            }
            found = true;
            ret = patt;
        }
    }
    if (!found) {
        throw std::runtime_error ("Could not find a size " + std::to_string (size) + " pattern that is a superset of " + sub);
    }
    return ret;
}

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



std::map<unsigned int, SignalPattern> decode (Display const& display) {
    std::map<unsigned int, SignalPattern> intToPattern;

    // i: The pattern of length 2 is #1, of length 3 is #7, of length 4 is #4, of length 7 is #8
    intToPattern[1] = findPattern (display, 1);
    intToPattern[4] = findPattern (display, 4);
    intToPattern[7] = findPattern (display, 7);
    intToPattern[8] = findPattern (display, 8);

    // ii: The only length-6 pattern that contains all of #4 would be #9.
    intToPattern[9] = findOnlySupersetOfSize (display, intToPattern[4], 6);

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