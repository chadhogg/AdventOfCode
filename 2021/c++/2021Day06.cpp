/// \file 2021Day05.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/6.

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <array>
#include <algorithm>
#include <functional>
#include <numeric>

#include "utilities.hpp"

using Num = unsigned int;

std::vector<Num> getInput () {
    std::vector<Num> result;
    Num x;
    scanf("%u", &x);
    result.push_back (x);
    while (scanf (",%u", &x) > 0) {
        result.push_back (x);
    }
    return result;
}

std::vector<Num> iterate (std::vector<Num> const& old) {
    std::vector<Num> next;
    for (Num x : old) {
        if (x == 0) {
            next.push_back (6);
            next.push_back (8);
        }
        else {
            next.push_back (x - 1);
        }
    }
    return next;
}

std::vector<Num> generations (std::vector<Num> const& original, unsigned int times) {
    std::vector<Num> previous {original};
    std::vector<Num> current;
    for (Num time {0u}; time < times; ++time) {
        current = iterate (previous);
        previous = current;
    }
    return previous;
}
const int SIZE = 9;
std::array<unsigned long, SIZE> doGenerations (std::vector<Num> const& original, unsigned int times) {
    std::array<unsigned long, SIZE> previous {0};
    std::array<unsigned long, SIZE> current {0};
    for (Num x : original) {
        ++previous[x];
    }
    for (unsigned int time {0U}; time < times; ++time) {
        //std::cout << previous << "\n";
        current[0] = previous[1];
        current[1] = previous[2];
        current[2] = previous[3];
        current[3] = previous[4];
        current[4] = previous[5];
        current[5] = previous[6];
        current[6] = previous[7] + previous[0];
        current[7] = previous[8];
        current[8] = previous[0];
        previous = current;
    }

    return previous;
}

int main () {
    std::vector<Num> input = getInput ();
    std::vector<Num> final = generations (input, 80);
    std::cout << final.size () << "\n";
    std::array<unsigned long, SIZE> final2 = doGenerations (input, 256);
    std::cout << std::accumulate (final2.begin (), final2.end (), 0UL) << "\n";
    
    return 0;
}