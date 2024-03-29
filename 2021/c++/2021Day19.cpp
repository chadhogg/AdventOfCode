/// \file 2021Day19.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/19.

// Key insight: If sensor A sees two beacons that are X distance from each other and sensor B sees two beacons that are X distance from each other,
//   then those two beacons are likely to be the same beacons.
// We can determine which ones in the pair match each other by looking for other pairs that involve one but not the other.

// Second key insight: If those beacons are the same, there is an affine transformation from sensor B's view of the world to sensor A's,
//   and we can find that transformation matrix by solving a system of linear equations.
// Specifically, any beacon gives us three of the equations, with [x,y,z] being the original location, [x',y',z'] being location from sensor 0's perspective.
//   x'   a b c d   x   ax + by + cz + d
//   y'   e f g h   y   ex + fy + gz + h
//   z' = i j k l * z = ix + jy + kz + l
//   1    0 0 0 1   1   0  + 0  + 0  + 1
//
//   xa + yb + zc + 1d + 0e + 0f + 0g + 0h + 0i + 0j + 0k + 0l = x'
//   0a + 0b + 0c + 0d + xe + yf + zg + 1h + 0i + 0j + 0k + 0l = y'
//   0a + 0b + 0c + 0d + 0e + 0f + 0g + 0h + xi + yj + zk + 1l = z'
// After solving the system of linear equations to find a, b, ... l we can apply it to all beacons and the sensor itself to find true coordinates.


#include <iostream>
#include <cstdio>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

#include "utilities.hpp"



/// A 3-dimensional vector in which all compoennts are integers.  Notably can be used to model a point in 3-D space.
struct Vec3 {
    /// The three components of the vector.
    long m_x, m_y, m_z;

    /// \brief Calculates the Euclidean distance between this point and another point.
    /// \param[in] other The other point.
    /// \return The Euclidean distance between myself and it.
    double distance (Vec3 const& other) const {
        long xDiff = m_x - other.m_x;
        long yDiff = m_y - other.m_y;
        long zDiff = m_z - other.m_z;
        return sqrt (xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
    }

    /// \brief Tests whether or not two vectors are identical.
    /// \param[in] other The other vector.
    /// \return True if this vector and the other vector are the same.
    bool operator== (Vec3 const& other) const {
        return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
    }
};

/// \brief An object that hashes Vec3s.
template<>
struct std::hash<Vec3> {
    /// \brief Hashes a Vec3.
    /// \param[in] thing A Vec3 that should be hashed.
    /// \return That Vec3's hash value.
    std::size_t operator()(Vec3 const& thing) const {
        return thing.m_x + thing.m_y * 7 + thing.m_z * 13;
    }
};

/// \brief Inserts a vector into an output stream.
/// \param[inout] out The stream.
/// \param[in] vec The vector.
/// \return The stream.
std::ostream& operator<< (std::ostream & out, Vec3 const& vec) {
    out << vec.m_x << "," << vec.m_y << "," << vec.m_z;
    return out;
}



/// A system of linear equations, modeled as an augmented matrix.
class SystemOfEquations {
public:
    using Row = std::vector<double>;
    using Matrix = std::vector<Row>;

    /// \brief Adds an equation to the system.
    /// \param[in] row A vector of coefficient values.
    /// \post This equation has been added to the system.
    void addEquation (Row const& row) {
        m_matrix.push_back (row);
    }

    /// \brief Attempts to solve the system of equations using Gauss-Jordan Elimination.
    /// \post The equations are solved such that each has the value of one variable.
    /// \return The values of each of the variables.
    std::vector<double> solve () {
        moveZeroesToBottom ();
        triangularForm ();
        if (hasAllZeroesRow ()) {
            throw std::runtime_error ("There are an infinite number of solutions to the system of equations.");
        }
        else if(hasAlmostAllZeroesRow ()) {
            throw std::runtime_error ("There is no solution to the system of equations.");
        }
        reduce ();
        backSubstitute ();
        std::vector<double> results;
        for (Row const& row : m_matrix) {
            results.push_back (row.back ());
        }
        return results;
    }


private:

