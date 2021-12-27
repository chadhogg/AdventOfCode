/// \file 2019Day06.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/6.

#include <iostream>
#include <string>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "utilities.hpp"

using Orbit = std::pair<std::string, std::string>;

std::vector<Orbit> getInput () {
    std::vector<Orbit> orbits;
    std::string line;
    while (std::cin >> line) {
        std::size_t where = line.find (')');
        orbits.push_back ({line.substr (0, where), line.substr (where + 1)});
    }
    return orbits;
}

unsigned int getDepth (std::vector<Orbit> const& orbits, std::string const& object) {
    static std::unordered_map<std::string, unsigned int> memo;
    auto iter = memo.find (object);
    if (iter == memo.end ()) {
        for (Orbit const& orbit : orbits) {
            if (orbit.second == object) {
                unsigned int depth = getDepth (orbits, orbit.first) + 1;
                memo.insert ({object, depth});
                return depth;
            }
        }
        memo.insert ({object, 0U});
        return 0U;
    }
    else {
        return iter->second;
    }
}

unsigned int sumDepths (std::vector<Orbit> const& orbits) {
    unsigned int total {0U};
    std::unordered_set<std::string> finished;
    for (Orbit const& orbit : orbits) {
        if (finished.count (orbit.first) == 0) {
            total += getDepth (orbits, orbit.first);
            finished.insert (orbit.first);
        }
        if (finished.count (orbit.second) == 0) {
            total += getDepth (orbits, orbit.second);
            finished.insert (orbit.second);
        }
    }
    return total;
}

std::vector<std::string> findAncestors (std::vector<Orbit> const& orbits, std::string object) {
    for (Orbit const& orbit : orbits) {
        if (orbit.second == object) {
            std::vector<std::string> parent = findAncestors (orbits, orbit.first);
            parent.push_back (object);
            return parent;
        }
    }
    return {};
}

unsigned int orbitalTransfers (std::vector<Orbit> const & orbits) {
    std::vector<std::string> you = findAncestors (orbits, "YOU");
    std::vector<std::string> san = findAncestors (orbits, "SAN");
    unsigned int index;
    for (index = 0U; you.at (index) == san.at (index); ++index) {}
    return (you.size () - index - 1) + (san.size () - index - 1);
}


int main () {
    std::vector<Orbit> orbits = getInput ();
    std::cout << sumDepths (orbits) << "\n";
    std::cout << orbitalTransfers (orbits) << "\n";
    return 0;
}
