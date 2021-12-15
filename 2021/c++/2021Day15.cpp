/// \file 2021Day15.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2021/day/15.

#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>

#include "utilities.hpp"

using Risk = unsigned int;

template<typename T>
using Grid = std::vector<std::vector<T>>;

using Path = std::vector<Coordinate>;

struct CoordinatePlus {
    CoordinatePlus (Coordinate c) 
        : coord {c}, cheapestPath {}, riskToGetHere {0}, estimatedRiskToGoal {0} {
    }
    Coordinate coord;
    Path cheapestPath;
    Risk riskToGetHere;
    Risk estimatedRiskToGoal;

    inline Risk getRisk () const { return riskToGetHere + estimatedRiskToGoal; }
};

constexpr Risk MAX_RISK = 9U;
constexpr Risk MIN_RISK = 1U;

Grid<Risk> getInput () {
    Grid<Risk> problem;
    std::string line;
    while (std::cin >> line) {
        std::vector<Risk> row;
        for (char c : line) {
            row.push_back (c - '0');
        }
        problem.push_back (row);
    }
    return problem;
}

Risk heuristicCost (Coordinate const& current, Coordinate const& goal) {
    Risk manhattan = std::abs (current.row - goal.row) + std::abs (current.col - goal.col);
    return manhattan * MIN_RISK;
}

class CoordinatePlusComparator {
public:
    inline bool operator() (CoordinatePlus const& x, CoordinatePlus const& y) const {
        return x.getRisk () > y.getRisk ();
    }
};

static CoordinatePlusComparator comp {};

CoordinatePlus findCheapestPath (Grid<Risk> const& problem) {
    Coordinate goal {(int)problem.size () - 1, (int)problem.at (0).size () - 1};

    std::vector<CoordinatePlus> open;
    std::unordered_map<Coordinate, Risk> costs;
    
    CoordinatePlus start {{0, 0}};
    start.cheapestPath.push_back ({0, 0});
    start.estimatedRiskToGoal = heuristicCost (start.coord, goal);
    open.push_back (start);
    costs.insert (std::make_pair (start.coord, start.getRisk ()));
    
    while (!open.empty ()) {
        std::pop_heap (open.begin (), open.end (), comp);
        CoordinatePlus current {open.back ()};
        open.pop_back ();

        if (current.coord == goal) {
            return current;
        }

        if (current.getRisk () > costs.at (current.coord)) {
            continue; // already found a cheaper route here
        }

        std::vector<Coordinate> neighbors;
        if (current.coord.row > 0) {
            neighbors.push_back ({current.coord.row - 1, current.coord.col});
        }
        if (current.coord.row < (int)problem.size () - 1) {
            neighbors.push_back ({current.coord.row + 1, current.coord.col});
        }
        if (current.coord.col > 0) {
            neighbors.push_back ({current.coord.row, current.coord.col - 1});
        }
        if (current.coord.col < (int)problem.at (0).size () - 1) {
            neighbors.push_back ({current.coord.row, current.coord.col + 1});
        }
        for (Coordinate const& n : neighbors) {
            CoordinatePlus next {n};
            next.cheapestPath = current.cheapestPath;
            next.cheapestPath.push_back (n);
            next.riskToGetHere = current.riskToGetHere + problem.at (n.row).at (n.col);
            next.estimatedRiskToGoal = heuristicCost (next.coord, goal);
            if (costs.find (n) == costs.end () || costs.at (n) > next.getRisk ()) {
                open.push_back (next);
                std::push_heap (open.begin (), open.end (), comp);
                costs[n] = next.getRisk ();
            }
        }

    }
    
    throw std::runtime_error ("No path found.");
}

/// \brief Runs the prorgram.
/// \return Always 0.
int main () {
    Grid<Risk> problem = getInput ();
    CoordinatePlus answer1 = findCheapestPath (problem);
    std::cout << answer1.riskToGetHere << "\n";
    return 0;
}