    inline static bool approxEqual (double x, double y) {
        return std::abs (x - y) < 0.00001;
    }

    void swapRows (unsigned int rowIndexA, unsigned int rowIndexB) {
        Row temp = m_matrix[rowIndexA];
        m_matrix[rowIndexA] = m_matrix[rowIndexB];
        m_matrix[rowIndexB] = temp;
    }

    unsigned int countLeadingZeroes (unsigned int rowIndex) const {
        for (unsigned int index {0U}; index < m_matrix.at (rowIndex).size (); ++index) {
            if (!approxEqual (m_matrix.at (rowIndex).at (index), 0.0f)) {
                return index;
            }
        }
        return m_matrix.at (rowIndex).size ();
    }

    void moveZeroesToBottom () {
        for (int targetRow {(int)m_matrix.size () - 1}; targetRow > 0; --targetRow) {
            unsigned int indexWithMostLeadingZeroes = targetRow;
            for (int otherRow {targetRow - 1}; otherRow >= 0; --otherRow) {
                if (countLeadingZeroes (otherRow) > countLeadingZeroes (indexWithMostLeadingZeroes)) {
                    indexWithMostLeadingZeroes = otherRow;
                }
            }
            swapRows (targetRow, indexWithMostLeadingZeroes);
        }
    }

    void triangularForm () {
        for (unsigned int killerRow {0U}; killerRow < m_matrix.size (); ++killerRow) {
            for (unsigned int killedRow {killerRow + 1U}; killedRow < m_matrix.size (); ++killedRow) {
                double factor = m_matrix.at (killedRow).at (killerRow) / m_matrix.at (killerRow).at (killerRow);
                for (unsigned int col {0U}; col < m_matrix.at (0).size (); ++col) {
                    m_matrix[killedRow][col] = m_matrix[killedRow][col] - factor * m_matrix[killerRow][col];
                }
            }
            moveZeroesToBottom ();
        }
    }

    bool hasAllZeroesRow () const {
        for (unsigned int row {0U}; row < m_matrix.size (); ++row) {
            if (countLeadingZeroes (row) == m_matrix.at (row).size ()) {
                return true;
            }
        }
        return false;
    }

    bool hasAlmostAllZeroesRow () const {
        for (unsigned int row {0U}; row < m_matrix.size (); ++row) {
            if (countLeadingZeroes (row) == m_matrix.at (row).size () - 1) {
                return true;
            }
        }
        return false;
    }

    void reduce () {
        for (unsigned int row {0U}; row < m_matrix.size (); ++row) {
            double factor = m_matrix.at (row).at (row);
            for (unsigned int col {0U}; col < m_matrix.at (row).size (); ++col) {
                m_matrix[row][col] /= factor;
            }
        }
    }

    void backSubstitute () {
        for (int rowUsed {(int)m_matrix.size () - 1}; rowUsed >= 0; --rowUsed) {
            for (int rowModified {rowUsed - 1}; rowModified >= 0; --rowModified) {
                double factor = m_matrix.at (rowModified).at (rowUsed);
                for (unsigned int col {0U}; col < m_matrix.at (0).size (); ++col) {
                    m_matrix[rowModified][col] -= factor * m_matrix[rowUsed][col];
                }
            }
        }
    }

    Matrix m_matrix;
};



/// A representation of an affine transformation in 3D space.
class TransformationMatrix {
public:

    /// \brief Constructs a transformation matrix.
    /// \param[in] coefficients The values to be stored in the matrix, in row order.
    TransformationMatrix (std::vector<double> const& coefficients) {
        assert (coefficients.size () == 12);
        for (unsigned int row {0U}; row < m_matrix.size (); ++row) {
            for (unsigned int col {0U}; col < m_matrix[row].size (); ++col) {
                m_matrix[row][col] = coefficients.at (row * m_matrix[row].size () + col);
            }
        }
    }

