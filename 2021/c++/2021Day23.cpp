/// \file 2021Day23.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/23.

// Note: It took 20.797s to solve the sample problem without hashing.
// It's down to   0.983s with hashing.

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
constexpr int LOWER_SIDE_ROW = 3;
constexpr int UPPER_SIDE_ROW = 2;
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
        return (m_where.row == LOWER_SIDE_ROW || m_where.row == UPPER_SIDE_ROW) && m_where.col == getGoalCol ();
    }

    bool hasValidPosition () const {
        if (m_where.row == HALL_ROW && m_where.col >= FIRST_HALL_COL && m_where.col <= LAST_HALL_COL) { return true; }
        if (m_where.row == LOWER_SIDE_ROW && (m_where.col == AMBER_SIDE_COL || m_where.col == BRONZE_SIDE_COL || m_where.col == COPPER_SIDE_COL || m_where.col == DESERT_SIDE_COL)) { return true; }
        if (m_where.row == UPPER_SIDE_ROW && (m_where.col == AMBER_SIDE_COL || m_where.col == BRONZE_SIDE_COL || m_where.col == COPPER_SIDE_COL || m_where.col == DESERT_SIDE_COL)) { return true; }
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

    bool isSpaceOpen (Coordinate const& where) const {
        for (Amphipod const& pod : pods) {
            if (pod.m_where == where) { return false; }
        }
        return true;
    }

    std::vector<GameState> successors () const {
        std::vector<GameState> result;
        std::vector<GameState> awayToHome = getAwayToHomeMoves ();
        std::vector<GameState> hallToHome = getHallwayToHomeMoves ();
        std::vector<GameState> awayToHall = getAwayToHallMoves ();
        result.insert (result.end (), awayToHall.begin (), awayToHall.end ());
        result.insert (result.end (), hallToHome.begin (), hallToHome.end ());
        result.insert (result.end (), awayToHome.begin (), awayToHome.end ());
        return result;
    }

private:

    Amphipod & findAmphipod (Coordinate const& where) {
        for (Amphipod & pod : pods) {
            if (pod.m_where == where) { return pod; }
        }
        throw std::runtime_error ("Couldn't find amphipod.");
    }

    Amphipod const& findAmphipod (Coordinate const& where) const {
        for (Amphipod const& pod : pods) {
            if (pod.m_where == where) { return pod; }
        }
        throw std::runtime_error ("Couldn't find amphipod.");
    }
    bool isInFinalPosition (Amphipod const& pod) const {
        if (pod.m_where.col != pod.getGoalCol ()) { return false; }
        else {
            if (pod.m_where.row == LOWER_SIDE_ROW) { return true; }
            else if (pod.m_where.row == UPPER_SIDE_ROW) {
                Coordinate below {pod.m_where.row + 1, pod.m_where.col};
                if (!isSpaceOpen (below) && findAmphipod (below).m_type == pod.m_type) { return true; }
                else { return false; }
            }
            else { return false; }
        }
    }

    std::vector<GameState> getHallwayToHomeMoves () const {
        std::vector<GameState> successors;
        for (Amphipod const& amph : pods) {
            if (amph.m_where.row != HALL_ROW) { continue; }
            if (!isSpaceOpen ({UPPER_SIDE_ROW, amph.getGoalCol ()})) { continue; }
            if (!isSpaceOpen ({LOWER_SIDE_ROW, amph.getGoalCol ()}) && findAmphipod ({LOWER_SIDE_ROW, amph.getGoalCol ()}).m_type != amph.m_type) { continue; }
            GameState successor {*this};
            Amphipod& current = successor.findAmphipod (amph.m_where);
            if (successor.moveHomeOrDieTrying (current)) {
                successors.push_back (successor);
            }            
        }
        return successors;
    }

    std::vector<GameState> getAwayToHomeMoves () const {
        std::vector<GameState> successors;
        for (Amphipod const& amph : pods) {
            if (amph.m_where.row == HALL_ROW) { continue; }
            if (amph.m_where.col == amph.getGoalCol ()) { continue; }
            if (!isSpaceOpen ({UPPER_SIDE_ROW, amph.getGoalCol ()})) { continue; }
            if (!isSpaceOpen ({LOWER_SIDE_ROW, amph.getGoalCol ()}) && findAmphipod ({LOWER_SIDE_ROW, amph.getGoalCol ()}).m_type != amph.m_type) { continue; }
            GameState successor {*this};
            Amphipod& current = successor.findAmphipod (amph.m_where);
            if (successor.moveIntoHallwayOrDieTrying (current) && successor.moveHomeOrDieTrying (current)) {
                successors.push_back (successor);
            }
        }
        return successors;
    }

    std::vector<GameState> getAwayToHallMoves () const {
        std::vector<GameState> successors;
        for (Amphipod const& amph : pods) {
            if (amph.m_where.row == HALL_ROW) { continue; }
            if (isInFinalPosition (amph)) { continue; }
            GameState successor {*this};
            Amphipod& current = successor.findAmphipod (amph.m_where);
            if (!successor.moveIntoHallwayOrDieTrying (current)) { continue; }
            GameState westSuccessor {successor};
            Amphipod& westCurrent = westSuccessor.findAmphipod (current.m_where);
            while (westSuccessor.moveWestOrDieTrying (westCurrent)) {
                if (westCurrent.m_where.col != AMBER_SIDE_COL && westCurrent.m_where.col != BRONZE_SIDE_COL && westCurrent.m_where.col != COPPER_SIDE_COL && westCurrent.m_where.col != DESERT_SIDE_COL) {
                    successors.push_back (westSuccessor);
                }
            }
            GameState eastSuccessor {successor};
            Amphipod& eastCurrent = eastSuccessor.findAmphipod (current.m_where);
            while (eastSuccessor.moveEastOrDieTrying (eastCurrent)) {
                if (eastCurrent.m_where.col != AMBER_SIDE_COL && eastCurrent.m_where.col != BRONZE_SIDE_COL && eastCurrent.m_where.col != COPPER_SIDE_COL && eastCurrent.m_where.col != DESERT_SIDE_COL) {
                    successors.push_back (eastSuccessor);
                }
            }
        }
        return successors;
    }

    bool moveWestOrDieTrying (Amphipod & pod) {
        assert (pod.m_where.row == HALL_ROW);
        Coordinate west {pod.m_where.row, pod.m_where.col - 1};
        if (west.col >= FIRST_HALL_COL && isSpaceOpen (west)) {
            pod.m_where = west;
            energyUsed += pod.getEnergyPerStep ();
            return true;
        }
        else { return false; }
    }

    bool moveEastOrDieTrying (Amphipod & pod) {
        assert (pod.m_where.row == HALL_ROW);
        Coordinate east {pod.m_where.row, pod.m_where.col + 1};
        if (east.col <= LAST_HALL_COL && isSpaceOpen (east)) {
            pod.m_where = east;
            energyUsed += pod.getEnergyPerStep ();
            return true;
        }
        else { return false; }
    }

    bool moveIntoHallwayOrDieTrying (Amphipod & pod) {
        assert (pod.m_where.row != HALL_ROW);
        if (pod.m_where.row == LOWER_SIDE_ROW) {
            Coordinate north {pod.m_where.row - 1, pod.m_where.col};
            if (isSpaceOpen (north)) {
                pod.m_where = north;
                energyUsed += pod.getEnergyPerStep ();
            }
            else { return false; }
        }
        Coordinate north {pod.m_where.row - 1, pod.m_where.col};
        if (isSpaceOpen (north)) {
            pod.m_where = north;
            energyUsed += pod.getEnergyPerStep ();
            return true;
        }
        else { return false; }
    }

    bool moveHomeOrDieTrying (Amphipod & pod) {
        assert (pod.m_where.row == HALL_ROW);
        while (pod.m_where.col < pod.getGoalCol ()) {
            Coordinate right {pod.m_where.row, pod.m_where.col + 1};
            if (isSpaceOpen (right)) {
                pod.m_where = right;
                energyUsed += pod.getEnergyPerStep ();
            }
            else { return false; }
        }
        while (pod.m_where.col > pod.getGoalCol ()) {
            Coordinate left {pod.m_where.row, pod.m_where.col - 1};
            if (isSpaceOpen (left)) {
                pod.m_where = left;
                energyUsed += pod.getEnergyPerStep ();
            }
            else { return false; }
        }
        Coordinate below {pod.m_where.row + 1, pod.m_where.col};
        if (isSpaceOpen (below)) {
            pod.m_where = below;
            energyUsed += pod.getEnergyPerStep ();
        }
        else { return false; }
        below = {pod.m_where.row + 1, pod.m_where.col};
        if (isSpaceOpen (below)) {
            pod.m_where = below;
            energyUsed += pod.getEnergyPerStep ();
        }
        else {
            assert (findAmphipod (below).m_type == pod.m_type);
        }
        return true;
    }
};

