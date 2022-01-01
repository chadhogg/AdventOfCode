/// \file 2019Day17.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/17.

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_set>
#include <unordered_map>

#include "utilities.hpp"
#include "intcode.hpp"

using Picture = std::vector<std::vector<char>>;

constexpr char SCAFFOLD = '#';
constexpr char VACUUM_NORTH = '^';
constexpr char VACUUM_WEST = '<';
constexpr char VACUUM_SOUTH = 'v';
constexpr char VACUUM_EAST = '>';
constexpr char SPACE = '.';
constexpr char VISITED = 'o';

Picture generatePicture (NumbersList const& prog) {
    Picture picture;
    picture.push_back ({});
    ICComputer comp (prog, {});
    comp.executeAllInstructions ();
    NumbersList output = comp.getOutputs ();
    for (Number num : output) {
        if (num == '\n') {
            picture.push_back ({});
        }
        else {
            picture.back ().push_back ((char)num);
        }
    }
    return picture;
}

void drawPicture (Picture const& picture) {
    for (std::vector<char> const& vec : picture) {
        for (char c : vec) {
            std::cout << c;
        }
        std::cout << "\n";
    }
}

unsigned int sumIntersectionDistances (Picture const& picture) {
    unsigned int total {0U};
    for (unsigned int row {1U}; row < picture.size () - 1 && !picture[row + 1].empty (); ++row) {
        for (unsigned int col {1U}; col < picture[row].size () - 1; ++col) {
            if (picture[row][col] == SCAFFOLD && picture[row - 1][col] == SCAFFOLD && picture[row + 1][col] == SCAFFOLD && picture[row][col - 1] == SCAFFOLD && picture[row][col + 1] == SCAFFOLD) {
                total += row * col;
            }
        }
    }
    return total;
}

Coordinate ahead (Coordinate const& vacuumLoc, char vacuumVal) {
    switch (vacuumVal) {
        case VACUUM_NORTH: return {vacuumLoc.row - 1, vacuumLoc.col};
        case VACUUM_WEST: return {vacuumLoc.row, vacuumLoc.col - 1};
        case VACUUM_SOUTH: return {vacuumLoc.row + 1, vacuumLoc.col};
        case VACUUM_EAST: return {vacuumLoc.row, vacuumLoc.col + 1};
        default: throw std::runtime_error ("Bad vacuum?");
    }
}

bool canMoveInto (Picture const& picture, Coordinate const& where) {
    if (where.row < 0 || where.col < 0 || where.row >= (int)picture.size () || where.col >= (int)picture[where.row].size ()) { return false; }
    return picture[where.row][where.col] == SCAFFOLD || picture[where.row][where.col] == VISITED;
}

Coordinate toLeft (Coordinate const& vacuumLoc, char vacuumVal) {
    switch (vacuumVal) {
        case VACUUM_NORTH: return ahead (vacuumLoc, VACUUM_WEST);
        case VACUUM_WEST: return ahead (vacuumLoc, VACUUM_SOUTH);
        case VACUUM_SOUTH: return ahead (vacuumLoc, VACUUM_EAST);
        case VACUUM_EAST: return ahead (vacuumLoc, VACUUM_NORTH);
        default: throw std::runtime_error ("Bad vacuum?");
    }
}

Coordinate toRight (Coordinate const& vacuumLoc, char vacuumVal) {
    switch (vacuumVal) {
        case VACUUM_NORTH: return ahead (vacuumLoc, VACUUM_EAST);
        case VACUUM_WEST: return ahead (vacuumLoc, VACUUM_NORTH);
        case VACUUM_SOUTH: return ahead (vacuumLoc, VACUUM_WEST);
        case VACUUM_EAST: return ahead (vacuumLoc, VACUUM_SOUTH);
        default: throw std::runtime_error ("Bad vacuum?");
    }
}

char leftChar (char current) {
    switch (current) {
        case VACUUM_NORTH: return VACUUM_WEST;
        case VACUUM_WEST: return VACUUM_SOUTH;
        case VACUUM_SOUTH: return VACUUM_EAST;
        case VACUUM_EAST: return VACUUM_NORTH;
        default:  throw std::runtime_error ("Bad vacuum?");
    }
}

char rightChar (char current) {
    switch (current) {
        case VACUUM_NORTH: return VACUUM_EAST;
        case VACUUM_WEST: return VACUUM_NORTH;
        case VACUUM_SOUTH: return VACUUM_WEST;
        case VACUUM_EAST: return VACUUM_SOUTH;
        default:  throw std::runtime_error ("Bad vacuum?");
    }
}

