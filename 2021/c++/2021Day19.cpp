/// \file 2021Day19.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/19.

// Key insight: If sensor A sees two beacons that are X distance from each other and sensor B sees two beacons that are X distance from each other,
//   then those two beacons are likely to be the same beacons.
// We will assume the input was generated such that if we find 12 such beacons, they definitely are the same.
// Second key insight: If those beacons are the same, there is an affine transformation from sensor B's view of the world to sensor A's,
//   and we can find that transformation matrix by solving a system of linear equations.


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


inline bool approxEqual (double x, double y) {
    return std::abs (x - y) < 0.01;
}


namespace matrix {
    using Matrix = std::vector<std::vector<double>>;

    void swapRows (Matrix & matrix, unsigned int rowIndexA, unsigned int rowIndexB) {
        std::vector<double> temp = matrix[rowIndexA];
        matrix[rowIndexA] = matrix[rowIndexB];
        matrix[rowIndexB] = temp;
    }

    unsigned int countLeadingZeroes (Matrix const& matrix, unsigned int rowIndex) {
        for (unsigned int index {0U}; index < matrix.at (rowIndex).size (); ++index) {
            if (!approxEqual (matrix.at (rowIndex).at (index), 0.0f)) {
                return index;
            }
        }
        return matrix.at (rowIndex).size ();
    }
        unsigned int count {0U};

    void moveZeroesToBottom (Matrix & matrix) {
        for (int targetRow {(int)matrix.size () - 1}; targetRow > 0; --targetRow) {
            unsigned int indexWithMostLeadingZeroes = targetRow;
            for (int otherRow {targetRow - 1}; otherRow >= 0; --otherRow) {
                if (countLeadingZeroes (matrix, otherRow) > countLeadingZeroes (matrix, indexWithMostLeadingZeroes)) {
                    indexWithMostLeadingZeroes = otherRow;
                }
            }
            swapRows (matrix, targetRow, indexWithMostLeadingZeroes);
        }
    }

    void triangularForm (Matrix & matrix) {
        for (unsigned int killerRow {0U}; killerRow < matrix.size (); ++killerRow) {
            for (unsigned int killedRow {killerRow + 1U}; killedRow < matrix.size (); ++killedRow) {
                double factor = matrix.at (killedRow).at (killerRow) / matrix.at (killerRow).at (killerRow);
                for (unsigned int col {0U}; col < matrix.at (0).size (); ++col) {
                    matrix[killedRow][col] = matrix[killedRow][col] - factor * matrix[killerRow][col];
                }
            }
            moveZeroesToBottom (matrix);
        }
    }

    bool hasAllZeroesRow (Matrix const& matrix) {
        for (unsigned int row {0U}; row < matrix.size (); ++row) {
            if (countLeadingZeroes (matrix, row) == matrix.at (row).size ()) {
                return true;
            }
        }
        return false;
    }

    bool hasAlmostAllZeroesRow (Matrix const& matrix) {
        for (unsigned int row {0U}; row < matrix.size (); ++row) {
            if (countLeadingZeroes (matrix, row) == matrix.at (row).size () - 1) {
                return true;
            }
        }
        return false;
    }

    void reduce (Matrix & matrix) {
        for (unsigned int row {0U}; row < matrix.size (); ++row) {
            double factor = matrix.at (row).at (row);
            for (unsigned int col {0U}; col < matrix.at (row).size (); ++col) {
                matrix[row][col] /= factor;
            }
        }
    }

    void backSubstitute (Matrix & matrix) {
        for (int rowUsed {(int)matrix.size () - 1}; rowUsed >= 0; --rowUsed) {
            for (int rowModified {rowUsed - 1}; rowModified >= 0; --rowModified) {
                double factor = matrix.at (rowModified).at (rowUsed);
                for (unsigned int col {0U}; col < matrix.at (0).size (); ++col) {
                    matrix[rowModified][col] -= factor * matrix[rowUsed][col];
                }
            }
        }
    }

    void eliminate (Matrix & matrix) {
        moveZeroesToBottom (matrix);
        triangularForm (matrix);
        if (hasAllZeroesRow (matrix)) {
            throw std::runtime_error ("There are an infinite number of solutions to the system of equations.");
        }
        else if(hasAlmostAllZeroesRow (matrix)) {
            throw std::runtime_error ("There is no solution to the system of equations.");
        }
        reduce (matrix);
        backSubstitute (matrix);
    }
};

Vector3 multiply (matrix::Matrix const& transform, Vector3 const& input) {
    Vector3 result;
    result.m_x = transform.at (0).at (0) * input.m_x + transform.at (0).at (1) * input.m_y + transform.at (0).at (2) * input.m_z + transform.at (0).at (3);
    result.m_y = transform.at (1).at (0) * input.m_x + transform.at (1).at (1) * input.m_y + transform.at (1).at (2) * input.m_z + transform.at (1).at (3);
    result.m_z = transform.at (2).at (0) * input.m_x + transform.at (2).at (1) * input.m_z + transform.at (2).at (2) * input.m_z + transform.at (2).at (3);
    return result;
}


using Beacon = Vector3;
using Index = unsigned int;
using ScanID = unsigned int;
using Distance = double;
using IndexPair = std::pair<Index, Index>;

struct Scanner {
    unsigned int m_id;
    std::vector<Beacon> m_beacons;
    std::unordered_map<Distance, IndexPair> m_distances;
    Vector3 m_location;
};

