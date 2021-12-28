/// \file 2019Day10.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/10.

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include "utilities.hpp"

using AsteroidField = std::vector<std::vector<bool>>;
using DetectedCounts = std::vector<std::vector<unsigned int>>;


AsteroidField getInput () {
    AsteroidField field;
    std::string line;
    while (std::cin >> line) {
        std::vector<bool> row;
        for (char c : line) {
            if (c == '#') { row.push_back (true); }
            else if (c == '.') { row.push_back (false); }
            else { throw std::runtime_error ("Unknown character " + c); }
        }
        field.push_back (row);
    }
    return field;
}

int greatestCommonDivisor (int x, int y) {
    int larger = std::max (x, y);
    int smaller = std::min (x, y);
    while (smaller != 0) {
        int temp = smaller;
        smaller = larger % smaller;
        larger = temp;
    }
    return larger;
}

unsigned int countDetections (AsteroidField const& field, unsigned int origRow, unsigned int origCol) {
    unsigned int count {0U};
    for (unsigned int row {0U}; row < field.size (); ++row) {
        for (unsigned int col {0U}; col < field.at (row).size (); ++col) {
            if (field.at (row).at (col) && (row != origRow || col != origCol)) {
                int rowDiff = (int)row - (int)origRow;
                int colDiff = (int)col - (int)origCol;
                if (rowDiff == 0) { colDiff /= std::abs (colDiff); }
                else if (colDiff == 0) { rowDiff /= std::abs (rowDiff); }
                else {
                    int gcd = greatestCommonDivisor (std::abs (rowDiff), std::abs (colDiff));
                    rowDiff /= gcd;
                    colDiff /= gcd;
                }
                bool hit = false;
                int r = (int)origRow + rowDiff;
                int c = (int)origCol + colDiff;
                while (!(r == (int)row && c == (int)col) && !hit) {
                    if (field.at (r).at (c)) { hit = true; }
                    r += rowDiff;
                    c += colDiff;
                }
                if (!hit) { ++count; }
            }
        }
    }
    return count;
}

DetectedCounts countDetections (AsteroidField const& field) {
    DetectedCounts counts;
    for (unsigned int row {0U}; row < field.size (); ++row) {
        std::vector<unsigned int> count;
        for (unsigned int col {0U}; col < field.at (row).size (); ++col) {
            if (field.at (row).at (col)) {
                count.push_back (countDetections (field, row, col));
            }
            else {
                count.push_back (0U);
            }
        }
        counts.push_back (count);
    }
    return counts;
}

std::pair<unsigned int, std::pair<int, int>> maxDetectionCount (DetectedCounts const& counts) {
    std::pair<unsigned int, std::pair<int, int>> largest {0U, {0, 0}};
    for (unsigned int row {0U}; row < counts.size (); ++row) {
        for (unsigned int col {0U}; col < counts.at (row).size (); ++col) {
            if (counts.at (row).at (col) > largest.first) {
                largest = {counts.at (row).at (col), {row, col}};
            }
        }
    }
    return largest;
}

double computeAngle (int origRow, int origCol, int row, int col) {
    double colDiff = (col - origCol);
    double rowDiff = (row - origRow);
    if (rowDiff <= 0 && colDiff >= 0) {
        // first quadrant
        return atan (std::abs (colDiff / rowDiff)) * 180 / M_PI;
    }
    else if (rowDiff > 0 && colDiff > 0) {
        // second quadrant
        return atan (std::abs (rowDiff / colDiff)) * 180 / M_PI + 90;
    }
    else if (rowDiff >= 0 && colDiff <= 0) {
        // third quadrant
        return atan (std::abs (colDiff / rowDiff)) * 180 / M_PI + 180;
    }
    else if(rowDiff < 0 && colDiff < 0) {
        // fourth quadrant
        return atan (std::abs (rowDiff / colDiff)) * 180 / M_PI + 270;
    }
    else {
        return -1;
    }
}

std::vector<std::vector<double>> findAngles (AsteroidField const& field, int origRow, int origCol) {
    std::vector<std::vector<double>> angles;
    for (int row {0}; row < (int)field.size (); ++row ) {
        std::vector<double> vec;
        for (int col {0}; col < (int)field.at (row).size (); ++col) {
            if (field.at (row).at (col) && !(row == origRow && col == origCol)) {
                vec.push_back (computeAngle (origRow, origCol, row, col));
            }
            else {
                vec.push_back (-1.0);
            }
        }
        angles.push_back (vec);
    }
    return angles;
}

