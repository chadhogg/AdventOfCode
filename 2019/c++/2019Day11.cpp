/// \file 2019Day11.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/11.

#include <iostream>
#include <unordered_set>
#include <climits>

#include "utilities.hpp"
#include "intcode.hpp"


enum Facing {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

constexpr int BLACK_INPUT = 0;
constexpr int WHITE_INPUT = 1;
constexpr int TURN_LEFT = 0;
constexpr int TURN_RIGHT = 1;

Facing turnRight (Facing direction) {
    switch (direction) {
        case NORTH: return EAST;
        case EAST: return SOUTH;
        case SOUTH: return WEST;
        case WEST: return NORTH;
        default: throw std::runtime_error ("Unknown direction.");
    }
}

Facing turnLeft (Facing direction) {
    switch (direction) {
        case NORTH: return WEST;
        case WEST: return SOUTH;
        case SOUTH: return EAST;
        case EAST: return NORTH;
        default: throw std::runtime_error ("Unknown direction.");
    }
}

void moveForward (Coordinate & current, Facing const& direction) {
    switch (direction) {
        case NORTH: --current.row; break;
        case EAST: ++current.col; break;
        case SOUTH: ++current.row; break;
        case WEST: --current.col; break;
        default: throw std::runtime_error ("Unknown direction.");
    }
}

std::pair<std::unordered_set<Coordinate>, std::unordered_set<Coordinate>> runProgram (NumbersList const& prog, int startColor) {
    std::unordered_set<Coordinate> paintedWhite;
    std::unordered_set<Coordinate> painted;
    Coordinate currentLocation {0, 0};
    Facing currentDirection {NORTH};
    ICComputer comp;
    comp.loadProgram (prog, {startColor});
    if (startColor) { paintedWhite.insert (currentLocation); }
    while (!comp.isTerminated ()) {
        comp.executeUntilMissingInput ();
        painted.insert (currentLocation);
        if (comp.getOutputs ().at (comp.getOutputs ().size () - 2) == WHITE_INPUT) {
            paintedWhite.insert (currentLocation);
        }
        else {
            paintedWhite.erase (currentLocation);
        }
        if (comp.getOutputs ().back () == TURN_LEFT) {
            currentDirection = turnLeft (currentDirection);
        }
        else {
            currentDirection = turnRight (currentDirection);
        }
        moveForward (currentLocation, currentDirection);
        comp.addInput (paintedWhite.count (currentLocation));
    }
    return {paintedWhite, painted};
}

void drawLetters (std::unordered_set<Coordinate> const& white) {
    int minRow = INT_MAX, maxRow = INT_MIN, minCol = INT_MAX, maxCol = INT_MIN;
    for (Coordinate const& c : white) {
        minRow = std::min (minRow, c.row);
        maxRow = std::max (maxRow, c.row);
        minCol = std::min (minCol, c.col);
        maxCol = std::max (maxCol, c.col);
    }
    for (int row = minRow; row <= maxRow; ++row) {
        for (int col = minCol; col <= maxCol; ++col) {
            if (white.count ({row, col}) == 1) {
                std::cout << "#";
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

int main () {
    NumbersList prog = parseNumbersList (read<std::string> ());
    std::pair<std::unordered_set<Coordinate>, std::unordered_set<Coordinate>> part1 = runProgram (prog, BLACK_INPUT);
    std::cout << part1.second.size () << "\n";
    std::pair<std::unordered_set<Coordinate>, std::unordered_set<Coordinate>> part2 = runProgram (prog, WHITE_INPUT);
    drawLetters (part2.first);
    return 0;
}
