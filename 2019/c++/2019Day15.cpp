/// \file 2019Day15.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/15.

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

enum Command {
    NORTH = 1,
    SOUTH = 2,
    WEST = 3,
    EAST = 4
};

enum Status {
    HIT_WALL = 0,
    MOVED = 1,
    GOAL = 2
};

std::vector<Coordinate> getNeighbors (Coordinate c) {
    return { {c.row - 1, c.col}, {c.row + 1, c.col}, {c.row, c.col - 1}, {c.row, c.col + 1} };
}

struct Map {
    std::unordered_set<Coordinate> visited;
    std::unordered_set<Coordinate> walls;
    std::unordered_set<Coordinate> goals;
    std::unordered_set<Coordinate> oxygen;
    std::unordered_set<Coordinate> explorable;
    Coordinate drone;
    Coordinate upperLeft;
    Coordinate lowerRight;

    Map () : Map ({0, 0}) {}
    Map (Coordinate const& initialDrone) {
        initialize (initialDrone);
    }

    void draw () const;
    void initialize (Coordinate initialDrone);
    void recordWallHit (Coordinate where);
    void recordMove (Coordinate where);
};

using Path = std::vector<Command>;

void Map::draw () const{
    for (int i = 0; i < 50; ++i) {
        std::cout << "\n";
    }
    for (int row {upperLeft.row}; row <= lowerRight.row; ++row) {
        for (int col {upperLeft.col}; col <= lowerRight.col; ++col) {
            Coordinate here {row, col};
            if (here == drone) { std::cout << "D"; }
            else if (oxygen.count (here) == 1) { std::cout << "o"; }
            else if (goals.count (here) == 1) { std::cout << "g"; }
            else if (visited.count (here) == 1) { std::cout << "."; }
            else if (walls.count (here) == 1) { std::cout << "#"; }
            else { std::cout << " "; }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Map::initialize (Coordinate initialDrone) {
    visited.clear ();
    walls.clear ();
    goals.clear ();
    oxygen.clear ();
    explorable.clear ();
    drone = initialDrone;
    upperLeft = {drone.row - 1, drone.col - 1};
    lowerRight = {drone.row + 1, drone.col + 1};
    visited.insert (drone);
    for (Coordinate const& c : getNeighbors (drone)) {
        explorable.insert (c);
    }
}

void Map::recordWallHit (Coordinate where) {
    walls.insert (where);
    explorable.erase (where);
    upperLeft.row = std::min (upperLeft.row, where.row);
    upperLeft.col = std::min (upperLeft.col, where.col);
    lowerRight.row = std::max (lowerRight.row, where.row);
    lowerRight.col = std::max (lowerRight.col, where.col);
}

void Map::recordMove (Coordinate where) {
    drone = where;
    visited.insert (drone);
    explorable.erase (drone);
    for (Coordinate const& c : getNeighbors (drone)) {
        if (visited.count (c) == 0 && walls.count (c) == 0) {
            explorable.insert (c);
        }
    }
    upperLeft.row = std::min (upperLeft.row, where.row);
    upperLeft.col = std::min (upperLeft.col, where.col);
    lowerRight.row = std::max (lowerRight.row, where.row);
    lowerRight.col = std::max (lowerRight.col, where.col);
}

Coordinate findClosest (std::unordered_map<Coordinate, Path> const& map) {
    Coordinate chosen = map.begin ()->first;
    for (std::pair<Coordinate const, Path> const& pair : map) {
        if (pair.second.size () < map.at (chosen).size ()) {
            chosen = pair.first;
        }
    }
    return chosen;
}

std::pair<Coordinate, Path> pathToClosestGoal (Coordinate start, std::unordered_set<Coordinate> const& goals, std::unordered_set<Coordinate> const& known) {
    std::unordered_map<Coordinate, Path> finished;
    std::unordered_map<Coordinate, Path> visited;
    std::unordered_set<Coordinate> unvisited;
    visited.insert ({start, {}});
    while (true) {
        assert (!visited.empty ());
        Coordinate current = findClosest (visited);
        Path soFar = visited.at (current);
        visited.erase (current);

        if (known.count (current) == 1) {
            std::vector<std::pair<Coordinate, Path>> successors;
            successors.push_back ({{current.row - 1, current.col}, soFar});
            successors.push_back ({{current.row + 1, current.col}, soFar});
            successors.push_back ({{current.row, current.col - 1}, soFar});
            successors.push_back ({{current.row, current.col + 1}, soFar});
            successors[0].second.push_back (NORTH);
            successors[1].second.push_back (SOUTH);
            successors[2].second.push_back (WEST);
            successors[3].second.push_back (EAST);
            for (std::pair<Coordinate, Path> const& s : successors) {
                if (finished.count (s.first) == 0) {
                    if (visited.count (s.first) == 0) {
                        visited.insert ({s.first, s.second});
                    }
                    else if(visited.at (s.first).size () > s.second.size ()) {
                        visited[s.first] = s.second;
                    }
                }
            }
        }
        finished[current] = soFar;
        if (goals.count (current) == 1) {
            return {current, soFar};
        }
    }
}

void exploreManually (NumbersList & prog, Map & map) {
    ICComputer comp;
    comp.loadProgram (prog, {});
    while (!map.explorable.empty ()) {
        map.draw ();
        std::string human;
        while (human != "w" && human != "a" && human != "s" && human != "d") {
            human = read<std::string> ();
        }
        int input = 0;
        switch (human[0]) {
            case 'w': input = NORTH; break;
            case 's': input = SOUTH; break;
            case 'a': input = WEST; break;
            case 'd': input = EAST; break;
        }
        comp.addInput (input);
        Coordinate whereTo = map.drone;
        switch (input) {
            case NORTH: --whereTo.row; break;
            case SOUTH: ++whereTo.row; break;
            case WEST: --whereTo.col; break;
            case EAST: ++whereTo.col; break;
        }
        comp.executeUntilMissingInput ();
        Number output = comp.getOutputs ().back ();
        if (output == HIT_WALL) {
            map.recordWallHit (whereTo);
        }
        else {
            map.recordMove (whereTo);
            if (output == GOAL) { map.goals.insert (map.drone); }
        }
    }
}

void exploreAutomatically (NumbersList & prog, Map & map) {
    ICComputer comp;
    comp.loadProgram (prog, {});
    std::pair<Coordinate, Path> currentGoal = pathToClosestGoal (map.drone, map.explorable, map.visited);
    while (!map.explorable.empty ()) {
        map.draw ();
        if (currentGoal.second.empty ()) {
            currentGoal = pathToClosestGoal (map.drone, map.explorable, map.visited);
        }
        int input = currentGoal.second[0];
        currentGoal.second.erase (currentGoal.second.begin ());
        comp.addInput (input);
        Coordinate whereTo = map.drone;
        switch (input) {
            case NORTH: --whereTo.row; break;
            case SOUTH: ++whereTo.row; break;
            case WEST: --whereTo.col; break;
            case EAST: ++whereTo.col; break;
        }
        comp.executeUntilMissingInput ();
        Number output = comp.getOutputs ().back ();
        if (output == HIT_WALL) {
            map.recordWallHit (whereTo);
        }
        else {
            map.recordMove (whereTo);
            if (output == GOAL) { map.goals.insert (map.drone); }
        }
        std::this_thread::sleep_for (std::chrono::milliseconds (5));
    }
    map.draw ();
}

unsigned int fillWithOxygen (Map & map) {
    unsigned int years {0U};
    for (Coordinate const& c : map.goals) {
        map.oxygen.insert (c);
    }
    while (map.oxygen.size () < map.visited.size ()) {
        map.draw ();
        std::unordered_set<Coordinate> newOxygen;
        for (Coordinate const& c : map.oxygen) {
            newOxygen.insert (c);
            for (Coordinate const& n : getNeighbors (c)) {
                if (map.visited.count (n) == 1) {
                    newOxygen.insert (n);
                }
            }
        }
        map.oxygen = newOxygen;
        ++years;
        std::this_thread::sleep_for (std::chrono::milliseconds (5));
    }
    map.draw ();
    return years;
}

int main () {
    std::ifstream fin ("../inputs/Day15.my.input");
    NumbersList prog = parseNumbersList (read<std::string> (fin));
    fin.close ();
    Map map;
    exploreAutomatically (prog, map);
    unsigned int minutes = fillWithOxygen (map);
    std::cout << pathToClosestGoal ({0, 0}, map.goals, map.visited).second.size () << "\n";
    std::cout << minutes << "\n";
    return 0;
}
