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
#include <unordered_map>

#include "utilities.hpp"

constexpr int HALL_ROW = 1;
constexpr int FIRST_HALL_COL = 1;
constexpr int LAST_HALL_COL = 11;
constexpr int FIRST_SIDE_ROW = 3;
constexpr int SECOND_SIDE_ROW = 2;
constexpr int AMBER_SIDE_COL = 3;
constexpr int BRONZE_SIDE_COL = 5;
constexpr int COPPER_SIDE_COL = 7;
constexpr int DESERT_SIDE_COL = 9;

enum AmphipodType { AMBER, BRONZE, COPPER, DESERT };

AmphipodType typeFromSymbol (char symbol) {
    switch (symbol) {
        case 'A': return AMBER;
        case 'B': return BRONZE;
        case 'C': return COPPER;
        case 'D': return DESERT;
        default: throw std::runtime_error("Unexpected symbol.");
    }
}

struct Amphipod {
    Coordinate m_where;
    AmphipodType m_type;

    unsigned int getEnergyPerStep () const {
        switch (m_type) {
            case AMBER: return 1;
            case BRONZE: return 10;
            case COPPER: return 100;
            case DESERT: return 1000;
            default: throw std::runtime_error ("Unknown amphipod type");
        }
    }

    char getSymbol () const {
        switch (m_type) {
            case AMBER: return 'A';
            case BRONZE: return 'B';
            case COPPER: return 'C';
            case DESERT: return 'D';
            default: throw std::runtime_error ("Unknown amphipod type");
        }
    }

    int getGoalCol () const {
        switch (m_type) {
            case AMBER: return AMBER_SIDE_COL;
            case BRONZE: return BRONZE_SIDE_COL;
            case COPPER: return COPPER_SIDE_COL;
            case DESERT: return DESERT_SIDE_COL;
            default: throw std::runtime_error ("Unknown amphipod type");
        }
    }

    bool isWhereItShouldBe () const {
        return (m_where.row == FIRST_SIDE_ROW || m_where.row == SECOND_SIDE_ROW) && m_where.col == getGoalCol ();
    }

    bool hasValidPosition () const {
        if (m_where.row == HALL_ROW && m_where.col >= FIRST_HALL_COL && m_where.col <= LAST_HALL_COL) { return true; }
        if (m_where.row == FIRST_SIDE_ROW && (m_where.col == AMBER_SIDE_COL || m_where.col == BRONZE_SIDE_COL || m_where.col == COPPER_SIDE_COL || m_where.col == DESERT_SIDE_COL)) { return true; }
        if (m_where.row == SECOND_SIDE_ROW && (m_where.col == AMBER_SIDE_COL || m_where.col == BRONZE_SIDE_COL || m_where.col == COPPER_SIDE_COL || m_where.col == DESERT_SIDE_COL)) { return true; }
        return false;
    }
};

struct GameState {
    std::vector<Amphipod> pods;
    unsigned int energyUsed;

    bool isGoal () const {
        assert (pods.size () == 8);
        for (Amphipod const& pod : pods) {
            if (!pod.isWhereItShouldBe ()) { return false; }
        }
        return true;
    }

    bool isValid () const {
        if (pods.size () != 8) { return false; }
        std::unordered_map<AmphipodType, unsigned int> counts { {AMBER, 0U}, {BRONZE, 0U}, {COPPER, 0U}, {DESERT, 0U}};
        for (Amphipod const& pod : pods) {
            if (!pod.hasValidPosition ()) { return false; }
            ++counts[pod.m_type];
        }
        if (counts.at (AMBER) != 2 || counts.at (BRONZE) != 2 || counts.at (COPPER) != 2 || counts.at (DESERT) != 2) { return false; }
        for (unsigned int index1 {0U}; index1 < pods.size (); ++index1) {
            for (unsigned int index2 {index1 + 1}; index2 < pods.size (); ++index2) {
                if (pods.at (index1).m_where == pods.at (index2).m_where) { return false; }
            }
        }
        return true;
    }

    std::vector<GameState> successors () const {
        std::vector<GameState> result;
        // TODO
        return result;
    }

private:

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
            else {
                bool found = false;
                for (Amphipod const& pod : state.pods) {
                    if (pod.m_where.row == row && pod.m_where.col == col) {
                        out << pod.getSymbol ();
                        found = true;
                    }
                }
                if (!found) { out << "."; }
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
    std::vector<Coordinate> amphipods {{2, 3}, {2, 5}, {2, 7}, {2, 9}, {3, 3}, {3, 5}, {3, 7}, {3, 9}};
    for (Coordinate const& amph : amphipods) {
        state.pods.push_back ({amph, typeFromSymbol (lines[amph.row].at (amph.col))});
    }
    state.energyUsed = 0U;
    assert (state.isValid ());
    return state;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    GameState initial = getInput ();
    std::cout << initial << "\n";
    return 0;
}