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
#include <unordered_set>

#include "utilities.hpp"

// Stolen from my computer graphics course.
#include "Vector3.hpp"
#include "Matrix3.hpp"

using Beacon = Vector3;
using Index = unsigned int;
using ScanID = unsigned int;
using Distance = float;
using IndexPair = std::pair<Index, Index>;

struct Scanner {
    unsigned int m_id;
    std::vector<Beacon> m_beacons;
    std::unordered_map<Distance, IndexPair> m_distances;
};

using SomeScanners = std::unordered_map<ScanID, Scanner>;

std::vector<Matrix3> g_allRotationMatrices;

SomeScanners getInput () {
    SomeScanners scanners;
    Scanner scanner;
    std::string line;
    bool workingOnScanner = false;
    ScanID id;
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

    for (std::pair<const ScanID, Scanner> & current : scanners) {
        for (Index index1 {0U}; index1 < current.second.m_beacons.size (); ++index1) {
            for (Index index2 {index1 + 1U}; index2 < current.second.m_beacons.size (); ++index2) {
                Vector3 oneToOther = current.second.m_beacons.at (index1) - current.second.m_beacons.at (index2);
                Distance distance = oneToOther.length ();
                assert (current.second.m_distances.find (distance) == current.second.m_distances.end ());
                current.second.m_distances.insert ({distance, {index1, index2}});
            }
        }
    }
    return scanners;
}


std::vector<std::pair<IndexPair, IndexPair>> findSameDistanceBeacons (Scanner const& done, Scanner const& other) {
    std::vector<std::pair<IndexPair, IndexPair>> beaconPairPairs;
    for (std::pair<Distance, IndexPair> const& donePair : done.m_distances) {
        auto iter = other.m_distances.find (donePair.first);
        if (iter != other.m_distances.end ()) {
            beaconPairPairs.push_back ({donePair.second, (*iter).second});
        }
    }
    return beaconPairPairs;
}

Index findCommonIndex (Index doneIndex, std::pair<IndexPair, IndexPair> const& matchA, std::pair<IndexPair, IndexPair> const& matchB) {
    assert (matchA.first.first == doneIndex || matchA.first.second == doneIndex);
    assert (matchB.first.first == doneIndex || matchB.first.second == doneIndex);
    IndexPair const& otherMatchA = matchA.second;
    IndexPair const& otherMatchB = matchB.second;
    // There should be one number that appears in both otherMatchA and otherMatchB, and we want to map doneIndex to it.
    if (otherMatchA.first == otherMatchB.first || otherMatchA.first == otherMatchB.second) { return otherMatchA.first; }
    else if (otherMatchA.second == otherMatchB.first || otherMatchA.second == otherMatchB.second) { return otherMatchA.second; }
    else { throw std::runtime_error ("There was no number in common between " + to_string (otherMatchA) + " and " + to_string (otherMatchB)); }
}

std::unordered_map<Index, Index> mapBeacons (Scanner const& done, Scanner const& other, std::vector<std::pair<IndexPair, IndexPair>> const& matchingPairs) {
    std::unordered_map<Index, Index> doneToOther;
    std::unordered_map<Index, Index> otherToDone;
    std::unordered_set<Index> doneIndices;
    for (std::pair<IndexPair, IndexPair> const& match : matchingPairs) {
        doneIndices.insert (match.first.first);
        doneIndices.insert (match.first.second);
    }

    for (Index doneIndex : doneIndices) {
        for (std::pair<IndexPair, IndexPair> const& matchA : matchingPairs) {
            if (matchA.first.first == doneIndex || matchA.first.second == doneIndex) {
                Index matchADoneOther = (matchA.first.first == doneIndex ? matchA.first.second : matchA.first.first);
                for (std::pair<IndexPair, IndexPair> const& matchB : matchingPairs) {
                    if (matchB.first.first == doneIndex || matchB.first.second == doneIndex) {
                        Index matchBDoneOther = (matchB.first.first == doneIndex ? matchB.first.second : matchB.first.first);
                        if (matchADoneOther != matchBDoneOther) {
                            // We have found two different pairs the involve the index we are currently working on.
                            Index otherIndex = findCommonIndex (doneIndex, matchA, matchB);
                            if (doneToOther.find (doneIndex) == doneToOther.end ()) {
                                doneToOther.insert ({doneIndex, otherIndex});
                            }
                            if (otherToDone.find (otherIndex) == otherToDone.end ()) {
                                otherToDone.insert ({otherIndex, doneIndex});
                            }
                            assert (doneToOther.at (doneIndex) == otherIndex);
                            assert (otherToDone.at (otherIndex) == doneIndex);
                        }
                    }
                }
            }
        }
    }

    //if (doneToOther.size () != doneIndices.size ()) {
    //    std::cout << "Some indices could not be mapped.\n";
    //}
    return doneToOther;
}

void searchForOverlappingScanners (SomeScanners & finished, SomeScanners & outstanding) {
    for (std::pair<ScanID, Scanner> done : finished) {
        for (std::pair<ScanID, Scanner> other : outstanding) {
            std::vector<std::pair<IndexPair, IndexPair>> matches = findSameDistanceBeacons (done.second, other.second);
            std::unordered_map<Index, Index> doneToOther = mapBeacons (done.second, other.second, matches);
            std::cout << "From " << done.first << " to " << other.first << " found " << doneToOther.size () << " mappable beacons.\n";
        }
    }
}

void initGlobals () {
    // Happened to already be in the correct orientation.
    g_allRotationMatrices.push_back (Matrix3 ());
}


/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    initGlobals ();
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