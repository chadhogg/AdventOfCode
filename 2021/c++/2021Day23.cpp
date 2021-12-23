/// \file 2021Day23.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/23.



#include <iostream>
#include <string>
#include <cstdio>
#include <cassert>
#include <vector>
#include <climits>
#include <unordered_set>

#include "utilities.hpp"

constexpr unsigned int HALL_ROW = 1;
constexpr unsigned int FIRST_HALL_COL = 1;
constexpr unsigned int LAST_HALL_COL = 11;
constexpr unsigned int FIRST_SIDE_ROW = 3;
constexpr unsigned int SECOND_SIDE_ROW = 2;
constexpr unsigned int AMBER_SIDE_COL = 3;
constexpr unsigned int BRONZE_SIDE_COL = 5;
constexpr unsigned int COPPER_SIDE_COL = 7;
constexpr unsigned int DESERT_SIDE_COL = 9;

constexpr unsigned int AMBER_ENERGY = 1;
constexpr unsigned int BRONZE_ENERGY = 10;
constexpr unsigned int COPPER_ENERGY = 100;
constexpr unsigned int DESERT_ENERGY = 1000;

struct GameState {
    Coordinate amber1, amber2, bronze1, bronze2, copper1, copper2, desert1, desert2;
    unsigned int energyUsed;

    bool isGoal () const {
        return ((amber1.row == FIRST_SIDE_ROW || amber1.row == SECOND_SIDE_ROW) && amber1.col == AMBER_SIDE_COL &&
                (amber2.row == FIRST_SIDE_ROW || amber2.row == SECOND_SIDE_ROW) && amber2.col == AMBER_SIDE_COL &&
                (bronze1.row == FIRST_SIDE_ROW || bronze1.row == SECOND_SIDE_ROW) && bronze1.col == BRONZE_SIDE_COL &&
                (bronze2.row == FIRST_SIDE_ROW || bronze2.row == SECOND_SIDE_ROW) && bronze2.col == BRONZE_SIDE_COL &&
                (copper1.row == FIRST_SIDE_ROW || copper1.row == SECOND_SIDE_ROW) && copper1.col == COPPER_SIDE_COL &&
                (copper2.row == FIRST_SIDE_ROW || copper2.row == SECOND_SIDE_ROW) && copper2.col == COPPER_SIDE_COL &&
                (desert1.row == FIRST_SIDE_ROW || desert1.row == SECOND_SIDE_ROW) && desert1.col == DESERT_SIDE_COL &&
                (desert2.row == FIRST_SIDE_ROW || desert2.row == SECOND_SIDE_ROW) && desert2.col == DESERT_SIDE_COL);
    }
};

std::ostream & operator<< (std::ostream & out, GameState const& state) {
    constexpr int FIRST_ROW {0};
    constexpr int LAST_ROW {4};
    constexpr int FIRST_COL {0};
    constexpr int LAST_COL {12};
    for (int row = FIRST_ROW; row <= LAST_ROW; ++row) {
        for (int col = FIRST_COL; col <= LAST_COL; ++col) {
            if ((row == 0) ||
                (row == 1 && (col == FIRST_COL || col == LAST_COL)) ||
                (row == 2 && (col <= 2 || col == 4 || col == 6 || col == 8 || col >= 10)) ||
                (row == 3 && (col == 2 || col == 4 || col == 6 || col == 8 || col == 10)) ||
                (row == 4 && col >= 2 && col <= 10)) {
                out << "#";
            }
            else if (row >= 3 && (col < 2 || col > 10)) {
                out << " ";
            }
            else if ((state.amber1.row == row && state.amber1.col == col) || (state.amber2.row == row && state.amber2.col == col)) {
                out << "A";
            }
            else if ((state.bronze1.row == row && state.bronze1.col == col) || (state.bronze2.row == row && state.bronze2.col == col)) {
                out << "B";
            }
            else if ((state.copper1.row == row && state.copper1.col == col) || (state.copper2.row == row && state.copper2.col == col)) {
                out << "C";
            }
            else if ((state.desert1.row == row && state.desert1.col == col) || (state.desert2.row == row && state.desert2.col == col)) {
                out << "D";
            }
            else {
                out << ".";
            }
        }
        out << "\n";
    }
    out << state.energyUsed << "E\n";
    return out;
}


GameState getInput () {
    GameState state;
    std::string lines[5];
    std::string firstLine, secondLine, thirdLine, fourthLine, fifthLine;
    for (unsigned int num {0U}; num < 5; ++num) {
        std::getline (std::cin, lines[num]);
    }
    assert (lines[0] == "#############");
    assert (lines[1] == "#...........#");
    assert (lines[2].substr (0, 3) == "###" && lines[2].at (4) == '#' && lines[2].at (6) == '#' && lines[2].at (8) == '#' && lines[2].substr (10, 3) == "###");
    assert (lines[3].substr (0, 3) == "  #" && lines[3].at (4) == '#' && lines[3].at (6) == '#' && lines[3].at (8) == '#' && lines[3].at (10) == '#');
    assert (lines[4] == "  #########");
    unsigned int numAmber {0U}, numBronze {0U}, numCopper {0U}, numDesert {0U};
    std::vector<Coordinate> amphipods {{2, 3}, {2, 5}, {2, 7}, {2, 9}, {3, 3}, {3, 5}, {3, 7}, {3, 9}};
    for (Coordinate const& amph : amphipods) {
        if (lines[amph.row].at (amph.col) == 'A') {
            if (numAmber == 0) { state.amber1 = amph; }
            else { state.amber2 = amph; }
            ++numAmber;
        }
        else if (lines[amph.row].at (amph.col) == 'B') {
            if (numBronze == 0) { state.bronze1 = amph; }
            else { state.bronze2 = amph; }
            ++numBronze;
        }
        else if (lines[amph.row].at (amph.col) == 'C') {
            if (numCopper == 0) { state.copper1 = amph; }
            else { state.copper2 = amph; }
            ++numCopper;
        }
        else if (lines[amph.row].at (amph.col) == 'D') {
            if (numDesert == 0) { state.desert1 = amph; }
            else { state.desert2 = amph; }
            ++numDesert;
        }
        else {
            assert (false && "impossible");
        }
    }
    assert (numAmber == 2 && numBronze == 2 && numCopper == 2 && numDesert == 2);
    state.energyUsed = 0U;
    return state;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    GameState initial = getInput ();
    std::cout << initial << "\n";
    return 0;
}