    /// \brief Transforms a vector.
    /// \param[in] input The input vector.  It is presumed to have a fourth component of 1.
    /// \return The vector after being multiplied on the left by this matrix.
    Vec3 multiply (Vec3 const& input) const {
        Vec3 result;
        result.m_x = lrint (m_matrix[0][0] * input.m_x + m_matrix[0][1] * input.m_y + m_matrix[0][2] * input.m_z + m_matrix[0][3]);
        result.m_y = lrint (m_matrix[1][0] * input.m_x + m_matrix[1][1] * input.m_y + m_matrix[1][2] * input.m_z + m_matrix[1][3]);
        result.m_z = lrint (m_matrix[2][0] * input.m_x + m_matrix[2][1] * input.m_y + m_matrix[2][2] * input.m_z + m_matrix[2][3]);
        return result;
    }

private:
    std::array<std::array<double, 4>, 3> m_matrix;
};


using Beacon = Vec3;
using Index = unsigned int;
using ScanID = unsigned int;
using Distance = double;
using IndexPair = std::pair<Index, Index>;


/// The information about a scanner.
struct Scanner {
    /// It's number.
    ScanID m_id;
    /// A collection of the locations of its beacons.
    std::vector<Beacon> m_beacons;
    /// A map from distances to pairs of beacons, for finding overlapping scanners.
    std::unordered_map<Distance, IndexPair> m_distances;
    /// The location of the scanner, relative to scanner 0.
    Vec3 m_location;
};


using SomeScanners = std::unordered_map<ScanID, Scanner>;


/// \brief Gets the input to the problem.
/// \return A map from scanner numbers to the scanners themselves.
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
            long x, y, z;
            int count = sscanf (line.c_str (), "%ld,%ld,%ld", &x, &y, &z);
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
                Distance distance = current.second.m_beacons.at (index1).distance (current.second.m_beacons.at (index2));
                assert (current.second.m_distances.find (distance) == current.second.m_distances.end ());
                current.second.m_distances.insert ({distance, {index1, index2}});
            }
        }
    }
    return scanners;
}


/// \brief Finds the pairs of beacons in two scanners that have the same distance from each other.
/// \param[in] done A scanner whose position and orientation are already fixed.
/// \param[in] other Another scanner, whose position and orientation are not yet known.
/// \return A vector of pairs in which the first element is a pair of beacon indexes in done and the seocnd is a pair of beacon indexes in other
///   and the distance between the first pair equals the distance between the second pair. 
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

/// \brief Determines which beacon index in the other scanner matches a given index in the finished scanner.
/// \param[in] doneIndex The beacon index within the finished scanner that we are trying to match.  It should be common to both pairs.
/// \param[in] matchA One pair of same distance beacon pairs.
/// \param[in] matchB Another pair of same distance beacon pairs.
/// \return The index from the unfinished scanner that is common to both pairs. 
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

