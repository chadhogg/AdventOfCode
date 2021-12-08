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

char determineA (SignalPattern const& one, SignalPattern const& seven) {
    if (one.find (seven.at (0)) == std::string::npos) {
        return seven.at (0);
    }
    else if(one.find (seven.at (1)) == std::string::npos) {
        return seven.at (1);
    }
    else if(one.find (seven.at (2)) == std::string::npos) {
        return seven.at (2);
    }
    else {
        throw std::runtime_error ("Somehow, every position in 7 was also used in 1?");
    }
}

int decode (Display const& display) {
    std::map<char, char> crossing;
    SignalPattern one = findPattern (display, 1);
    SignalPattern four = findPattern (display, 4);
    SignalPattern seven = findPattern (display, 7);
    crossing['a'] = determineA (one, seven);
    std::cout << "One is " << one << "\n";
    std::cout << "Seven is " << seven << "\n";
    std::cout << "Segment a comes from " << crossing['a'] << "\n";
    // TODO
    return 0;
}

void part2 (Problem const& prob) {
    unsigned long total {0UL};
    for (Display const& disp : prob) {
        total += decode (disp);
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