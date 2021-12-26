/// \file 2019Day03.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/3.

#include <iostream>
#include <vector>
#include <utility>
#include <climits>

#include "utilities.hpp"

using Direction = std::string;
using Directions = std::vector<Direction>;
using Coordinates = std::vector<Coordinate>;


std::pair<Directions, Directions> getInput () {
    std::pair<Directions, Directions> wires;
    wires.first = parseCSV (read<std::string> ());
    wires.second = parseCSV (read<std::string> ());
    return wires;
}

Coordinates findCoordinates (Directions const& dirs) {
    Coordinates coords;
    coords.push_back ({0, 0});
    for (Direction const& dir : dirs) {
        if(dir.front () == 'R') {
            coords.push_back ({coords.back ().row, coords.back ().col + std::stoi (dir.substr (1))});
        }
        else if (dir.front () == 'D') {
            coords.push_back ({coords.back ().row + std::stoi (dir.substr (1)), coords.back ().col});
        }
        else if (dir.front () == 'L') {
            coords.push_back ({coords.back ().row, coords.back ().col - std::stoi (dir.substr (1))});
        }
        else if (dir.front () == 'U') {
            coords.push_back ({coords.back ().row - std::stoi (dir.substr (1)), coords.back ().col});
        }
        else {
            throw std::runtime_error ("Unknown direction " + dir);
        }
    }
    return coords;
}

inline unsigned int manhattanDistance (Coordinate const& c1, Coordinate const& c2) {
    return std::abs (c1.row - c2.row) + std::abs (c1.col - c2.col);
}

std::vector<std::pair<Coordinate, unsigned int>> findIntersections (Coordinates const& wire1, Coordinates const& wire2) {
    std::vector<std::pair<Coordinate, unsigned int>> result;
    unsigned int wire1Length {0U};
    Coordinate prevWire1 = wire1.front ();
    for (unsigned int index1 {1U}; index1 < wire1.size (); ++index1) {
        Coordinate currWire1 = wire1.at (index1);
        Coordinate prevWire2 = wire2.front ();
        unsigned int wire2Length {0U};
        for (unsigned int index2 {1U}; index2 < wire2.size (); ++index2) {
            Coordinate currWire2 = wire2.at (index2);
            if (prevWire1.row == currWire1.row && prevWire2.col == currWire2.col) {
                // first is horizontal, second is vertical
                if (prevWire1.row >= std::min (prevWire2.row, currWire2.row) && prevWire1.row <= std::max (prevWire2.row, currWire2.row)) {
                    if (prevWire2.col >= std::min (prevWire1.col, currWire1.col) && prevWire2.col <= std::max (prevWire1.col, currWire1.col)) {
                        if (prevWire1.row != 0 || prevWire2.col != 0) {
                            Coordinate inter {prevWire1.row, prevWire2.col};
                            result.push_back ({inter, wire1Length + wire2Length + manhattanDistance (prevWire1, inter) + manhattanDistance (prevWire2, inter)});
                        }
                    }
                }
            }
            else if(prevWire1.col == currWire1.col && prevWire2.row == currWire2.row) {
                // first is vertical, second is horizontal
                if (prevWire1.col >= std::min (prevWire2.col, currWire2.col) && prevWire1.col <= std::max (prevWire2.col, currWire2.col)) {
                    if (prevWire2.row >= std::min (prevWire1.row, prevWire2.row) && prevWire2.row <= std::max (prevWire1.row, currWire1.row)) {
                        if (prevWire2.row != 0 || prevWire1.col != 0) {
                            Coordinate inter {prevWire2.row, prevWire1.col};
                            result.push_back ({inter, wire1Length + wire2Length + manhattanDistance (prevWire1, inter) + manhattanDistance (prevWire2, inter)});
                        }
                    }
                }
            }
            wire2Length += manhattanDistance (prevWire2, currWire2);
            prevWire2 = currWire2;
        }
        wire1Length += manhattanDistance (prevWire1, currWire1);
        prevWire1 = currWire1;
    }
    return result;
}


unsigned int minManhattanDistance (std::vector<std::pair<Coordinate, unsigned int>> const& intersections) {
    unsigned int lowest = UINT_MAX;
    for (std::pair<Coordinate, unsigned int> const& pair : intersections) {
        unsigned int dist = std::abs (pair.first.row) + std::abs (pair.first.col);
        lowest = std::min (lowest, dist);
    }
    return lowest;
}

unsigned int minLength (std::vector<std::pair<Coordinate, unsigned int>> const& intersections) {
    unsigned int lowest = UINT_MAX;
    for (std::pair<Coordinate, unsigned int> const& pair : intersections) {
        lowest = std::min (lowest, pair.second);
    }
    return lowest;
}

int main () {
    std::pair<Directions, Directions> dirs = getInput ();
    Coordinates wire1 = findCoordinates (dirs.first);
    Coordinates wire2 = findCoordinates (dirs.second);
    std::vector<std::pair<Coordinate, unsigned int>> intersections = findIntersections (wire1, wire2);
    std::cout << minManhattanDistance (intersections) << "\n";
    std::cout << minLength (intersections) << "\n";
    return 0;
}