/// \brief Find a plausible mapping from beacon indices in one scanner to beacon indices in another scanner.
/// \param[in] done A scanner whose orientation and position are fixed.
/// \param[in] other A scanner whose orientation and position are still unknown.
/// \param[in] matchingPairs A list of pairs of beacon pairs that are the same distance from each other from both scanner's perspectives.
/// \return A map from beacon indices in the done scanner to beacon indices in the other scanner.
/// \note This is a fuzzy process where we could get false positives.  If something looks like it isnt' working out, we just silently discard it.
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
                                //std::cout << "Weirdly, found a match between " << matchA.first << " and " << matchB.first << " but not " << matchA.second << " and " << matchB.second << "\n";
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
                                //std::cout << "Hmm, beacon " << doneIndex << " seems to be mappable to both " << doneToOther.at (doneIndex) << " and to " << otherIndex << "\n";
                                doneToOther.erase (doneIndex);
                            }
                            if (otherToDone.at (otherIndex) != doneIndex) {
                                //std::cout << "Hmm, beacon " << otherIndex << " seems to be mappable from both " << otherToDone.at (otherIndex) << " and from " << doneIndex << "\n";
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


/// \brief Finds the transformation matrix that transforms one scanner's position to another's.
/// \param[in] done A scanner whose position and orientation are already known.
/// \param[in] other A scanner whose position and orientation are not yet known.
/// \param[in] doneToOther A mapping of beacon indices for the done scanner to beacon indices for the other scanner.
/// \return A transformation matrix that transforms other scanner coordinates to done scanner coordinates.
TransformationMatrix findTransformationMatrix (Scanner const& done, Scanner const& other, std::unordered_map<Index, Index> const& doneToOther) {
    SystemOfEquations system;
    unsigned int numAdded {0U};
    for (unsigned int doneIndex {0U}; numAdded < 12; ++doneIndex) {
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
            system.addEquation (topRow);
            system.addEquation (middleRow);
            system.addEquation (bottomRow);
            numAdded += 3;
        }
    }
    std::vector<double> coefficients = system.solve ();
    return TransformationMatrix (coefficients);
}


/// \brief Finds one scanner whose position and orientation can be determined, and determines them.
/// \param[inout] finished A collection of scanners whose position and orientation are already known.
/// \param[inout] outstanding A collection of scanners whose position and orientation are not yet known.
/// \post One scanner from outstanding has been removed and a transformed version of it added to finished.
void searchForOverlappingScanners (SomeScanners & finished, SomeScanners & outstanding) {
    for (std::pair<ScanID, Scanner> done : finished) {
        for (std::pair<ScanID, Scanner> other : outstanding) {
            std::vector<std::pair<IndexPair, IndexPair>> matches = findSameDistanceBeacons (done.second, other.second);
            std::unordered_map<Index, Index> doneToOther = mapBeacons (done.second, other.second, matches);
            if (doneToOther.size () >= 12) {
                //std::cout << "It appears that scanner " << other.first << " shares visibility with scanner " << done.first << "\n";
                TransformationMatrix transform = findTransformationMatrix (done.second, other.second, doneToOther);
                //std::cout << transform << "\n";
                Scanner revised {other.second};
                for (unsigned int beaconIndex {0U}; beaconIndex < revised.m_beacons.size (); ++beaconIndex) {
                    revised.m_beacons[beaconIndex] = transform.multiply (revised.m_beacons.at (beaconIndex));
                }
                revised.m_location = transform.multiply (revised.m_location);
                outstanding.erase (revised.m_id);
                finished.insert ({revised.m_id, revised});
                break;
            }
        }
    }
}


/// \brief Finds the largest Manhattan distance between two scanners.
/// \param[in] scanners A collection of all of the scanners.
/// \return The largest Manhattan distance between any two scanners.
long furthestDistance (SomeScanners const& scanners) {
    long furthest = 0L;
    for (std::pair<ScanID, Scanner> const& scan1 : scanners) {
        for (std::pair<ScanID, Scanner> const& scan2 : scanners) {
            long distance = std::abs (scan1.second.m_location.m_x - scan2.second.m_location.m_x);
            distance += std::abs (scan1.second.m_location.m_y - scan2.second.m_location.m_y);
            distance += std::abs (scan1.second.m_location.m_z - scan2.second.m_location.m_z);
            if (distance > furthest) { furthest = distance; }
        }
    }
    return furthest;
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
    std::unordered_set<Beacon> allBeacons;
    for (std::pair<ScanID, Scanner> const& scan : finished) {
        for (Beacon const& beacon : scan.second.m_beacons) {
            allBeacons.insert (beacon);
        }
    }
    std::cout << allBeacons.size () << "\n";
    std::cout << furthestDistance (finished) << "\n";
    return 0;
}