using SomeScanners = std::unordered_map<ScanID, Scanner>;


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
            double x, y, z;
            int count = sscanf (line.c_str (), "%lf,%lf,%lf", &x, &y, &z);
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
                            if (matchA.second.first != matchB.second.first && matchA.second.first != matchB.second.second && matchA.second.second != matchB.second.first && matchA.second.second != matchB.second.second) {
                                std::cout << "Weirdly, found a match between " << matchA.first << " and " << matchB.first << " but not " << matchA.second << " and " << matchB.second << "\n";
                                break;
                            }
                            // We have found two different pairs the involve the index we are currently working on.
                            Index otherIndex = findCommonIndex (doneIndex, matchA, matchB);
                            if (doneToOther.find (doneIndex) == doneToOther.end ()) {
                                doneToOther.insert ({doneIndex, otherIndex});
                            }
                            if (otherToDone.find (otherIndex) == otherToDone.end ()) {
                                otherToDone.insert ({otherIndex, doneIndex});
                            }
                            if (doneToOther.at (doneIndex) != otherIndex) {
                                std::cout << "Hmm, beacon " << doneIndex << " seems to be mappable to both " << doneToOther.at (doneIndex) << " and to " << otherIndex << "\n";
                                doneToOther.erase (doneIndex);
                            }
                            if (otherToDone.at (otherIndex) != doneIndex) {
                                std::cout << "Hmm, beacon " << otherIndex << " seems to be mappable from both " << otherToDone.at (otherIndex) << " and from " << doneIndex << "\n";
                                otherToDone.erase (otherIndex);
                            }
                        }
                    }
                }
            }
        }
    }
    return doneToOther;
}

matrix::Matrix findTransformationMatrix (Scanner const& done, Scanner const& other, std::unordered_map<Index, Index> const& doneToOther) {
    matrix::Matrix matrix;
    for (unsigned int doneIndex {0U}; matrix.size () < 12; ++doneIndex) {
        if (doneToOther.find (doneIndex) != doneToOther.end ()) {
            Beacon const& doneBeacon = done.m_beacons.at (doneIndex);
            Beacon const& otherBeacon = other.m_beacons.at (doneToOther.at (doneIndex));
            std::vector<double> topRow (13, 0.0);
            topRow[0] = otherBeacon.m_x;
            topRow[1] = otherBeacon.m_y;
            topRow[2] = otherBeacon.m_z;
            topRow[3] = 1.0;
            topRow[12] = doneBeacon.m_x;
            std::vector<double> middleRow (13, 0.0);
            middleRow[4] = otherBeacon.m_x;
            middleRow[5] = otherBeacon.m_y;
            middleRow[6] = otherBeacon.m_z;
            middleRow[7] = 1.0;
            middleRow[12] = doneBeacon.m_y;
            std::vector<double> bottomRow (13, 0.0);
            bottomRow[8] = otherBeacon.m_x;
            bottomRow[9] = otherBeacon.m_y;
            bottomRow[10] = otherBeacon.m_z;
            bottomRow[11] = 1.0;
            bottomRow[12] = doneBeacon.m_z;
            matrix.push_back (topRow);
            matrix.push_back (middleRow);
            matrix.push_back (bottomRow);
        }
    }
    matrix::eliminate (matrix);

    matrix::Matrix transform;
    transform.push_back ({matrix[0][12], matrix[1][12], matrix[2][12], matrix[3][12]});
    transform.push_back ({matrix[4][12], matrix[5][12], matrix[6][12], matrix[7][12]});
    transform.push_back ({matrix[8][12], matrix[9][12], matrix[10][12], matrix[11][12]});
    transform.push_back ({0.0, 0.0, 0.0, 1.0});

    //for (std::pair<Index, Index> mapping : doneToOther) {
        //std::cout << "\nDone:        " << done.m_beacons.at (mapping.first) << "\n";
        //std::cout << "Transformed: " << multiply (transform, other.m_beacons.at (mapping.second)) << "\n";
        //assert (done.m_beacons.at (mapping.first) == multiply (transform, other.m_beacons.at (mapping.second)));
    //}
    return transform;
}

void searchForOverlappingScanners (SomeScanners & finished, SomeScanners & outstanding) {
    for (std::pair<ScanID, Scanner> done : finished) {
        for (std::pair<ScanID, Scanner> other : outstanding) {
            std::vector<std::pair<IndexPair, IndexPair>> matches = findSameDistanceBeacons (done.second, other.second);
            std::unordered_map<Index, Index> doneToOther = mapBeacons (done.second, other.second, matches);
            if (doneToOther.size () >= 12) {
                std::cout << "It appears that scanner " << other.first << " shares visibility with scanner " << done.first << "\n";
                matrix::Matrix transform = findTransformationMatrix (done.second, other.second, doneToOther);
                std::cout << transform << "\n";
                Scanner revised {other.second};
                for (unsigned int beaconIndex {0U}; beaconIndex < revised.m_beacons.size (); ++beaconIndex) {
                    revised.m_beacons[beaconIndex] = multiply (transform, revised.m_beacons.at (beaconIndex));
                }
                revised.m_location = multiply (transform, revised.m_location);
                outstanding.erase (revised.m_id);
                finished.insert ({revised.m_id, revised});
                break;
            }
        }
    }
}



/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    SomeScanners problem = getInput ();
    SomeScanners finished;
    SomeScanners outstanding;
    finished.insert ({0U, problem.at (0U)});
    outstanding = problem;
    outstanding.erase (0U);
    while (!outstanding.empty ()) {
        searchForOverlappingScanners (finished, outstanding);
    }
    return 0;
}