void testAngles () {
    std::cout << "These should be strictly increasing.\n";
    std::cout << "Angle to (3,5) is " << computeAngle (5, 5, 3, 5) << "\n";
    std::cout << "Angle to (3,6) is " << computeAngle (5, 5, 3, 6) << "\n";
    std::cout << "Angle to (3,7) is " << computeAngle (5, 5, 3, 7) << "\n";
    std::cout << "Angle to (4,7) is " << computeAngle (5, 5, 4, 7) << "\n";
    std::cout << "Angle to (5,6) is " << computeAngle (5, 5, 5, 6) << "\n";
    std::cout << "Angle to (6,7) is " << computeAngle (5, 5, 6, 7) << "\n";
    std::cout << "Angle to (7,7) is " << computeAngle (5, 5, 7, 7) << "\n";
    std::cout << "Angle to (7,6) is " << computeAngle (5, 5, 7, 6) << "\n";
    std::cout << "Angle to (6,5) is " << computeAngle (5, 5, 6, 5) << "\n";
    std::cout << "Angle to (7,4) is " << computeAngle (5, 5, 7, 4) << "\n";
    std::cout << "Angle to (7,3) is " << computeAngle (5, 5, 7, 3) << "\n";
    std::cout << "Angle to (6,3) is " << computeAngle (5, 5, 6, 3) << "\n";
    std::cout << "Angle to (5,4) is " << computeAngle (5, 5, 5, 4) << "\n";
    std::cout << "Angle to (4,3) is " << computeAngle (5, 5, 4, 3) << "\n";
    std::cout << "Angle to (3,3) is " << computeAngle (5, 5, 3, 3) << "\n";
    std::cout << "Angle to (3,4) is " << computeAngle (5, 5, 3, 4) << "\n";
}

std::pair<double, std::pair<int, int>> nextOneToVaporize (AsteroidField const& field, std::vector<std::vector<double>> const& angles, int origRow, int origCol, double prevAngle) {
    std::pair<double, std::pair<int, int>> result {360, {0, 0}};
    for (int row {0}; row < (int)field.size (); ++row) {
        for (int col {0}; col < (int)field.at (row).size (); ++col) {
            if (field.at (row).at (col)) {
                if (angles.at (row).at (col) > prevAngle) {
                    if (angles.at (row).at (col) < result.first) {
                        // found a lower angle than we had found before
                        result = {angles.at (row).at (col), {row, col}};
                    }
                    else if(angles.at (row).at (col) == result.first) {
                        if (std::abs (origRow - row) + std::abs (origCol - col) < std::abs (origRow - result.second.first) + std::abs (origCol - result.second.second)) {
                            // found a closer asteroid at same angle we had found before
                            result = {angles.at (row).at (col), {row, col}};
                        }
                    }
                }
            }
        }
    }
    return result;
}

int get200thVaporization (AsteroidField & field, std::vector<std::vector<double>> & angles, int origRow, int origCol) {
    double prevAngle = -0.5;
    std::pair<double, std::pair<int, int>> result;
    for (unsigned int count {0U}; count < 200U; ++count) {
        result = nextOneToVaporize (field, angles, origRow, origCol, prevAngle);
        if (result.first == 360) {
            prevAngle = -0.5;
            result = nextOneToVaporize (field, angles, origRow, origCol, prevAngle);
        }
        assert (result.first >= 0.0 && result.first <= 360);
        field[result.second.first][result.second.second] = false;
        prevAngle = result.first;
    }
    return result.second.first + result.second.second * 100;
}

int main () {
    AsteroidField field = getInput ();
    DetectedCounts detectedCounts = countDetections (field);
    std::pair<unsigned int, std::pair<int, int>> whereToBuild = maxDetectionCount (detectedCounts);
    std::cout << whereToBuild << "\n";
    std::vector<std::vector<double>> angles = findAngles (field, whereToBuild.second.first, whereToBuild.second.second);
    std::cout << get200thVaporization (field, angles, whereToBuild.second.first, whereToBuild.second.second) << "\n";
    //testAngles ();
    return 0;
}