NumbersList findCompleteSequence (Picture picture) {
    Coordinate vacuumLoc {0,0};
    std::vector<std::vector<bool>> visited;
    unsigned int numVisited {0U};
    unsigned int numScaffold {0U};
    for (unsigned int row {0U}; row < picture.size (); ++row) {
        visited.push_back ({});
        for (unsigned int col {0U}; col < picture[row].size (); ++col) {
            visited.back ().push_back (false);
            if (picture[row][col] == VACUUM_NORTH || picture[row][col] == VACUUM_WEST || picture[row][col] == VACUUM_SOUTH || picture[row][col] == VACUUM_EAST) {
                vacuumLoc = {(int)row, (int)col};
            }
            else if (picture[row][col] == SCAFFOLD) {
                ++numScaffold;
            }
        }
    }
    NumbersList program;
    while (numVisited < numScaffold) {
        Coordinate forward = ahead (vacuumLoc, picture[vacuumLoc.row][vacuumLoc.col]);
        Coordinate left = toLeft (vacuumLoc, picture[vacuumLoc.row][vacuumLoc.col]);
        Coordinate right = toRight (vacuumLoc, picture[vacuumLoc.row][vacuumLoc.col]);
        if (canMoveInto (picture, forward)) {
            if (program.back () != 'L' && program.back () != 'R') { ++program.back (); }
            else { program.push_back (1); }
            if (picture[forward.row][forward.col] == SCAFFOLD) { ++numVisited; }
            picture[forward.row][forward.col] = picture[vacuumLoc.row][vacuumLoc.col];
            picture[vacuumLoc.row][vacuumLoc.col] = VISITED;
            vacuumLoc = forward;
        }
        else if (canMoveInto (picture, left)) {
            program.push_back ('L');
            picture[vacuumLoc.row][vacuumLoc.col] = leftChar (picture[vacuumLoc.row][vacuumLoc.col]);
        }
        else if(canMoveInto (picture, right)) {
            program.push_back ('R');
            picture[vacuumLoc.row][vacuumLoc.col] = rightChar (picture[vacuumLoc.row][vacuumLoc.col]);
        }
        else {
            throw std::runtime_error ("No ability to do anything useful?");
        }
        //drawPicture (picture);
    }
    return program;
}

std::string movementProgToString (NumbersList const& movement) {
    std::string str = "";
    for (unsigned int index {0U}; index < movement.size (); ++index) {
        if (movement[index] == 'L') {
            str += 'L';
        }
        else if (movement[index] == 'R') {
            str += 'R';
        }
        else if (movement[index] == 'A') {
            str += 'A';
        }
        else if (movement[index] == 'B') {
            str += 'B';
        }
        else if (movement[index] == 'C') {
            str += 'C';
        }
        else {
            str += std::to_string (movement[index]);
        }
        if (index < movement.size () - 1) {
            str += ",";
        }
    }
    return str;
}

std::string replaceWith (std::string const& orig, std::string const& sub, char replacement) {
    std::string result = orig;
    for (unsigned int index {0U}; index < result.size () - sub.size () + 1; ++index) {
        if (result.substr (index, sub.size ()) == sub) {
            result = result.substr (0, index) + replacement + result.substr (index + sub.size ());
        }
    }
    return result;
}

Number notifyRobots (NumbersList prog, std::string const& mainMovement, std::string const& subA, std::string const& subB, std::string const& subC) {
    prog[0] = 2;
    NumbersList input;
    for (char c : mainMovement) {
        input.push_back (c);
    }
    input.push_back ('\n');
    for (char c : subA) {
        input.push_back (c);
    }
    input.push_back ('\n');
    for (char c : subB) {
        input.push_back (c);
    }
    input.push_back ('\n');
    for (char c : subC) {
        input.push_back (c);
    }
    input.push_back ('\n');
    input.push_back ('n');
    input.push_back ('\n');
    ICComputer comp {prog, input};
    comp.executeAllInstructions ();
    return comp.getOutputs ().back ();
}

int main () {
    std::ifstream fin ("../inputs/Day17.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    Picture view = generatePicture (prog);
    //drawPicture (view);
    std::cout << sumIntersectionDistances (view) << "\n";
    NumbersList fullSequence = findCompleteSequence (view);
    std::string fullString = movementProgToString (fullSequence);
    //std::cout << fullString << "\n\n";
    std::string subA = "L,10,L,10,R,6";
    std::string remainder = replaceWith (fullString, subA, 'A');
    //std::cout << remainder << "\n" << subA << "\n\n";
    std::string subB = "R,12,L,12,L,12";
    std::string remainder2 = replaceWith (remainder, subB, 'B');
    //std::cout << remainder2 << "\n" << subA << "\n" << subB << "\n\n";
    std::string subC = "L,6,L,10,R,12,R,12";
    std::string remainder3 = replaceWith (remainder2, subC, 'C');
    //std::cout << remainder3 << "\n" << subA << "\n" << subB << "\n" << subC << "\n\n";
    std::cout << notifyRobots (prog, remainder3, subA, subB, subC) << "\n";
    return 0;
}