bool operator== (Amphipod const& a1, Amphipod const& a2) {
    return a1.m_type == a2.m_type && a1.m_where == a2.m_where;
}

bool operator== (GameState const& s1, GameState const& s2) {
    for (Amphipod const& a1 : s1.pods) {
        bool found = false;
        for (Amphipod const& a2 : s2.pods) {
            if (a1 == a2) {
                found = true;
                break;
            }
        }
        if (!found) { return false; }
    }
    return true;
}

std::hash<Coordinate> cHasher;

template<>
struct std::hash<Amphipod> {
    std::size_t operator() (Amphipod const& pod) const {
        return cHasher (pod.m_where) + pod.m_type;
    }
};

std::hash<Amphipod> aHasher;

template<>
struct std::hash<GameState> {
    std::size_t operator() (GameState const& state) const {
        std::size_t result = 0;
        for (Amphipod const& pod : state.pods) {
            result += aHasher (pod);
        }
        return result;
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

unsigned int part1 (GameState const& initial) {
    std::vector<GameState> frontier {initial};
    std::unordered_map<GameState, unsigned int> seen;
    unsigned int cheapest = UINT_MAX;
    while (!frontier.empty ()) {
        GameState current = frontier.back ();
        //std::cout << current << "\n";
        frontier.pop_back ();
        if (current.isGoal ()) {
            //std::cout << current << "\n";
            if (current.energyUsed < cheapest) {
                cheapest = current.energyUsed;
            }
        }
        else if (current.energyUsed <= cheapest) {
            std::vector<GameState> successors = current.successors ();
            for (GameState const& successor : successors) {
                auto const iter = seen.find (successor);
                if (iter == seen.end ()) {
                    seen.insert ({successor, successor.energyUsed});
                    frontier.push_back (successor);
                }
                else if ((*iter).second > successor.energyUsed) {
                    seen[successor] = successor.energyUsed;
                    frontier.push_back (successor);
                }
                else {
                    //std::cout << "Not adding to frontier because cheaper version exists.\n";
                    //std::cout << successor << "\n";
                }
            }
        }
    }
    return cheapest;
}

/// \brief Runs the program.
/// \return Always 0.
int main () {
    GameState initial = getInput ();
    std::cout << initial << "\n";
    std::cout << part1 (initial) << "\n";
    return 0;
}