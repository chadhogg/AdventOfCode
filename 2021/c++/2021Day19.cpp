/// \file 2021Day19.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/19.

// Key insight: If sensor A sees two beacons that are X distance from each other and sensor B sees two beacons that are X distance from each other,
//   then those two beacons are likely to be the same beacons.
// We will assume the input was generated such that if we find 12 such beacons, they definitely are the same.


#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>

#include "utilities.hpp"

// Stolen from my computer graphics course.
#include "Vector3.hpp"

using Beacon = Vector3;
using IndexPair = std::pair<unsigned int, unsigned int>;

struct Scanner {
    unsigned int m_id;
    std::vector<Beacon> m_beacons;
    std::unordered_map<float, IndexPair> m_distances;
};

using SomeScanners = std::unordered_map<unsigned int, Scanner>;

SomeScanners getInput () {
    SomeScanners scanners;
    Scanner scanner;
    std::string line;
    bool workingOnScanner = false;
    unsigned int id;
    while (std::getline (std::cin, line)) {
        if (line == "") {
            assert (workingOnScanner);
            scanners.insert ({scanner.m_id, scanner});
            workingOnScanner = false;
        }
        else if (sscanf (line.c_str (), "--- scanner %u ---", &id) == 1) {
            assert (!workingOnScanner);
            scanner = {};
            scanner.m_id = id;
            workingOnScanner = true;
        }
        else {
            assert (workingOnScanner);
            float x, y, z;
            int count = sscanf (line.c_str (), "%f,%f,%f", &x, &y, &z);
            assert (count == 3);
            scanner.m_beacons.push_back ({x, y, z});
        }
    }
    if (workingOnScanner) {
        scanners.insert ({scanner.m_id, scanner});
    }

    for (std::pair<const unsigned int, Scanner> & current : scanners) {
        for (unsigned int index1 {0U}; index1 < current.second.m_beacons.size (); ++index1) {
            for (unsigned int index2 {index1 + 1U}; index2 < current.second.m_beacons.size (); ++index2) {
                Vector3 oneToOther = current.second.m_beacons.at (index1) - current.second.m_beacons.at (index2);
                float distance = oneToOther.length ();
                assert (current.second.m_distances.find (distance) == current.second.m_distances.end ());
                current.second.m_distances.insert ({distance, {index1, index2}});
            }
        }
    }
    return scanners;
}


std::vector<std::pair<IndexPair, IndexPair>> findSameDistanceBeacons (Scanner const& done, Scanner const& other) {
    std::vector<std::pair<IndexPair, IndexPair>> beaconPairPairs;
    for (std::pair<float, IndexPair> const& donePair : done.m_distances) {
        auto iter = other.m_distances.find (donePair.first);
        if (iter != other.m_distances.end ()) {
            beaconPairPairs.push_back ({donePair.second, (*iter).second});
        }
    }
    return beaconPairPairs;
}

void searchForOverlappingScanners (SomeScanners & finished, SomeScanners & outstanding) {
    for (std::pair<unsigned int, Scanner> done : finished) {
        for (std::pair<unsigned int, Scanner> other : outstanding) {
            std::vector<std::pair<IndexPair, IndexPair>> matches = findSameDistanceBeacons (done.second, other.second);
            std::cout << "From " << done.first << " to " << other.first << " found " << matches.size () << " matches.\n";
        }
    }
}


/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    SomeScanners problem = getInput ();
    std::cout << problem.size () << "\n";
    SomeScanners finished;
    SomeScanners outstanding;
    finished.insert ({0U, problem.at (0U)});
    outstanding = problem;
    outstanding.erase (0U);
    searchForOverlappingScanners (finished, outstanding);
    return 0;
}